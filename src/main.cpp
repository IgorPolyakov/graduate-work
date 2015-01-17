//#include <QCoreApplication>
//#include <QImage>
#include <QDebug>
#include <qmath.h>
#include <QTime>
#include <iostream>
//#include <omp.h>


int main(int argc, char *argv[])
{
/*
    if (!outImg.load (QString (argv[8]))) {
        std::cout << "Cannot load out picture\n";
        return(-1);
    }
*/
    if(argc < 3)
    {
        std::cout << "Please, input two images" << std::endl;
        return (-1);
    }
    int pr = 0;
    while((pr = getopt(argc, argv, shortopts.toAscii().data())) != -1)
    {
        switch (pr)
        {
            case 'l':
                if (!firstImg.load (optarg)) {
                    std::cout << "Cannot load first picture\n";
                    return(-1);
                }
                break;
            case 'r':
                if (!secondImg.load (optarg)) {
                    std::cout << "Cannot load second picture\n";
                    return(-1);
                }
                break;
            case 'v':
                //QString version(GIT_VERSION);
                std::cout << "App version" << "\n";
                break;
            case 'h':
                std::cout << "Usage ./binary -l <First image> -o <Second image>";
                break;
            case 'd':
                std::cout << "Debug mode ON" << "\n";
            default:
                break;
        }
    }
    firstImg  =  firstImg.convertToFormat(QImage::Format_ARGB32);
    secondImg = secondImg.convertToFormat(QImage::Format_ARGB32);
    outImg = outImg.convertToFormat(QImage::Format_ARGB32);
    return 0;
}//End of Main
