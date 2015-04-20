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
        std::cout << "LukasKanadeQt: пропущены операнды, задающие входные файлы\nПо команде «lukas_kanade_qt -h» можно получить дополнительную информацию.\n";
        return (0);
    }
    int pr = 0;

    while ((pr = getopt(argc, argv, "l:vhdi:w:g:o:")) != -1) {
        switch (pr) {
        case 'l':
            listfilename = optarg;
            break;
        case 'v':
            std::cout << "LukasKanadeQt version: " << VERSION << "\n";
            std::cout << "branch: " << PROJECT_GIT_REF << "\n";
            std::cout << "build date: " << PROJECT_BUILD_DATE << " " << PROJECT_BUILD_TIME << "\n";
            return (0);
        case 'h':
            std::cout << "\nNAME: \n\tLukasKanadeQt \n\tUsage to EXEC ./lukas_kanade_qt -l <First image> -r <Second image>\n";
            std::cout << "\nDESCRIPTION:";
            std::cout << "\n\tApplication created in order to write a graduate work on specialty 220301\n";
            std::cout << "\n\t-l\t\t load list path image";
            std::cout << "\n\t-o\t\t output directory";
            std::cout << "\n\t-i\t\t count iteration (1 by default)";
            std::cout << "\n\t-w\t\t size window search (3px by default)";
            std::cout << "\n\t-g\t\t step for grid (5px by default)";
            std::cout << "\n\t-v\t\t show version";
            std::cout << "\n\t-h\t\t show help";
            std::cout << "\n\t-d\t\t debug mod on\n";
            return (0);
        case 'w':
            g_sizeWindowSeach = atoi(optarg);
            std::cout << "Size window search: " << g_sizeWindowSeach << "\n";
            break;
        case 'g':
            g_stepForGrid = atoi(optarg);
            std::cout << "Step for grid: " << g_stepForGrid << "\n";
            break;
        case 'i':
            g_iteration = atoi(optarg);
            std::cout << "Count iteration: " << g_iteration << "\n";
            break;
        case 'o':
            g_outputFolder = optarg;
            std::cout << "Output directory: " << g_outputFolder.toStdString() << "\n";
            break;
        case 'd':
            std::cout << "Debug mode: ON" << "\n";
            g_isDebug = true;
            break;
        }
    }

    QFile listfile(listfilename);
    if (!listfile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cout << "Cannot load list image file\n";
        return (-1);
    }

    QStringList imagelist;
    QTextStream in(&listfile);
    while (!in.atEnd())
        imagelist.append(in.readLine());
    listfile.close();

    for (int i = 1, cnt = 0, ocnt = 0; i < imagelist.size(); i++)
    {
        qDebug()<< imagelist.size()<<ocnt<<cnt<<i;
        if (!leftImg.load(imagelist[cnt].toLocal8Bit().data())) {
            std::cout << "Cannot load " << cnt << "image file\n";
            return (-1);
        }
        if (!rightImg.load(imagelist[i].toLocal8Bit().data())) {
            std::cout << "Cannot load " << i << "image file\n";
            return (-1);
        }
        qDebug()<<imagelist[cnt]<<imagelist[i];
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
        info = QString("iteration - %1 sizeWindowSeach - %2 - %3 %4").arg(g_iteration).arg(g_sizeWindowSeach).arg(cnt).arg(i);
        outImg = computeGrid(leftImg, pToLeftImg, pToRightImg);
        joinImage(leftImg, rightImg, outImg, info);
        //memory, now you free!
        delete image;
        freeMemoryInt(pToLeftImg, leftImg.width());
        freeMemoryInt(pToRightImg, rightImg.width());
    }
    return 0;
}//End of Main
