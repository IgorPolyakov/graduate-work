#include <QPainter>
#include <QImage>
#include <QDebug>
#include "lk_struct.h"
#include "global.h"

#ifndef LK_FUNC_H
#define LK_FUNC_H

QImage computeGrid(QImage image, int** arrGrayPrevious, int** arrGrayNext);
double* computeOptFlow(SubSize* initialWindow, int** arrGrayPrevious, int** arrGrayNext);
double* multiplicMtrxAndVectr(double **array, int* vector);
int** genrateData(int w, int h);
int** getArrBright(QImage image);
void freeMemoryFloat(double** trash, int size);
void freeMemoryInt(int** trash, int size);
void getImageInfo(QImage image, QString path);
void inversion(double **A, int N);
void joinImage(QImage img1, QImage img2, QImage img3, QString info);

#endif
