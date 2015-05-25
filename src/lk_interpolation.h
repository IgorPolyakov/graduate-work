#ifndef LK_INTERPOLATION
#define LK_INTERPOLATION
#include <deprecated/dvfile.h>
/*!
\file
\brief Заголовочный файл с описанием функций. Данный файл содержит в себе определения функций отвечающих за интерполирование данных.
*/
double bilinearInterpolation (double delx, double dely, uchar q11, uchar q12, uchar q21, uchar q22, int x1, int y1);
double cubicInterpolate     (double p[4], double x);
double bicubicInterpolate   (double p[4][4], double x, double y);
#endif // LK_INTERPOLATION

