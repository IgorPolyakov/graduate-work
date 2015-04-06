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
    double* vectorOptFlw = new double[2];

    QPainter painter(&image);
    painter.setPen(QPen(Qt::red));
    for (int i = g_stepForGrid ; i < image.width() - g_stepForGrid; i = g_stepForGrid + i) {
        for (int j = g_stepForGrid; j < image.height() - g_stepForGrid; j = g_stepForGrid + j) {
            initialWindow->xCore = i;
            initialWindow->yCore = j;
            vectorOptFlw = computeOptFlow(initialWindow, arrGrayPrevious, arrGrayNext);
            painter.drawLine(initialWindow->xCore, initialWindow->yCore, initialWindow->xCore + vectorOptFlw[0], initialWindow->yCore + vectorOptFlw[1]);
        }
    }
    delete [] vectorOptFlw;
    delete initialWindow->radiusCode;
    delete initialWindow->xCore;
    delete initialWindow->yCore;
    delete initialWindow->xMax;
    delete initialWindow->yMax;
    delete InitialWindow;
    return image;
}

double* computeOptFlow(SubSize* initialWindow, int** arrGrayPrevious, int** arrGrayNext)
{
    double iY = 0,   iX = 0,   iTX = 0, iTY = 0, iXY = 0;
    double tmpX, tmpY, tmpT;
    double* shiftVectr = new double[2];
    for (int var = 0; var < 2; ++var) {
        shiftVectr = 0;
    }
    int deltaX = 0;
    int deltaY = 0;

    double **A = new double *[setSizeMatToInvers()];

    for (int i = 0; i < setSizeMatToInvers(); i++)
        A[i] = new double [setSizeMatToInvers()];

    int* b = new int [setSizeMatToInvers()];

    if (g_isDebug) qDebug() << "SubSize:X" << initialWindow->xCore << "Y:" << initialWindow->yCore << "R:" << initialWindow->radiusCode << "\n";
    for (int k = 0; k <= g_iteration; ++k) {//Отсчитываем число итераций, для уточнения вектора
        for (int i = (initialWindow->xCore - initialWindow->radiusCode); i <= (initialWindow->xCore + initialWindow->radiusCode); i++) {
            for (int j = (initialWindow->yCore - initialWindow->radiusCode); j <= (initialWindow->yCore + initialWindow->radiusCode); j++) {
                tmpX = ((double)arrGrayPrevious[i - 1][j] - (double)arrGrayPrevious[i + 1][j]) / 2;
                tmpY = ((double)arrGrayPrevious[i][j - 1] - (double)arrGrayPrevious[i][j + 1]) / 2;
                iX  += tmpX * tmpX;
                iY  += tmpY * tmpY;
                iXY += tmpX * tmpY;

                tmpT = ((double)arrGrayPrevious[i + deltaX][j + deltaY] - (double)arrGrayNext[i + deltaX][j + deltaY]) / 2;
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

        if (initialWindow->xCore + shiftVectr[0] + initialWindow->radiusCode > initialWindow->xMax) {
            break;
        }
        if (initialWindow->xCore + shiftVectr[0] - initialWindow->radiusCode < 0) {
            break;
        }
        if (initialWindow->yCore + shiftVectr[1] + initialWindow->radiusCode > initialWindow->yMax) {
            break;
        }
        if (initialWindow->yCore + shiftVectr[1] - initialWindow->radiusCode < 0) {
            break;
        }
        if ((shiftVectr[0] == shiftVectr[0]) || (shiftVectr[1] == shiftVectr[1])) { //NaN Checking
            deltaX = (int)floor(shiftVectr[0]);
            deltaY = (int)floor(shiftVectr[1]);/*
            deltaX = (int)floor(shiftVectr[0]);
            deltaY = (int)floor(shiftVectr[1]);*/
        } else {
            qDebug() << "NaN Error";
            shiftVectr[0] = 0;
            shiftVectr[1] = 0;
        }
    }
    if (g_isDebug) qDebug() << shiftVectr[0] << shiftVectr[1] << "return";
    freeMemoryFloat(A, setSizeMatToInvers());
    return shiftVectr;
    //delete [] shiftVectr;
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
    for (int cnt = 0; cnt < setSizeMatToInvers(); ++cnt) {
        tmp[cnt] = 0;
    }
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

void joinImage(QImage img1, QImage img2, QImage img3, QString info)
{
    QImage result(img1.width() + img2.width() + img3.width(), img1.height(), QImage::Format_ARGB32);
    QPainter paint;
    paint.begin(&result);
    paint.drawImage(0, 0, img1);
    paint.drawImage(img1.width(), 0, img2);
    paint.drawImage(img1.width() + img2.width(), 0, img3);
    result.save(g_outputFolder + info + ".png");
}

int* resizeImage(QImage image, int** arrGrayPrevious, int kK)
{
    /*if((image.width()%2 == 0)||(image.height()%2 == 0))*/
    int newWidth = (image.width()/kK);
    int newHeight= (image.height()/kK);
    int tmp = 0;

    int* ptmpImg = new int[newHeight * newWidth];
    int** data = new int*[newHeight];

    for(int i = 0; i < newHeight; ++i)
        data[i] = ptmpImg + newWidth * i;

    for (int i = 0; i < newWidth;i++) {
        for (int j = 0; j < newHeight;j++) {
            for (int ii = 0; ii < kK-1; ++ii) {
                for (int jj = 0; jj < kK-1; ++jj) {
                    tmp = arrGrayPrevious[kK * i + ii][ kK * j + jj];
                }
            }
            data[i][j] = tmp;
            tmp = 0;
        }
    }

    QImage result((uchar*)ptmpImg, newWidth, newHeight, QImage::Format_RGB32);
    QString s = QString::number(kK);
    result.save(g_outputFolder + "resize" + s + ".png");
    //freeMemoryInt(ptmpImg, newWidth);
    return ptmpImg;
}

void getMemoryForPyramid(QImage image, int** arrGrayPrevious, pointerToLvlPyramid pointToPyramid)
{
    pointToPyramid.l1 = resizeImage(image, arrGrayPrevious, 2);
    pointToPyramid.l2 = resizeImage(image, arrGrayPrevious, 4);
    pointToPyramid.l3 = resizeImage(image, arrGrayPrevious, 8);
}

