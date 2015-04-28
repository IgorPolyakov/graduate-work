#include "lk_func.h"
#include <QPainter>
#include <QImage>
#include <QDebug>
#include <QFileInfo>
#include <iostream>
#include <math.h>
#define LVL_PYRAMID 3
#define RESIZE 5
#define SIZE_MAT_INV 2

/*!
 * \brief inversion - Нахождения обратной матрицы.
 * \param [in] **A − Указатель на массив
 * \param [in] N − Его размер
 */
void inversion(double **A, int N)
{
    double temp;
    double **E = new double *[N];
    for (int i = 0; i < N; i++)
        E[i] = new double [N];
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            E[i][j] = 0.0;
            if (i == j)
                E[i][j] = 1.0;
        }
    for (int k = 0; k < N; k++) {
        temp = A[k][k];
        for (int j = 0; j < N; j++) {
            A[k][j] /= temp;
            E[k][j] /= temp;
        }
        for (int i = k + 1; i < N; i++) {
            temp = A[i][k];
            for (int j = 0; j < N; j++) {
                A[i][j] -= A[k][j] * temp;
                E[i][j] -= E[k][j] * temp;
            }
        }
    }
    for (int k = N - 1; k > 0; k--) {
        for (int i = k - 1; i >= 0; i--) {
            temp = A[i][k];
            for (int j = 0; j < N; j++) {
                A[i][j] -= A[k][j] * temp;
                E[i][j] -= E[k][j] * temp;
            }
        }
    }
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            A[i][j] = E[i][j];
    for (int i = 0; i < N; i++)
        delete [] E[i];
    delete [] E;
}

/*!
 * \brief computeGrid - Строит сетку по верх изображения, в точках
 * пересечения ищется вектор оптического потока
 * \param [in] leftImg - указатель на массив яркостей первого кадра
 * \param [in] rightImg - указатель на массив яркостей второго кадра
 * \return двумерный массив содержащий векторное поле, в формате VF
 */
VF2d* computeGrid(Data2Db* leftImg, Data2Db* rightImg)
{
    subSize* kernel = new subSize;
    /*double* vf= new double[SIZE_MAT_INV];
    for (int i = 0; i < SIZE_MAT_INV; ++i) {
        vf[i] = 1.0;
    }*/
    kernel->rc = g_sizeWindowSeach;
    kernel->step = g_stepForGrid;
    int Vcx = (leftImg->cx()-(2*kernel->rc)-1)/g_stepForGrid;
    int Vcy = (leftImg->cy()-(2*kernel->rc)-1)/g_stepForGrid;
    static VF2d *vf = new VF2d(Vcx, Vcy, 1,1,0,0, DV_ALIGNMENT);
    /*
     *Data2D_(int cx, int cy, int gx=1, int gy=1, int ox=0, int oy=0, int align = DV_ALIGNMENT)
    : ProtoData2D(DataType<T_>::depth, DataType<T_>::fmt, DataType<T_>::channels, 0, "", cx, cy, gx, gy, ox, oy, align) {}
    */
    for (int i = kernel->rc+1; i < (leftImg->cx()-kernel->rc); i += Vcy) {
        kernel->cx = i;
        for (int j = kernel->rc+1; (j < (leftImg->cy()-kernel->rc));
             j += Vcx) {
            kernel->cy = j;
            //vf = (VF2d)computeOptFlow(kernel, leftImg, rightImg);
        }
    }
    delete kernel;
    return vf;
}

/*!
 * \brief computeOptFlow - Вычисление вектора оптического потока
 * \param [in] kernel - структура содержащая сведения о местонахождении пикселя, размерах окна поиска и прочего
 * \param [in] leftImg - указатель на массив яркостей первого кадра
 * \param [in] rightImg - указатель на массив яркостей второго кадра
 * \return вектор оптического потока
 */
double* computeOptFlow(subSize* kernel, Data2Db* leftImg, Data2Db* rightImg)
{
    double iY = 0.0,   iX = 0.0,   iTX = 0.0, iTY = 0.0, iXY = 0.0;
    double tmpX, tmpY, tmpT;
    double* shiftVectr = new double[2];
    for (int i = 0; i < 2; ++i) {
        shiftVectr[i] = 0.0;
    }
    int deltaX = 0;
    int deltaY = 0;
    double **A = new double *[SIZE_MAT_INV];
    for (int i = 0; i < SIZE_MAT_INV; i++)
        A[i] = new double [SIZE_MAT_INV];
    int* b = new int [SIZE_MAT_INV];

    if (g_isDebug) qDebug() << "SubSize:X" << kernel->cx << "Y:" << kernel->cy << "R:" << kernel->rc << "\n";
    for (int k = 0; k <= g_iteration; ++k) {
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
        A[0][0] = iX;
        A[0][1] = iXY;
        A[1][0] = iXY;
        A[1][1] = iY;
        b[0] = -iTX;
        b[1] = -iTY;
        inversion(A, SIZE_MAT_INV);
        shiftVectr = multiplicMtrxAndVectr(A, b);

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

    for (int i = 0; i < SIZE_MAT_INV; ++i)
        delete[] A[i];
    delete [] A;

    delete[] b;
    return shiftVectr;
}

/*!
 * \brief multiplicMtrxAndVectr - Произведение матрицы на вектор
 * \param [in] **array − указатель на массив
 * \param [in] *vector − указатель на вектор
 * \return  *tmp − результат произведения
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
 * \brief getImageInfo - Получение информации о входном изображении
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
 * \brief joinImage - Объединение трёх изображений(первого, второго и первого с нанесённым поверх векторным полем)
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
 * \brief resizeImage - Функция масштабирования изображения, для построения пирамиды уменьшенных изображений
 * \param [in] image - структура содержащая сведения о размерах масштабируемого изображения
 * \param [in] arrGrayPrevious − Указатель на массив яркостей первого кадра
 * \param [in] kK − Коэффициент уменьшения изображения
 * \return [out] - указатель на массив масштабированных изображений
 */
int* resizeImage(imageInform* image, int** arrGrayPrevious, int kK)
{
    /*if((image.width()%2 == 0)||(image.height()%2 == 0))*/
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
 * \brief getMemoryForPyramid - Для построения пирамиды, масштабированных изображений, нужно выделить память, чем эта функция и занимается
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
 * \brief saveVfResult
 * \param vf
 */
void saveVfResult(VF2d vf/*, Data2Db img*/)
{
    /*if (WriteVF(g_outputFolder + "/" + "info" + ".png", VF2d* vf) == 0) {
        qDebug() << "Correct write data";
    } else {
        qDebug() << "Error : can't write vf data";
    }*/
}
