#include <QPainter>
#include <QImage>
#include <QDebug>
#include "lk_struct.h"

#ifndef LK_FUNC_H
#define LK_FUNC_H

int **getArrBright(QImage image);
void calcGrid(QImage image);
void freeArrBright(int** trash, int size);
void imageInfo(QImage image, int** arr);
void calcOptFlow(subSize window, int** arrayGray, int** arrayGrayNext);

#endif
