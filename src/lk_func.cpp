#include "lk_func.h"
#include <QPainter>
#include <QImage>
#include <QDebug>
#include <iostream>

#define STEP 10
#define SIZE_MAT_TO_INV 2

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

void calcGrid(QImage image)
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

double* calcOptFlow(subSize window, int** arrayGray, int** arrayGrayNext)
{
    int iY = 0,   iX = 0,   iT = 0;
    //qDebug() << " " << window.x_l << " " << window.x_r << " " << window.y_l << " " << window.y_r << " \n";
    for (int i = window.x_l; i < window.x_r; i++) {
        for (int j = window.y_l; j < window.y_r; j++) {
            qDebug() << iX << iY << iT;
            iX += arrayGray[i - 1][j] - arrayGray[i + 1][j];
            iY += arrayGray[i][j - 1] - arrayGray[i][j + 1];
            iT += arrayGray[i][j] - arrayGrayNext[i][j];
            std::cout << arrayGray[i][j] << " ";
        }
        std::cout << "\n";
    }

    double **A = new double *[SIZE_MAT_TO_INV];

    for (int i = 0; i < SIZE_MAT_TO_INV; i++)
        A[i] = new double [SIZE_MAT_TO_INV];

    A[0][0] = iX * iX;
    A[0][1] = iX * iY;
    A[1][0] = iX * iY;
    A[1][1] = iY * iY;

    int* b = new int [SIZE_MAT_TO_INV];
    b[0] = iX * iT;
    b[1] = iY * iT;

    qDebug() << "\n BEFORE " << A[0][0]<<  A[0][1] <<  A[1][0] <<    A[1][1] << "\n";
    inversion( A, SIZE_MAT_TO_INV);
    qDebug() << "\n AFTER " << A[0][0]<<  A[0][1] <<  A[1][0] <<    A[1][1] << "\n";
    double* shiftVectr = matrixVectorMultiplic(A, b);

    qDebug() << shiftVectr[0] << shiftVectr[1];
    freeMemoryFloat(A, SIZE_MAT_TO_INV);
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

double* matrixVectorMultiplic(double** array, int* vector)
{
    double* c = new double[2];
    c[0] = 0;
    c[1] = 0;
    qDebug() << vector[0] << " " << vector[1]<< " \n";
    for (int i = 0; i < SIZE_MAT_TO_INV; i++) {
        for (int j = 0; j < SIZE_MAT_TO_INV; j++) {
            c[i] += array[i][j] * (double)vector[j];
            qDebug() << c[i];
        }
    }
    //qDebug() << c[0] << " " << c[1]<< " \n";
    return c;
}

void imageInfo(QImage image, int** arr)
{
    qDebug() << "About image :: Size:" << image.size() << "\n"; //<< " Height:" << firstImg.height() << " W:" << firstImg.width() << "\n"
    for (int i = 0; i < image.width(); i++) {
        for (int j = 0; j < image.height(); j++) {
            std::cout << arr[i][j] << " ";
        }
        std::cout << "||\n";
    }
}
