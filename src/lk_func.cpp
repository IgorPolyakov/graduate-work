#include "lk_func.h"
#include <QPainter>
#include <QImage>
#include <QDebug>

#define STEP 10

struct subSize {
    int x_l;
    int x_r;
    int y_l;
    int y_r;
};

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
            //std::cout << ary[i][j] << " ";
        };
        //std::cout << "\n";
    };
    return ary;
}

/*void drawVectorOnImage(QImage image)
{
    QPainter painter(&image);
    painter.drawLine(STEP,STEP+1,STEP,STEP+1);
    image.save("output/out.png");

}*/

void caclGrid(QImage image)
{
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
            QPainter paint(&image);
            paint.drawPoint(i, j);
            image.save("output/out.png");
        };
    };
    image.save("output/out.png");
}

void calcOptFlow(subSize window, int** arrayGray, int** arrayGrayNext)
{
    int iY = 0,   iX = 0,   iT = 0;
    for (int i = window.x_l; i < window.x_r; i++) {
        for (int j = window.y_l; j < window.y_r; j++) {
            iX += arrayGray[i - 1][j] - arrayGray[i + 1][j];
            iY += arrayGray[i][j - 1] - arrayGray[i][j + 1];
            iT += arrayGray[i][j] - arrayGrayNext[i][j];
            //std::cout << ary[i][j] << " ";
        };
        //std::cout << "\n";
    };
}

void freeArrBright(int** trash, int size)
{
    for (int i = 0; i < size; ++i)
        delete[] trash[i];
    delete [] trash;
}

void imageInfo(QImage image)
{
    qDebug() << "About image :: Size:" << image.size() << "\n"; //<< " Height:" << firstImg.height() << " W:" << firstImg.width() << "\n"
}
