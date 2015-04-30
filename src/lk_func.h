#include <QPainter>
#include <QImage>
#include <QDebug>
#include "lk_struct.h"
#include "global.h"
#include "dv.h"

/*!
\file
\brief Заголовочный файл с описанием функций. Данный файл содержит в себе определения основных функций, используемых в программе
*/
#ifndef LK_FUNC_H
#define LK_FUNC_H
    Vec2d computeOptFlow(subSize* kernel, Data2Db *leftImg,Data2Db *rightImg);
    double* multiplicMtrxAndVectr(double **array, int* vector);
    int**   getMemoryForPyramid(imageInform* image, int** arrGrayPrevious);
    int*    resizeImage(imageInform* image, int** arrGrayPrevious, int kK);
    VF2d *computeGrid(Data2Db* leftImg, Data2Db* rightImg);
    void    getImageInfo(imageInform* image, QString path);
    void    inversion(Matx22d &A);
    void    joinImage(QImage img1, QImage img2, QImage img3, QString info);
    void    saveVfResult(VF2d &vf);
#endif
