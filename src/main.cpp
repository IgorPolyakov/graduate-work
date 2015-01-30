//#include <QCoreApplication>
#include <QImage>
#include <QDebug>
#include <qmath.h>
#include <QTime>
#include <QDir>
#include <iostream>
#include <getopt.h>
//#include <omp.h>

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
            std::cout << ary[i][j] << " ";
        };
        std::cout << "\n";
    };
    return ary;
}

void freeArrBright(int** trash, int size)
{
    for(int i = 0; i < size; ++i)
        delete[] trash[i];
    delete [] trash;
}

int main(int argc, char *argv[])
{
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
                std::cout << "App version" << "\n";
                break;
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
    qDebug() << "About image :: Size:"<< firstImg.size() << "\n";//<< " Height:" << firstImg.height() << " W:" << firstImg.width() << "\n";
    secondImg = secondImg.convertToFormat(QImage::Format_ARGB32);
    outImg = outImg.convertToFormat(QImage::Format_ARGB32);

    int** pArr = getArrBright(firstImg);


    freeArrBright(pArr, firstImg.width());
    QDir outDir("output");
    if(!outDir.exists())
    {
        outDir.mkpath(".");
    }
    return 0;
}//End of Main
