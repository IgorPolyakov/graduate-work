#include "lk_func.h"
#include <QPainter>
#include <QImage>
#include <QDebug>
#include <QFileInfo>
#include <iostream>

#define STEP 10

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

/*void drawVectorOnImage(QImage image)
{
    QPainter painter(&image);
    painter.drawLine(STEP,STEP+1,STEP,STEP+1);
    image.save("output/out.png");

}*/

void computeGrid(QImage image)
{
    QPainter paint(&image);
    unsigned int widthMax, heightMax;
    unsigned int widthStep, heightStep;
    unsigned int widthStart, heightStart;

    widthStart = image.width() / (STEP * 2);
    heightStart = image.width() / (STEP * 2);
    widthMax = image.width();
    heightMax = image.height();
    widthStep = image.width() / STEP;
    heightStep = image.height() / STEP;

    for (unsigned int i = widthStart ; i < widthMax; i = widthStep + i) {
        for (unsigned int j = heightStart; j < heightMax; j = heightStep + j) {
            paint.drawPoint(i, j);
        }
    }
    image.save("output/out.png");
}

double* computeOptFlow(SubSize* window, int** arrGrayPrevious, int** arrGrayNext)
{
    double iY = 0,   iX = 0,   iT = 0;
    if(g_isDebug) qDebug() << "SubSize:X(" << window->x_l << window->x_r << ")\n\t (" << window->y_r << "XX" << ")\n";
    for (int i = window->x_l; i < window->x_r; i++) {
        for (int j = window->y_l; j < window->y_r; j++) {
            if(g_isDebug) qDebug() << "X:" << iX << "Y:"<< iY <<"T:"<< iT <<"\n";
            iX += ((double)arrGrayPrevious[i - 1][j] - (double)arrGrayPrevious[i + 1][j]) / 2;
            //if(g_isDebug) qDebug() << "Al[" << i - 1 << "]["<<j<<"] = " << arrGrayPrevious[i-1][j] << "\t " << "Ar[" << i + 1 << "]["<<j<<"] = "<< arrGrayPrevious[i+1][j] << "\n";
            iY += ((double)arrGrayPrevious[i][j - 1] - (double)arrGrayPrevious[i][j + 1]) /2;
            //if(g_isDebug) qDebug() << "Al[" << i << "]["<< j - 1 <<"] = " << arrGrayPrevious[i][j-1] << "\t " << "Ar" << i << "]["<<j+1<<"] = "<< arrGrayPrevious[i][j+1] << "\n";
            iT += ((double)arrGrayPrevious[i][j] - (double)arrGrayNext[i][j]) / 2;
            //if(g_isDebug) qDebug() << "A[" << i<< "]["<<j<<"] = " << arrGrayPrevious[i][j] << " ";
        }
        //if(g_isDebug) qDebug() << "\n";
    }

    double **A = new double *[setSizeMatToInvers()];

    for (int i = 0; i < setSizeMatToInvers(); i++)
        A[i] = new double [setSizeMatToInvers()];

    A[0][0] = iX * iX;
    A[0][1] = iX * iY;
    A[1][0] = iX * iY;
    A[1][1] = iY * iY;

    int* b = new int [setSizeMatToInvers()];
    b[0] = iX * iT;
    b[1] = iY * iT;

    if(g_isDebug) qDebug() << "\nBEFORE\n" << "A =\t"<< A[0][0]<<  A[0][1] <<  "\n\t" << A[1][0] << A[1][1];
    inversion( A, setSizeMatToInvers());
    if(g_isDebug) qDebug() << "\nAFTER\n" << "A =\t"<< A[0][0]<<  A[0][1] <<  "\n\t" << A[1][0] << A[1][1];
    if(g_isDebug) qDebug() << "\n b =\t" << b[0] << "\n\t" << b[1];
    double* shiftVectr = multiplicMtrxAndVectr(A, b);

    //if(debug) qDebug() << shiftVectr[0] << shiftVectr[1];
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

int** genrateData(int w, int h) {
    int **E = new int *[w];

    for (int i = 0; i < w; i++)
        E[i] = new int [h];

    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            E[i][j] = i+j+10;
        }
    }

    return E;
};
