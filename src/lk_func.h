#include <QPainter>
#include <QImage>
#include <QDebug>

#ifndef LK_FUNC_H
#define LK_FUNC_H

int **getArrBright(QImage image);
void caclGrid(QImage image);
void freeArrBright(int** trash, int size);
void imageInfo (QImage image);

#endif
