#include <QPainter>
#include <QImage>
#include <QDebug>
#include "lk_struct.h"
#include "global.h"

#ifndef LK_FUNC_H
#define LK_FUNC_H

int** getArrBright(QImage image);
double* computeOptFlow(SubSize window, int** arrGrayPrevious, int** arrGrayNext);
double* multiplicMtrxAndVectr(double **array, int* vector);
void computeGrid(QImage image);
void freeMemoryFloat(double** trash, int size);
void freeMemoryInt(int** trash, int size);
void getImageInfo(QImage image, int** arr);
void inversion(double **A, int N);
int** genrateData(int w, int h);

#endif
