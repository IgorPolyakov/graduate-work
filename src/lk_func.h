#include <QPainter>
#include <QImage>
#include <QDebug>
#include "lk_struct.h"

#ifndef LK_FUNC_H
#define LK_FUNC_H

int** getArrBright(QImage image);
int* calcOptFlow(subSize window, int** arrayGray, int** arrayGrayNext);
int* matrixVectorMultiplic(int** array, int* vector);
void calcGrid(QImage image);
void freeMemoryFloat(int** trash, int size);
void freeMemoryInt(int** trash, int size);
void imageInfo(QImage image, int** arr);
void inversion(int **A, int N);

#endif
