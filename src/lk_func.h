#include "dv.h"
#include "global.h"
#include "lk_interpolation.h"
#include "lk_struct.h"
#include "lk_math.h"
#include <deprecated/dvfile.h>
#include <iostream>
#include <math.h>
#include <QDebug>
#include <QFile>
#include <vector>
#include "hdf5io.h"

/*!
\file
\brief Заголовочный файл с описанием функций. Данный файл содержит в себе определения основных функций, используемых в программе
*/
#ifndef LK_FUNC_H
#define LK_FUNC_H
Data2Db* resizeImage(Data2Db* image, int kK, QString prefix);
double* multiplicMtrxAndVectr(double **array, int* vector);
int     calcLvlPyramid(int cx, int cy, bool isPyramid);
std::vector<Data2Db*>* createPyramid_v2(Data2Db* img, int lvl_pyramid, QString pref);
Vec2d   computeOptFlow(subSize* kernel, Data2Db* leftImg, Data2Db* rightImg, Vec2d& dv, double* d_mid_x, double* d_mid_y, double* d_max_x, double* d_min_x, double* d_min_y, double* d_max_y, double* d_avg_x, double* d_avg_y);
VF2d*   computeGrid(Data2Db* leftImg, Data2Db* rightImg, VF2d *prev, QString info);
//void    getImageInfo(imageInform* image, QString path);
void    inversion(Matx22d &A);
void    printProgressBar(double fastProgBar, double slowProgBar);
void    saveVfResult(VF2d &vf, QString info);
void    derivativeVectorField(VF2d &vf, QString info);
void    writeHdf5File(QString path, ProtoData2D &layer, bool writeMode);
#endif
