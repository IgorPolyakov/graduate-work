#include "lk_func.h"
#include <QPainter>
#include <QImage>
#include <QDebug>
#include <QFileInfo>
#include <iostream>
#include <math.h>
#include <deprecated/dvfile.h>

#define LVL_PYRAMID 3
#define RESIZE 5
#define SIZE_MAT_INV 2

/*!
 * \brief inversion − Нахождения обратной матрицы(2 x 2).
 * \param [in] A − Ссылка на массив
 */
void inversion(Matx22d &A)
{
    double temp;
    double **E = new double *[A.rows];
    for (int i = 0; i < A.rows; i++)
        E[i] = new double [A.rows];
    for (int i = 0; i < A.rows; i++)
        for (int j = 0; j < A.rows; j++) {
            E[i][j] = 0.0;
            if (i == j)
                E[i][j] = 1.0;
        }
    for (int k = 0; k < A.rows; k++) {
        temp = A(k,k);
        for (int j = 0; j < A.rows; j++) {
            A(k,j) /= temp;
            E[k][j] /= temp;
        }
        for (int i = k + 1; i < A.rows; i++) {
            temp = A(i,k);
            for (int j = 0; j < A.rows; j++) {
                A(i,j) -= A(k,j) * temp;
                E[i][j] -= E[k][j] * temp;
            }
        }
    }
    for (int k = A.rows - 1; k > 0; k--) {
        for (int i = k - 1; i >= 0; i--) {
            temp = A(i,k);
            for (int j = 0; j < A.rows; j++) {
                A(i,j) -= A(k,j) * temp;
                E[i][j] -= E[k][j] * temp;
            }
        }
    }
    for (int i = 0; i < A.rows; i++)
        for (int j = 0; j < A.rows; j++)
            A(i,j) = E[i][j];
    for (int i = 0; i < A.rows; i++)
        delete [] E[i];
    delete [] E;
}

/*!
 * \brief computeGrid − Строит сетку по верх изображения, в точках
 * пересечения ищется вектор оптического потока
 * \param [in] leftImg − указатель на массив яркостей первого кадра
 * \param [in] rightImg − указатель на массив яркостей второго кадра
 * \return двумерный массив содержащий векторное поле, в формате VF
 */
VF2d* computeGrid(Data2Db* leftImg, Data2Db* rightImg)
{
    subSize* kernel = new subSize;

    kernel->rc = g_sizeWindowSeach;
    kernel->step = g_stepForGrid;
    int Vcx = (leftImg->cx()-(2*(kernel->rc+2))-1)/g_stepForGrid;
    int Vcy = (leftImg->cy()-(2*(kernel->rc+2))-1)/g_stepForGrid;
    VF2d *vf = new VF2d(Vcx, Vcy, g_stepForGrid, g_stepForGrid, g_sizeWindowSeach+1, g_sizeWindowSeach+1, DV_ALIGNMENT);
    Data2Db *state = new Data2Db("state",vf->cx(),vf->cy());
    vf->ad() = s_ptr<ProtoData2D>(state);
    for (int i = 0; i < Vcy; ++i) {
        kernel->cy = (i * vf->grid().y) + vf->origin().y;
        for (int j = 0; j < Vcx; ++j) {
            kernel->cx = (j * vf->grid().x) + vf->origin().x;
            vf->lines()[i][j] = computeOptFlow(kernel, leftImg, rightImg);
        }
    }
    delete kernel;
    return vf;
}

/*!
 * \brief computeOptFlow − Вычисление вектора оптического потока
 * \param [in] kernel − структура содержащая сведения о местонахождении
 * пикселя, размерах окна поиска и прочего
 * \param [in] leftImg − массив яркостей первого кадра
 * \param [in] rightImg − массив яркостей второго кадра
 * \return [out] vf − вектор оптического потока
 */
Vec2d computeOptFlow(subSize* kernel, Data2Db* leftImg, Data2Db* rightImg)
{
    double iY = 0.0,   iX = 0.0,   iTX = 0.0, iTY = 0.0, iXY = 0.0;
    double tmpX, tmpY, tmpT;
    Vec2d shiftVectr(0,0);
    int deltaX = 0;
    int deltaY = 0;

    if (g_isDebug) qDebug() << "SubSize:X" << kernel->cx << "Y:" << kernel->cy << "R:" << kernel->rc << "\n";
    for (int k = 0; k < g_iteration; ++k) {
        //Отсчитываем число итераций, для уточнения вектора
        for (int i = (kernel->cy - kernel->rc); i < (kernel->cy + kernel->rc); i++) {
            for (int j = (kernel->cx - kernel->rc); j < (kernel->cx + kernel->rc); j++) {
                tmpX = ((double)leftImg->lines()[i - 1][j] - (double)leftImg->lines()[i + 1][j]) / 2;
                tmpY = ((double)leftImg->lines()[i][j - 1] - (double)leftImg->lines()[i][j + 1]) / 2;
                iX  += tmpX * tmpX;
                iY  += tmpY * tmpY;
                iXY += tmpX * tmpY;

                tmpT = ((double)leftImg->lines()[i + deltaX][j + deltaY] - (double)rightImg->lines()[i + deltaX][j + deltaY]) / 2;
                iTX += tmpX * tmpT;
                iTY += tmpY * tmpT;
            }
        }

        Matx22d A(iX, iXY, iXY, iY);

        Vec2d b(-iTX,-iTY);

        inversion(A);
        shiftVectr = A*b;

        if (kernel->cx + shiftVectr[0] + kernel->rc > leftImg->cx()) {
            break;
        }
        if (kernel->cx + shiftVectr[0] - kernel->rc < 0) {
            break;
        }
        if (kernel->cy + shiftVectr[1] + kernel->rc > leftImg->cy()) {
            break;
        }
        if (kernel->cy + shiftVectr[1] - kernel->rc < 0) {
            break;
        }
        if ((shiftVectr[0] == shiftVectr[0])
            || (shiftVectr[1] == shiftVectr[1])) { //NaN Checking
            deltaX = (int)floor(shiftVectr[0]);
            deltaY = (int)floor(shiftVectr[1]);
        } else {
            if (g_isDebug)  qDebug() << "NaN Error";
            shiftVectr[0] = 0.0;
            shiftVectr[1] = 0.0;
        }
    }
    if (g_isDebug) qDebug() << shiftVectr[0] << shiftVectr[1] << "return";
    return shiftVectr;
}

/*!
 * \brief multiplicMtrxAndVectr − Произведение матрицы на вектор
 * \param [in] **array − указатель на массив
 * \param [in] *vector − указатель на вектор
 * \return [out] tmp − результат произведения
 */
double* multiplicMtrxAndVectr(double** array, int* vector)
{
    double* tmp = new double[SIZE_MAT_INV];
    for (int cnt = 0; cnt < SIZE_MAT_INV; ++cnt) {
        tmp[cnt] = 0;
    }
    for (int i = 0; i < SIZE_MAT_INV; i++) {
        for (int j = 0; j < SIZE_MAT_INV; j++) {
            tmp[i] += array[i][j] * (double)vector[j];
        }
    }
    return tmp;
}

/*!
 * \brief getImageInfo − Получение информации о входном изображении
 * \param [in] image − Изображение
 * \param [in] path − Путь к нему
 */
void getImageInfo(imageInform* image, QString path)
{
    QFileInfo fileImage(path);
    qDebug() << "About image " << fileImage.fileName() << " :: FileSize:"
             << fileImage.size() << "bytes. Size: " << image->height << "x" <<
             image->width;
}

/*!
 * \brief joinImage − Объединение трёх изображений(первого, второго и первого с нанесённым поверх векторным полем)
 * \param [in] img1 − Первое изображение
 * \param [in] img2 − Второе изображение
 * \param [in] img3 − Первого с нанесённым поверх векторным полем
 * \param [in] info − Имя сохраняемого файла
 */
void joinImage(QImage img1, QImage img2, QImage img3, QString info)
{
    QImage result(img1.width() + img2.width() + img3.width(),
                  img1.height(), QImage::Format_ARGB32);
    QPainter paint;
    paint.begin(&result);
    paint.drawImage(0, 0, img1);
    paint.drawImage(img1.width(), 0, img2);
    paint.drawImage(img1.width() + img2.width(), 0, img3);
    result.save(g_outputFolder + "/" + info + ".png");
}

/*!
 * \brief resizeImage − Функция масштабирования изображения, для построения пирамиды уменьшенных изображений
 * \param [in] image − структура содержащая сведения о размерах масштабируемого изображения
 * \param [in] arrGrayPrevious − Указатель на массив яркостей первого кадра
 * \param [in] kK − Коэффициент уменьшения изображения
 * \return [out] − указатель на массив масштабированных изображений
 */
int* resizeImage(imageInform* image, int** arrGrayPrevious, int kK)
{
    int newWidth = (image->width/kK);
    int newHeight= (image->height/kK);
    int tmp = 0;
    int* ptmpImg = new int[newHeight * newWidth];
    int** data = new int*[newHeight];
    for(int i = 0; i < newHeight; ++i)
        data[i] = ptmpImg + newWidth * i;
    for (int i = 0; i < newWidth; i++) {
        for (int j = 0; j < newHeight; j++) {
            for (int ii = 0; ii < kK-1; ++ii) {
                for (int jj = 0; jj < kK-1; ++jj) {
                    tmp = arrGrayPrevious[kK * i + ii][ kK * j + jj];
                }
            }
            data[i][j] = tmp;
            tmp = 0;
        }
    }
    QImage result((uchar*)ptmpImg, newWidth, newHeight,
                  QImage::Format_RGB32);
    QString s = QString::number(kK);
    result.save(g_outputFolder + "/resize" + s + ".png");
    //freeMemoryInt(ptmpImg, newWidth);
    delete[] data;
    return ptmpImg;
}

/*!
 * \brief getMemoryForPyramid − Для построения пирамиды, масштабированных изображений, нужно выделить память, чем эта функция и занимается
 * \param [in] image − исходное изображение
 * \param [in] arrGrayPrevious − указатель на массив яркостей первого кадра
 */
int** getMemoryForPyramid(imageInform* image, int** arrGrayPrevious)
{
    int** pToPyramid = new int*[LVL_PYRAMID];
    for (int i = 0; i < LVL_PYRAMID; ++i)
        pToPyramid[i] = new int[(image->height * image->width)/RESIZE];
    for (int i = 0; i < LVL_PYRAMID; ++i) {
        pToPyramid[i] = resizeImage(image, arrGrayPrevious, RESIZE);
    }
    return pToPyramid;
}

/*!
 * \brief saveVfResult − Сохраниение векторного поля в формате VF. Просмотр возможен в программе df−cl
 * \param [in] vf − Указатель на векторное поле.
 */
void saveVfResult(VF2d &vf)
{
    if (WriteVF(QString(g_outputFolder + "/" + "info" + ".vf").toLocal8Bit().data(), &vf) == 0) {
        qDebug() << "Correct write data";
    } else {
        qDebug() << "Error : can't write vf data";
    }
}
