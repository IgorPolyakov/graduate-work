#include <QDebug>
#include <QDir>
#include <QImage>
#include <qmath.h>
#include <iostream>
#include <getopt.h>
#include "lk_func.h"

int main(int argc, char *argv[])
{
    //QCoreApplication app(argc, argv);
    QImage firstImg, secondImg, outImg;
    if (optarg == NULL) {
        //std::cout << "LukasKanadeQt: пропущены операнды, задающие входные файлы\nПо команде «lukas_kanade_qt -h» можно получить дополнительную информацию.";
        //return(0);
    }
    int pr = 0;
    while ((pr = getopt(argc, argv, "l:r:vhd")) != -1) {
        switch (pr) {
        case 'l':
            if (!firstImg.load(optarg)) {
                std::cout << "Cannot load first picture\n";
                return (-1);
            }
            qDebug() << "F Image path" << optarg;
            break;
        case 'r':
            if (!secondImg.load(optarg)) {
                std::cout << "Cannot load second picture\n";
                return (-1);
            }
            qDebug() << "S Image path" << optarg;
            break;
        case 'v':
            std::cout << "LukasKanadeQt version: " << GITHASH << "\n";
            return (0);
        case 'h':
            std::cout << "\nNAME: \n\tLukasKanadeQt \n\tUsage to EXEC ./lukas_kanade_qt -l <First image> -o <Second image>\n";
            std::cout << "\nDESCRIPTION:";
            std::cout << "\n\tApplication created in order to write a graduate work on specialty 220301\n";
            std::cout << "\n\t-l\n\t\t load left image";
            std::cout << "\n\t-r\n\t\t load right image";
            std::cout << "\n\t-v\n\t\t show version";
            std::cout << "\n\t-h\n\t\t show help";
            std::cout << "\n\t-d\n\t\t debug mod on\n";
            return (0);
        case 'd':
            std::cout << "Debug mode ON" << "\n";
            break;
        }
    }
    firstImg  =  firstImg.convertToFormat(QImage::Format_ARGB32);
    imageInfo(firstImg);
    secondImg = secondImg.convertToFormat(QImage::Format_ARGB32);
    imageInfo(secondImg);
    outImg = outImg.convertToFormat(QImage::Format_ARGB32);

    int** pArr = getArrBright(firstImg);
    freeArrBright(pArr, firstImg.width());
    QDir outDir("output");
    if (!outDir.exists()) {
        outDir.mkpath(".");
    }
    caclGrid(firstImg);
    return 0;
}//End of Main
