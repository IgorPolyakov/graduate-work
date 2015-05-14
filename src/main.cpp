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
#include "dv.h"
#include "deprecated/dvfile.h"
#include "version.h"
int main(int argc, char *argv[])
{
    QString listfilename;
    /*!
     * Default opt'arg:Begin
     */
    g_isDebug = false;
    g_sizeWindowSeach = 24;
    g_stepForGrid = 16;
    g_iteration = 10;
    g_outputFolder = "output/";
    g_interpolation = 1;
    bool isPyramid = false;
    /*!
     * end
     */
    if (argc <= 1) {
        qDebug() <<
                 "LukasKanadeQt: пропущены операнды, задающие входные файлы\nПо команде «lukas_kanade_qt -h» можно получить дополнительную информацию.\n";
        return (0);
    }
    int pr = 0;
    while ((pr = getopt(argc, argv, "l:vhdpi:w:g:o:b:")) != -1) {
        switch (pr) {
        case 'l':
            listfilename = optarg;
            break;
        case 'v':
            qDebug() << "LukasKanadeQt version: " << VERSION ;
            qDebug() << "branch: " << PROJECT_GIT_REF ;
            qDebug() << "build date: " << PROJECT_BUILD_DATE << " " <<
                     PROJECT_BUILD_TIME ;
            return (0);
        case 'h':
            qDebug() << "\nNAME: \n\tLukasKanadeQt \n\tUsage to EXEC ./bin/lukas_kanade_qt  -l <list_of_image>";
            qDebug() << "\nDESCRIPTION:";
            qDebug() << "\n\tApplication created in order to write a graduate work on specialty 220301\n";
            qDebug() << "\n\t-l\t\t load list path image";
            qDebug() << "\n\t-o\t\t output directory";
            qDebug() << "\n\t-i\t\t count iteration (1 by default)";
            qDebug() << "\n\t-w\t\t size window search (3px by default)";
            qDebug() << "\n\t-g\t\t step for grid (5px by default)";
            qDebug() << "\n\t-p\t\t pyramid mod (enable by default)";
            qDebug() << "\n\t-v\t\t show version";
            qDebug() << "\n\t-b\t\t use interpolation method (1 - B-spline, 2 - Bilinear), 1 by default";
            qDebug() << "\n\t-h\t\t show help";
            qDebug() << "\n\t-d\t\t debug mod on\n";
            return (0);
        case 'w':
            g_sizeWindowSeach = atoi(optarg);
            qDebug() << "Size window search: " << g_sizeWindowSeach;
            break;
        case 'b':
            g_interpolation= atoi(optarg);
            qDebug() << "Interpolation method: " << g_interpolation;
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
        case 'p':
            qDebug() << "Pyramid mode: ON" ;
            isPyramid = true;
            break;
        }
    }
    QFile listfile(listfilename);
    if (!listfile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Cannot load image list file\n";
        return (-1);
    }
    QStringList imagelist;
    QTextStream in(&listfile);
    Data2Db *pLeftImg = 0;
    Data2Db *pRightImg = 0;
    VF2d *vf = 0;
    VF2d* prevFiled = 0;

    while (!in.atEnd())
        imagelist.append(in.readLine());
    listfile.close();
    for (int i = 1, cnt = 0, ocnt = 0; i < imagelist.size(); i++) {
        printProgressBar(0, (100*i)/imagelist.size());
        pLeftImg = ReadImage(imagelist[cnt].toLocal8Bit().data());
        if (pLeftImg==NULL) {
            qDebug() << "Cannot load " << cnt << "image file\n";
            return (-1);
        }
        pRightImg = ReadImage(imagelist[i].toLocal8Bit().data());
        if (pRightImg==NULL) {
            qDebug() << "Cannot load " << i << "image file\n";
            return (-1);
        }

        QDir outDir(g_outputFolder);
        if (!outDir.exists()) {
            outDir.mkpath(".");
        }
        printProgressBar(15, (100*i)/imagelist.size());

        int lvl_pyramid = calcLvlPyramid(pLeftImg->cx(),pLeftImg->cy(),isPyramid);

        std::vector<Data2Db*> *listLeft = createPyramid_v2(pLeftImg, lvl_pyramid);
        std::vector<Data2Db*> *listRight = createPyramid_v2(pRightImg, lvl_pyramid);

        printProgressBar(20, (100*i)/imagelist.size());

        if(g_isDebug)
        {
            for (int i_cnt = 0; i_cnt <= lvl_pyramid; ++i_cnt) {
                QString name = QString(g_outputFolder + "/" + "left_%1.png").arg(i_cnt);
                WriteImage(name.toLocal8Bit().data(), (*listLeft)[i_cnt]);
            }

            for (int i_cnt = 0; i_cnt <= lvl_pyramid; ++i_cnt) {
                QString name = QString(g_outputFolder + "/" + "right_%1.png").arg(i_cnt);
                WriteImage(name.toLocal8Bit().data(), (*listRight)[i_cnt]);
            }
        }
            for (int j = lvl_pyramid; j >= 0; j--){
                printProgressBar(20+(lvl_pyramid-j), (100*i)/imagelist.size());
                vf = prevFiled = computeGrid((*listLeft)[j], (*listRight)[j], prevFiled);
                if(g_isDebug)saveVfResult(*vf, "lvl_debug_" + QString("%1").arg(j));
            }
        printProgressBar(75, (100*i)/imagelist.size());
        saveVfResult(*vf, "vector_field_lvl-" + QString("%1").arg(lvl_pyramid));
    }
    printProgressBar(100,100);
    return 0;
}//End of Main
