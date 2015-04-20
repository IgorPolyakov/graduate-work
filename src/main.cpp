#include <QDebug>
#include <QDir>
#include <QFile>
#include <QImage>
#include <QTextStream>
#include <qmath.h>
#include <iostream>
#include <getopt.h>
#include <stdlib.h>
#include "lk_func.h"
#include "version.h"
int main(int argc, char *argv[])
{
    QImage leftImg, rightImg, outImg;
    QString info, listfilename;
    /*
     * Default opt'arg
     * Begin
     */
    g_isDebug = false;
    g_sizeWindowSeach = 4;
    g_stepForGrid = 10;
    g_iteration = 1;
    g_outputFolder = "output/";
    /*
     * end
     */
    if (argc <= 1) {
        qDebug() << "LukasKanadeQt: пропущены операнды, задающие входные файлы\nПо команде «lukas_kanade_qt -h» можно получить дополнительную информацию.\n";
        return (0);
    }
    int pr = 0;

    while ((pr = getopt(argc, argv, "l:vhdi:w:g:o:")) != -1) {
        switch (pr) {
        case 'l':
            listfilename = optarg;
            break;
        case 'v':
            qDebug() << "LukasKanadeQt version: " << VERSION ;
            qDebug() << "branch: " << PROJECT_GIT_REF ;
            qDebug() << "build date: " << PROJECT_BUILD_DATE << " " << PROJECT_BUILD_TIME ;
            return (0);
        case 'h':
            qDebug() << "\nNAME: \n\tLukasKanadeQt \n\tUsage to EXEC ./lukas_kanade_qt -l <First image> -r <Second image>\n";
            qDebug() << "\nDESCRIPTION:";
            qDebug() << "\n\tApplication created in order to write a graduate work on specialty 220301\n";
            qDebug() << "\n\t-l\t\t load list path image";
            qDebug() << "\n\t-o\t\t output directory";
            qDebug() << "\n\t-i\t\t count iteration (1 by default)";
            qDebug() << "\n\t-w\t\t size window search (3px by default)";
            qDebug() << "\n\t-g\t\t step for grid (5px by default)";
            qDebug() << "\n\t-v\t\t show version";
            qDebug() << "\n\t-h\t\t show help";
            qDebug() << "\n\t-d\t\t debug mod on\n";
            return (0);
        case 'w':
            g_sizeWindowSeach = atoi(optarg);
            qDebug() << "Size window search: " << g_sizeWindowSeach;
            break;
        case 'g':
            g_stepForGrid = atoi(optarg);
            qDebug() << "Step for grid: " << g_stepForGrid;
            break;
        case 'i':
            g_iteration = atoi(optarg);
            qDebug() << "Count iteration: " << g_iteration;
            break;
        case 'o':
            g_outputFolder = optarg;
            qDebug() << "Output directory: " << g_outputFolder;
            break;
        case 'd':
            qDebug() << "Debug mode: ON" ;
            g_isDebug = true;
            break;
        }
    }

    QFile listfile(listfilename);
    if (!listfile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Cannot load list image file\n";
        return (-1);
    }

    QStringList imagelist;
    QTextStream in(&listfile);
    while (!in.atEnd())
        imagelist.append(in.readLine());
    listfile.close();

    for (int i = 1, cnt = 0, ocnt = 0; i < imagelist.size(); i++)
    {
        std::cout << 0 << "," << (100*i)/imagelist.size() << "," << std::endl;
        if (!leftImg.load(imagelist[cnt].toLocal8Bit().data())) {
            qDebug() << "Cannot load " << cnt << "image file\n";
            return (-1);
        }
        if (!rightImg.load(imagelist[i].toLocal8Bit().data())) {
            qDebug() << "Cannot load " << i << "image file\n";
            return (-1);
        }
        leftImg = leftImg.convertToFormat(QImage::Format_ARGB32);
        rightImg = rightImg.convertToFormat(QImage::Format_ARGB32);

        int** pToLeftImg = getArrBright(leftImg);
        int** pToRightImg = getArrBright(rightImg);

        imageInform* image = new imageInform;
        image->height = leftImg.height();
        image->width  = leftImg.width();

        //getImageInfo(image, optarg);

        QDir outDir("output");
        if (!outDir.exists()) {
            outDir.mkpath(".");
        }
        std::cout << 15 << "," << (100*i)/imagelist.size() << "," << std::endl;
        info = QString("iteration - %1 sizeWindowSeach - %2 - %3 %4").arg(g_iteration).arg(g_sizeWindowSeach).arg(cnt).arg(i);
        outImg = computeGrid(leftImg, pToLeftImg, pToRightImg);
        std::cout << 75 << "," << (100*i)/imagelist.size() << "," << std::endl;
        joinImage(leftImg, rightImg, outImg, info);
        //memory, now you free!
        delete image;
        freeMemoryInt(pToLeftImg, leftImg.width());
        freeMemoryInt(pToRightImg, rightImg.width());
        std::cout << 100 << "," << (100*i)/imagelist.size() << "," << std::endl;
    }
    std::cout << 100 << "," << 100 << "," << std::endl;
    return 0;
}//End of Main
