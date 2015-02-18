#include "lk_func.h"
#include <QPainter>
#include <QImage>
#include <QDebug>
#include <QFileInfo>
#include <iostream>
#include <math.h>

int setSizeMatToInvers()
{
    return 2;
}

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

int **getArrBright(QImage image)
{
    int gray;
    //get some space for array of bright
    int** ary = new int*[image.width()];
    for (int i = 0; i < image.width(); ++i)
        ary[i] = new int[image.height()];
    //convert color image to grayscale
    for (int i = 0; i < image.width(); i++) {
        for (int j = 0; j < image.height(); j++) {
            gray = qGray(image.pixel(i, j));
            ary[i][j] = gray;
        }
    }
    return ary;
}

QImage computeGrid(QImage image, int** arrGrayPrevious, int** arrGrayNext)
{
    SubSize* initialWindow = new SubSize;
    initialWindow->radiusCode = g_sizeWindowSeach;
    initialWindow->xMax = image.width();
    initialWindow->yMax = image.height();
    double* shiftVector = new double[2];

    QPainter painter(&image);
    painter.setPen(QPen(Qt::red));
    for (int i = g_stepForGrid ; i < image.width() - g_stepForGrid; i = g_stepForGrid + i) {
        for (int j = g_stepForGrid; j < image.height() - g_stepForGrid; j = g_stepForGrid + j) {
            initialWindow->xCore = i;
            initialWindow->yCore = j;
            shiftVector = computeOptFlow(initialWindow, arrGrayPrevious, arrGrayNext);
            painter.drawLine(initialWindow->xCore, initialWindow->yCore, initialWindow->xCore + shiftVector[0], initialWindow->yCore + shiftVector[1]);
        }
    }
    delete [] shiftVector;
    delete [] initialWindow;
    return image;
}

double* computeOptFlow(SubSize* initialWindow, int** arrGrayPrevious, int** arrGrayNext)
{
    double iY = 0,   iX = 0,   iTX = 0, iTY = 0, iXY = 0;
    double tmpX, tmpY, tmpT;
    double* shiftVectr = new double[2];
    SubSize* modifyWindow = new SubSize;

    modifyWindow->xCore = initialWindow->xCore;
    modifyWindow->yCore = initialWindow->yCore;
    modifyWindow->radiusCode = initialWindow->radiusCode;
    modifyWindow->xMax = initialWindow->xMax;
    modifyWindow->yMax = initialWindow->yMax;
    double **A = new double *[setSizeMatToInvers()];

    for (int i = 0; i < setSizeMatToInvers(); i++)
        A[i] = new double [setSizeMatToInvers()];

    int* b = new int [setSizeMatToInvers()];

    if (g_isDebug) qDebug() << "SubSize:X" << initialWindow->xCore << "Y:" << initialWindow->yCore << "R:" << initialWindow->radiusCode << "\n";
    for (int k = 0; k < g_iteration; ++k) {//Отсчитываем число итераций, для уточнения вектора
        for (int i = (initialWindow->xCore - initialWindow->radiusCode); i < (initialWindow->xCore + initialWindow->radiusCode); i++) {
            for (int j = (initialWindow->yCore - initialWindow->radiusCode); j < (initialWindow->yCore + initialWindow->radiusCode); j++) {
                //if (g_isDebug) qDebug() << "X:" << iX << "Y:" << iY << "T:" << iTX << "\n";
                tmpX = ((double)arrGrayPrevious[i - 1][j] - (double)arrGrayPrevious[i + 1][j]) / 2;
                //if(g_isDebug) qDebug() << "Al[" << i - 1 << "]["<<j<<"] = " << arrGrayPrevious[i-1][j] << "\t " << "Ar[" << i + 1 << "]["<<j<<"] = "<< arrGrayPrevious[i+1][j] << "\n";
                tmpY = ((double)arrGrayPrevious[i][j - 1] - (double)arrGrayPrevious[i][j + 1]) / 2;
                iX  += tmpX * tmpX;
                iY  += tmpY * tmpY;
                iXY += tmpX * tmpY;
            }
        }
        for (int i = (modifyWindow->xCore - modifyWindow->radiusCode); i < (modifyWindow->xCore + modifyWindow->radiusCode); i++) {
            for (int j = (modifyWindow->yCore - modifyWindow->radiusCode); j < (modifyWindow->yCore + modifyWindow->radiusCode); j++) {
                tmpX = ((double)arrGrayPrevious[i - 1][j] - (double)arrGrayPrevious[i + 1][j]) / 2;
                tmpY = ((double)arrGrayPrevious[i][j - 1] - (double)arrGrayPrevious[i][j + 1]) / 2;
                tmpT = ((double)arrGrayPrevious[i][j] - (double)arrGrayNext[i][j]) / 2;
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

        inversion(A, setSizeMatToInvers());
        shiftVectr = multiplicMtrxAndVectr(A, b);

        if ((shiftVectr[0] == shiftVectr[0]) || (shiftVectr[1] == shiftVectr[1])) { //NaN Checking
            modifyWindow->xCore += floor(shiftVectr[0]);
            modifyWindow->yCore += floor(shiftVectr[1]);
        } else
            qDebug() << "NaN Error";
        if (modifyWindow->xCore > modifyWindow->xMax) {
            modifyWindow->xCore = modifyWindow->xMax;
        }
        if (modifyWindow->yCore > modifyWindow->yMax) {
            modifyWindow->yCore = modifyWindow->yMax;
        }
        if (g_isDebug) qDebug() << shiftVectr[0] << shiftVectr[1] << "temp--";
    }
    if (g_isDebug) qDebug() << shiftVectr[0] << shiftVectr[1] << "return";
    freeMemoryFloat(A, setSizeMatToInvers());
    return shiftVectr;
    //delete shiftVectr;
}
void freeMemoryInt(int** trash, int size)
{
    for (int i = 0; i < size; ++i)
        delete[] trash[i];
    delete [] trash;
}

void freeMemoryFloat(double** trash, int size)
{
    for (int i = 0; i < size; ++i)
        delete[] trash[i];
    delete [] trash;
}

double* multiplicMtrxAndVectr(double** array, int* vector)
{
    double* tmp = new double[setSizeMatToInvers()];
    tmp[0] = 0;
    tmp[1] = 0;//вот тут вот не правильно(( но да ладно)
    for (int i = 0; i < setSizeMatToInvers(); i++) {
        for (int j = 0; j < setSizeMatToInvers(); j++) {
            tmp[i] += array[i][j] * (double)vector[j];
        }
    }
    return tmp;
}

void getImageInfo(QImage image, QString path)
{
    QFileInfo fileImage(path);
    qDebug() << "About image " << fileImage.fileName() << " :: FileSize:" << fileImage.size() << "bytes" << image.size();
}

int** genrateData(int w, int h)
{
    int **E = new int *[w];

    for (int i = 0; i < w; i++)
        E[i] = new int [h];

    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            E[i][j] = i + j + 10;
        }
    }
    return E;
}

void joinImage(QImage img1, QImage img2, QImage img3, QString info)
{
    QImage result(img1.width() + img2.width() + img3.width(), img1.height(), QImage::Format_ARGB32);
    QPainter paint;
    paint.begin(&result);
    paint.drawImage(0, 0, img1);
    paint.drawImage(img1.width(), 0, img2);
    paint.drawImage(img1.width() + img2.width(), 0, img3);
    result.save("output/" + info + ".png");
}
