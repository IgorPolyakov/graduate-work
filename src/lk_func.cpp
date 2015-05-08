#include "lk_func.h"
#include <QPainter>
#include <QImage>
#include <QDebug>
#include <QFileInfo>
#include <iostream>
#include <math.h>
#include <deprecated/dvfile.h>
#include <vector>
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
 * \param [in] prev − указатель на векторное поле содержащий предыдущий уровень пирамиды
 * \return [out] двумерный массив содержащий векторное поле, в формате VF
 */
VF2d* computeGrid(Data2Db* leftImg, Data2Db* rightImg, VF2d* prev)
{
    subSize* kernel = new subSize;
    kernel->rc = g_sizeWindowSeach;
    kernel->step = g_stepForGrid;
    int Vcx = (leftImg->cx()-(2*(kernel->rc+2))-1)/g_stepForGrid;
    int Vcy = (leftImg->cy()-(2*(kernel->rc+2))-1)/g_stepForGrid;
    VF2d *vf = new VF2d(Vcx, Vcy, g_stepForGrid, g_stepForGrid, g_sizeWindowSeach+1, g_sizeWindowSeach+1, DV_ALIGNMENT);
    Data2Db *state = new Data2Db("state",vf->cx(),vf->cy());
    vf->ad() = s_ptr<ProtoData2D>(state);
    if (prev)
    {
        for (int i = 0; i < vf->cy(); ++i) {
            for (int j = 0; j < vf->cx(); ++j) {
                int tmpX = (j *(prev->cx()-1))/(vf->cx()-1), tmpY = (i * (prev->cy()-1))/(vf->cy()-1);
                vf->lines()[i][j] = prev->lines()[tmpX][tmpY] * 2;
            }
        }
    }
    for (int i = 0; i < Vcy; ++i) {
        for (int j = 0; j < Vcx; ++j) {
            kernel->y_1 = (i * vf->grid().y) + vf->origin().y;
            kernel->x_1 = (j * vf->grid().x) + vf->origin().x;
            kernel->y_2 = (i * vf->grid().y) + vf->origin().y + vf->lines()[i][j][1];
            kernel->x_2 = (j * vf->grid().x) + vf->origin().x + vf->lines()[i][j][0];
            /*TODO:  Cделать проверку выхода за границы*/
            vf->lines()[i][j] += computeOptFlow(kernel, leftImg, rightImg);
            state->lines()[i][j] = 1;
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
    double tmpX = 0.0, tmpY = 0.0, tmpT = 0.0;
    Vec2d shiftVectr(0,0);
    int deltaX = 0;
    int deltaY = 0;
    if (g_isDebug) qDebug() << "SubSize:X" << kernel->x_1 << "Y:" << kernel->y_1 << "R:" << kernel->rc << "\n";
    for (int k = 0; k < g_iteration; ++k) {
        //Отсчитываем число итераций, для уточнения вектора
        for (int i = -kernel->rc; i <= kernel->rc; i++)
        {
            for (int j = -kernel->rc; j <= kernel->rc; j++)//(kernel->x_1 - kernel->rc); j < (kernel->x_1 + kernel->rc); j++)
            {
                if (((i - 1) > 0) && ((i + 1) < leftImg->cy()) && ((j - 1) > 0) && ((j + 1) < leftImg->cx()))
                {
                    tmpX = ((double)leftImg->lines()[kernel->y_1 + i][ kernel->x_1 + j - 1] - (double)leftImg->lines()[kernel->y_1 + i][kernel->x_1 + j + 1]) / 2.0;
                    tmpY = ((double)leftImg->lines()[kernel->y_1 + i - 1][kernel->x_1 + j] - (double)leftImg->lines()[kernel->y_1 + i + 1][kernel->x_1 + j]) / 2.0;
                    iX  += tmpX * tmpX;
                    iY  += tmpY * tmpY;
                    iXY += tmpX * tmpY;
                    //Если мы выходим за рамки изображения, то обнуляем такое уточнение
                    /*if ((i + deltaY) < 0 || (i + deltaY) > kernel->y_1 ||(j + deltaX) < 0 || (j + deltaX) > kernel->x_1) {
                        tmpT = ((double)leftImg->lines()[i][j] - (double)rightImg->lines()[i][j]) / 2.0;
                    } else {
                        tmpT = ((double)leftImg->lines()[i + deltaY][j + deltaX] - (double)rightImg->lines()[i + deltaY][j + deltaX]) / 2.0;
                    }*/
                    tmpT = ((double)leftImg->lines()[kernel->y_2 + i - 1][kernel->x_2 + j] - (double)rightImg->lines()[kernel->y_2 + i + 1][kernel->x_2 + j]) / 2.0;
                    iTX += tmpX * tmpT;
                    iTY += tmpY * tmpT;
                }
            }
        }
        Matx22d A(iX, iXY, iXY, iY);
        Vec2d b(-iTX,-iTY);
        inversion(A);
        shiftVectr = A*b;
        if (kernel->x_1 + shiftVectr[0] + kernel->rc > leftImg->cx()) {
            break;
        }
        if (kernel->x_1 + shiftVectr[0] - kernel->rc < 0) {
            break;
        }
        if (kernel->y_1 + shiftVectr[1] + kernel->rc > leftImg->cy()) {
            break;
        }
        if (kernel->y_1 + shiftVectr[1] - kernel->rc < 0) {
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
 * \param [in] kK − Коэффициент уменьшения изображения
 * \return [out] − указатель на массив масштабированных изображений
 */
Data2Db* resizeImage(Data2Db* image, int kK)
{
    int newWidth = (image->cx()/kK);
    int newHeight= (image->cy()/kK);
    int tmp = 0;
    Data2Db* smallImg = new Data2Db(newWidth, newHeight);
    for (int i = 0; i < newHeight; i++) {
        for (int j = 0; j < newWidth; j++) {
            for (int ii = 0; ii < kK-1; ++ii) {
                for (int jj = 0; jj < kK-1; ++jj) {
                    tmp = image->lines()[kK * i + ii][ kK * j + jj];
                }
            }
            smallImg->lines()[i][j] = tmp;
            tmp = 0;
        }
    }
    return smallImg;
}
/*!
 * \brief createPyramid_v2 − Выделение памяти для пирамиды изображений
 * \param [in] img − Указатель на оригинальное изображение
 * \param [in] lvl_pyramid − Уровень пирамиды
 * \return [out] Список изображений
 */
std::vector<Data2Db*>* createPyramid_v2(Data2Db* img, int lvl_pyramid)
{
    int r = 1;
    std::vector<Data2Db*> *listImg = new std::vector<Data2Db*>;
    listImg->push_back(img); //первый уровень(оригинал)
    for (int i = 0; i < lvl_pyramid; i++) {
        r = r + r;
        //r = 1 << i;
        listImg->push_back(resizeImage(img, r));
    }
    return listImg;
}
/*!
 * \brief saveVfResult − Сохранение векторного поля в формате VF. Просмотр возможен в программе df−cl
 * \param [in] vf − Указатель на векторное поле
 * \param [in] info - Имя сохраняемого файла
 */
void saveVfResult(VF2d &vf, QString info)
{
    if (WriteVF(QString(g_outputFolder + "/" + info + ".vf").toLocal8Bit().data(), &vf) == 0) {
        qDebug() << "Correct write data";
    } else {
        qDebug() << "Error : can't write vf data";
    }
}
