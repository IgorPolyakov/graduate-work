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
Vec2d   computeOptFlow(subSize* kernel, Data2Db *leftImg, Data2Db *rightImg, Vec2d &dv, double *d_mid_x, double *d_mid_y, double *d_max_x, double *d_min_x, double *d_min_y, double *d_max_y, double *d_avg_x, double *d_avg_y);
double* multiplicMtrxAndVectr(double **array, int* vector);
double  bilinearInterpolation(double delx, double dely, uchar q11, uchar q12, uchar q21, uchar q22, int x1, int y1);
std::vector<Data2Db*>* createPyramid_v2(Data2Db* img, int lvl_pyramid);
Data2Db* resizeImage(Data2Db* image, int kK);
VF2d*   computeGrid(Data2Db* leftImg, Data2Db* rightImg, VF2d *prev);
void    getImageInfo(imageInform* image, QString path);
void    inversion(Matx22d &A);
void    joinImage(QImage img1, QImage img2, QImage img3, QString info);
void    saveVfResult(VF2d &vf, QString info);
void    printProgressBar(int fast, int slow);
int     calcLvlPyramid(int cx, int cy, bool isPyramid);
#endif
