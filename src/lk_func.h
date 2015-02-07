#include <QPainter>
#include <QImage>
#include <QDebug>
#include "lk_struct.h"

#ifndef LK_FUNC_H
#define LK_FUNC_H

int** getArrBright(QImage image);
double* calcOptFlow(subSize window, int** arrayGray, int** arrayGrayNext);
double* matrixVectorMultiplic(double **array, int* vector);
void calcGrid(QImage image);
void freeMemoryFloat(double** trash, int size);
void freeMemoryInt(int** trash, int size);
void imageInfo(QImage image, int** arr);
void inversion(double **A, int N);

#endif
