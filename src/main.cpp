//#include <QCoreApplication>
#include <QImage>
#include <QDebug>
#include <qmath.h>
#include <QTime>
#include <iostream>
#include <getopt.h>
//#include <omp.h>


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
    secondImg = secondImg.convertToFormat(QImage::Format_ARGB32);
    outImg = outImg.convertToFormat(QImage::Format_ARGB32);
    /*
        if (!outImg.load (QString (argv[8]))) {
            std::cout << "Cannot load out picture\n";
            return(-1);
        }
    */
    return 0;
}//End of Main
