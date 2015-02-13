#include "lk_func.h"
#include <QPainter>
#include <QImage>
#include <QDebug>
#include <QFileInfo>
#include <iostream>

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

void computeGrid(QImage image, int** arrGrayPrevious, int** arrGrayNext)
{
    SubSize* ARA = new SubSize;
    ARA->radiusCode = g_sizeWindowSeach;
    double* shiftVector = new double[2];

    QPainter painter(&image);
    painter.setPen(QPen(Qt::red));
    for (int i = g_stepForGrid ; i < image.width() - g_stepForGrid; i = g_stepForGrid + i) {
        for (int j = g_stepForGrid; j < image.height() - g_stepForGrid; j = g_stepForGrid + j) {
            ARA->xCore = i;
            ARA->yCore = j;
            for (int k = 0; k <= g_iteration; ++k) {
                shiftVector = computeOptFlow(ARA, arrGrayPrevious, arrGrayNext);
                ARA->xCore = i + shiftVector[0];
                ARA->yCore = j + shiftVector[1];
                if (g_isDebug) qDebug() << "SubS89ize:X" << ARA->xCore << "Y:" << ARA->yCore << "R:" << ARA->radiusCode << "\n";
            }
            if ((shiftVector[0] == shiftVector[0]) || (shiftVector[1] == shiftVector[1])) //NaN Checking
                painter.drawLine(ARA->xCore, ARA->yCore, ARA->xCore + shiftVector[1], ARA->yCore + shiftVector[0]);
            else
                painter.drawPoint(ARA->xCore, ARA->yCore);
        }
    }
    image.save("input/img1100.png");
    delete [] shiftVector;
    delete [] ARA;
}

double* computeOptFlow(SubSize* window, int** arrGrayPrevious, int** arrGrayNext)
{
    double iY = 0,   iX = 0,   iTX = 0, iTY = 0, iXY = 0;
    if (g_isDebug) qDebug() << "SubSize:X" << window->xCore << "Y:" << window->yCore << "R:" << window->radiusCode << "\n";

    for (int i = (window->xCore - window->radiusCode); i < (window->xCore + window->radiusCode); i++) {
        for (int j = (window->yCore - window->radiusCode); j < (window->yCore + window->radiusCode); j++) {
            //if (g_isDebug) qDebug() << "X:" << iX << "Y:" << iY << "T:" << iTX << "\n";
            double tmpX, tmpY, tmpT;
            tmpX = ((double)arrGrayPrevious[i - 1][j] - (double)arrGrayPrevious[i + 1][j]) / 2;
            iX += tmpX * tmpX;
            //if(g_isDebug) qDebug() << "Al[" << i - 1 << "]["<<j<<"] = " << arrGrayPrevious[i-1][j] << "\t " << "Ar[" << i + 1 << "]["<<j<<"] = "<< arrGrayPrevious[i+1][j] << "\n";
            tmpY = ((double)arrGrayPrevious[i][j - 1] - (double)arrGrayPrevious[i][j + 1]) / 2;
            iY += tmpY * tmpY;
            iXY += tmpX * tmpY;
            //if(g_isDebug) qDebug() << "Al[" << i << "]["<< j - 1 <<"] = " << arrGrayPrevious[i][j-1] << "\t " << "Ar" << i << "]["<<j+1<<"] = "<< arrGrayPrevious[i][j+1] << "\n";
            tmpT = ((double)arrGrayPrevious[i][j] - (double)arrGrayNext[i][j]) / 2;
            iTX += tmpX * tmpT;
            iTY += tmpY * tmpT;
            //if(g_isDebug) qDebug() << "A[" << i<< "]["<<j<<"] = " << arrGrayPrevious[i][j] << " ";
        }
        //if(g_isDebug) qDebug() << "\n";
    }

    double **A = new double *[setSizeMatToInvers()];

    for (int i = 0; i < setSizeMatToInvers(); i++)
        A[i] = new double [setSizeMatToInvers()];

    A[0][0] = iX;
    A[0][1] = iXY;
    A[1][0] = iXY;
    A[1][1] = iY;

    int* b = new int [setSizeMatToInvers()];

    b[0] = -iTX;
    b[1] = -iTY;

    inversion(A, setSizeMatToInvers());
    double* shiftVectr = multiplicMtrxAndVectr(A, b);
    freeMemoryFloat(A, setSizeMatToInvers());
    return shiftVectr;
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
};
