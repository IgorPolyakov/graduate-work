#include <QDebug>
#include <QDir>
#include <QImage>
#include <qmath.h>
#include <iostream>
#include <getopt.h>
#include "lk_func.h"

int main(int argc, char *argv[])
{
    QImage leftImg, rightImg, outImg;
    g_isDebug = false;
    if (argc <= 1) {
        std::cout << "LukasKanadeQt: пропущены операнды, задающие входные файлы\nПо команде «lukas_kanade_qt -h» можно получить дополнительную информацию.\n";
        return(0);
    }
    int pr = 0;
    while ((pr = getopt(argc, argv, "l:r:vhd")) != -1) {
        switch (pr) {
        case 'l':
            if (!leftImg.load(optarg)) {
                std::cout << "Cannot load left picture\n";
                return (-1);
            }
            getImageInfo(leftImg, optarg);
            break;
        case 'r':
            if (!rightImg.load(optarg)) {
                std::cout << "Cannot load right picture\n";
                return (-1);
            }
            getImageInfo(rightImg, optarg);
            break;
        case 'v':
            std::cout << "LukasKanadeQt version: " << GITHASH << "\n";
            return (0);
        case 'h':
            std::cout << "\nNAME: \n\tLukasKanadeQt \n\tUsage to EXEC ./lukas_kanade_qt -l <First image> -r <Second image>\n";
            std::cout << "\nDESCRIPTION:";
            std::cout << "\n\tApplication created in order to write a graduate work on specialty 220301\n";
            std::cout << "\n\t-l\t\t load left image";
            std::cout << "\n\t-r\t\t load right image";
            std::cout << "\n\t-v\t\t show version";
            std::cout << "\n\t-h\t\t show help";
            std::cout << "\n\t-d\t\t debug mod on\n";
            return (0);
        case 'd':
            std::cout << "Debug mode ON" << "\n";
            g_isDebug = true;
            break;
        }
    }

    leftImg = leftImg.convertToFormat(QImage::Format_ARGB32);
    rightImg = rightImg.convertToFormat(QImage::Format_ARGB32);
    outImg = outImg.convertToFormat(QImage::Format_ARGB32);

    int** pToLeftImg = getArrBright(leftImg);
//   int** pToLeftImg = genrateData(leftImg.height(),leftImg.width());
    int** pToRightImg = getArrBright(rightImg);

    QDir outDir("output");
    if (!outDir.exists()) {
        outDir.mkpath(".");
    }
    SubSize ARA = {10, 13, 10, 13};


    computeOptFlow(ARA, pToLeftImg, pToRightImg);

    freeMemoryInt(pToLeftImg, leftImg.width());
    freeMemoryInt(pToRightImg, rightImg.width());
    return 0;
}//End of Main
