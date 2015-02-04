#include "lk_func.h"
#include <QPainter>
#include <QImage>
#include <QDebug>
#include <iostream>

#define STEP 10

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

int* calcOptFlow(subSize window, int** arrayGray, int** arrayGrayNext)
{
    int iY = 0,   iX = 0,   iT = 0;
    qDebug() << " " << window.x_l << " " << window.x_r << " " << window.y_l << " " << window.y_r << " \n";
    for (int i = window.x_l; i < window.x_r; i++) {
        for (int j = window.y_l; j < window.y_r; j++) {
            iX += arrayGray[i - 1][j] - arrayGray[i + 1][j];
            iY += arrayGray[i][j - 1] - arrayGray[i][j + 1];
            iT += arrayGray[i][j] - arrayGrayNext[i][j];
            std::cout << arrayGray[i][j] << " ";
        }
        std::cout << "\n";
    }
    int A[2][2] = {{iX * iX, iX * iY}, {iX * iY, iY * iY}};
    int b[2] = {iX * iT, iY * iT };

    return shiftVectr;
}

void freeArrBright(int** trash, int size)
{
    for (int i = 0; i < size; ++i)
        delete[] trash[i];
    delete [] trash;
}

int* matrixVectorMultiplic(int** array, int* vector)
{
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            c[i] += (array[i][j] * vector[j]);
        }
    }
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
