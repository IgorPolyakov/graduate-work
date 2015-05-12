#include "lk_func.h"
#include <QPainter>
#include <QImage>
#include <QDebug>
#include <QFileInfo>
#include <QFile>
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
 * \brief bilinearInterpolation
 * \param x   - Смещение по х
 * \param y   - Смещение по у
 * \param q11 - Значение функции в левом верхнем углу
 * \param q12 - Значение функции в левом нижнем углу
 * \param q21 - Значение функции в правом верхнем углу
 * \param q22 - Значение функции в правом нижнем углу
 * \param x1  - Координата верхнего левого угла по х
 * \param y1  - Координата верхнего левого угла по у
 * \return
 */
double bilinearInterpolation(double delx, double dely, uchar q11, uchar q12, uchar q21, uchar q22, int x1, int y1)
{
    int x2 = x1 + 1;
    int y2 = y1 + 1;
    double x = x1 + delx;
    double y = y1 + dely;

    double r1 = (((x2 - x)/(x2 - x1)) * q11) + (((x - x1)/(x2 - x1)) * q21);
    double r2 = (((x2 - x)/(x2 - x1)) * q12) + (((x - x1)/(x2 - x1)) * q22);
    double ri = (((y2 - y)/(y2 - y1)) *  r1) + (((y - y1)/(y2 - y1)) *  r2);
    return ri;
}

int calcLvlPyramid(int cx, int cy)
{
    int lvl_pyramid = 0;
    if (g_isPyramid)
    {
        if (cx > cy) {
            int tmp = cy;
            while ((tmp-(2*(g_sizeWindowSeach + 2))-1)/g_stepForGrid >= 1)
            {
                lvl_pyramid++;
                tmp = tmp/2;
            }
        }
        else {
            int tmp = cx;
            while ((tmp-(2*(g_sizeWindowSeach + 2))-1)/g_stepForGrid >= 1)
            {
                lvl_pyramid++;
                tmp = tmp/2;
            }
        }
        lvl_pyramid--;
    }
    else
        lvl_pyramid = 0;
    return lvl_pyramid;
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
    QFile writeDelta(g_outputFolder + "//delta.txt");
    QFile writeVector(g_outputFolder + "//vector.txt");
    writeDelta.open(QIODevice::WriteOnly);
    writeVector.open(QIODevice::WriteOnly);
    QTextStream wD(&writeDelta);
    QTextStream wV(&writeVector);

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
        for (int i = 0; i < vf->cy(); ++i)
        {
            for (int j = 0; j < vf->cx(); ++j)
            {
                int tmpX = (j *(prev->cx()-1))/(vf->cx()-1), tmpY = (i * (prev->cy()-1))/(vf->cy()-1);
                vf->lines()[i][j] = prev->lines()[tmpY][tmpX] * 2.0;
            }
        }
    }
    for (int i = 0; i < Vcy; ++i)
    {
        for (int j = 0; j < Vcx; ++j)
        {
            kernel->y_1 = (i * vf->grid().y) + vf->origin().y;
            kernel->x_1 = (j * vf->grid().x) + vf->origin().x;
            kernel->y_2 = (i * vf->grid().y) + vf->origin().y + vf->lines()[i][j][1];
            kernel->x_2 = (j * vf->grid().x) + vf->origin().x + vf->lines()[i][j][0];
            /*TODO:  Cделать проверку выхода за границы*/
            /*vf->lines()[i][j] += */computeOptFlow(kernel, leftImg, rightImg, vf->lines()[i][j]);//, wD, wV);
            state->lines()[i][j] = 1;
        }
    }
    writeDelta.close();
    writeVector.close();
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
Vec2d computeOptFlow(subSize* kernel, Data2Db* leftImg, Data2Db* rightImg, Vec2d& dv)//, QTextStream &wD, QTextStream &wV)
{
    double iY = 0.0,   iX = 0.0,   iTX = 0.0, iTY = 0.0, iXY = 0.0;
    double tmpX = 0.0, tmpY = 0.0, tmpT = 0.0;
    Vec2d delta(0,0);
    double thdelta = 0.001;
    if (g_isDebug) qDebug() << "SubSize:X" << kernel->x_1 << "Y:" << kernel->y_1 << "R:" << kernel->rc << "\n";
    for (int k = 0; k < g_iteration; ++k) //Отсчитываем число итераций, для уточнения вектора
    {
        double x2 = kernel->x_1 + dv[0];
        double y2 = kernel->y_1 + dv[1];
        kernel->x_2 = x2;
        kernel->y_2 = y2;
        x2 -= kernel->x_2;
        y2 -= kernel->y_2;

        /*!
         * B-spline interpolation
         *  or
         * Bilinear interpolation
         */
        Data2Dd rightBlock(kernel->rc*2+1, kernel->rc*2+1);
        double RX[4] = {0};
        double RY[4] = {0};
        bicubic_bspline_coefs(RX, RY, x2, y2);
        for (int ii = 0; ii < kernel->rc*2+1; ++ii)
        {
            for (int jj = 0; jj < kernel->rc*2+1; ++jj)
            {
                int xx = jj+kernel->x_2-kernel->rc;
                int yy = ii+kernel->y_2-kernel->rc;

                if (xx < 0) xx = 0;
                if (xx > rightImg->cx()-2) xx = rightImg->cx()-2;
                if (yy < 0) yy = 0;
                if (yy > rightImg->cy()-2) yy = rightImg->cy()-2;

                if (x2 < 0.0001 && y2 < 0.0001)
                    rightBlock.lines()[ii][jj] = rightImg->lines()[yy][xx];
                else
                    switch (g_interpolation) {
                    case 0:
                        rightBlock.lines()[ii][jj] = BicubicBspline2d<uchar>(rightImg->lines(), rightImg->cx(), rightImg->cy(), xx, yy, x2, y2, RX, RY);
                        break;
                    case 1:
                        rightBlock.lines()[ii][jj] = (uchar)bilinearInterpolation(x2, y2, rightImg->lines()[yy][xx], rightImg->lines()[yy][xx+1], rightImg->lines()[yy+1][xx], rightImg->lines()[yy+1][xx+1], xx, yy);
                        break;
                    default:
                        rightBlock.lines()[ii][jj] = BicubicBspline2d<uchar>(rightImg->lines(), rightImg->cx(), rightImg->cy(), xx, yy, x2, y2, RX, RY);
                        break;
                    }
                    /* TODO: Добавить биленейную интерполяцию и интерполяцию на основе разложения в ряд Тейлора*/
            }
        }

        for (int i = -kernel->rc; i <= kernel->rc; i++)
        {
            for (int j = -kernel->rc; j <= kernel->rc; j++)
            {
                int xx = kernel->x_1 + j;
                int yy = kernel->y_1 + i;
                if (xx<1) xx = 1;
                if (xx+1>leftImg->cx()-1) xx = leftImg->cx()-2;
                if (yy<1) yy = 1;
                if (yy+1>leftImg->cy()-1) yy = leftImg->cy()-2;

                tmpX = ((double)leftImg->lines()[yy][xx - 1] - (double)leftImg->lines()[yy][xx + 1]) / 2.0;
                tmpY = ((double)leftImg->lines()[yy - 1][xx] - (double)leftImg->lines()[yy + 1][xx]) / 2.0;
                iX  += tmpX * tmpX;
                iY  += tmpY * tmpY;
                iXY += tmpX * tmpY;

                tmpT = ((double)leftImg->lines()[yy][xx] - (double)rightBlock.lines()[kernel->rc + i][kernel->rc + j]);
                iTX += tmpX * tmpT;
                iTY += tmpY * tmpT;
            }
        }
        Matx22d A(iX, iXY, iXY, iY);
        Vec2d b(-iTX,-iTY);
        inversion(A);
        delta = A*b;
        dv += delta;

        //wD << delta[0] << "\t" << delta[1] << "\t" << k << "\n";
        //wV << dv[0] << "\t" << dv[1] << "\t" << k << "\n";

        if (delta[0]*delta[0] < thdelta && delta[1]*delta[1] < thdelta) break;
        //if (delta[0]*delta[0] + delta[1]*delta[1] < thdelta) break;
    }
    if (g_isDebug) qDebug() << dv[0] << dv[1] << "return";
    return dv;
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
