#include <QColor>
#include <QDebug>
#include <QDir>
#include <QImage>
#include <qmath.h>
#include <QPainter>
#include <QPen>
#include <QPixmap>
#include <QtCore>
#include <QtGui>
#include <QTime>
#include <iostream>
#include <getopt.h>

int **getArrBright(QImage image)
{
    int gray;
    //get some space for array of bright
    int** ary = new int*[image.width()];
    for(int i = 0; i < image.width(); ++i)
        ary[i] = new int[image.height()];
    //convert color image to grayscale
    for(int i = 0; i < image.width(); i++)
    {
        for(int j = 0; j < image.height(); j++)
        {
            gray = qGray(image.pixel(i, j));
            ary[i][j] = gray;
            //std::cout << ary[i][j] << " ";
        };
        //std::cout << "\n";
    };
    return ary;
}

void drawVectorOnImage()
{
    /*QPixmap pixmap;
    pixmap.load(QString::fromUtf8("frame00.png"));
    QPainter painter(&pixmap);
    QPen Red(Qt::red);
    painter.setPen(Red);
    painter.drawLine(50,50,250,250);
    pixmap.save("output/out");*/

}

void freeArrBright(int** trash, int size)
{
    for(int i = 0; i < size; ++i)
        delete[] trash[i];
    delete [] trash;
}

void imageInfo (QImage image)
{
    qDebug() << "About image :: Size:"<< image.size() << "\n";//<< " Height:" << firstImg.height() << " W:" << firstImg.width() << "\n"
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QImage firstImg,secondImg,outImg;
    /*if(argc == 0)
    {
        std::cout << "App: пропущены операнды, задающие входные файлы\nПо команде «app -h» можно получить дополнительную информацию.";
        return (-1);
    }*/
    int pr = 0;
    while((pr = getopt(argc, argv, "l:r:vhd")) != -1)
    {
        switch (pr)
        {
            case 'l':
                if (!firstImg.load (optarg)) {
                    std::cout << "Cannot load first picture\n";
                    return(-1);
                }
                qDebug() << "F Image path" << optarg;
                break;
            case 'r':
                if (!secondImg.load (optarg)) {
                    std::cout << "Cannot load second picture\n";
                    return(-1);
                }
                qDebug() << "S Image path" << optarg;
                break;
            case 'v':
                //QString version(GIT_VERSION);
                std::cout << "Version: " << GITHASH << "\n";
                return(0);
            case 'h':
                std::cout << "Usage ./binary -l <First image> -o <Second image>\n";
                break;
            case 'd':
                std::cout << "Debug mode ON" << "\n";
            default:
                std::cout << "App: пропущены операнды, задающие входные файлы\nПо команде «app -h» можно получить дополнительную информацию.";
                break;
        }
    }
    firstImg  =  firstImg.convertToFormat(QImage::Format_ARGB32);
    imageInfo(firstImg);
    secondImg = secondImg.convertToFormat(QImage::Format_ARGB32);
    outImg = outImg.convertToFormat(QImage::Format_ARGB32);

    int** pArr = getArrBright(firstImg);


    freeArrBright(pArr, firstImg.width());
    QDir outDir("output");
    if(!outDir.exists())
    {
        outDir.mkpath(".");
    }
    drawVectorOnImage();
    return app.exec();
}//End of Main
