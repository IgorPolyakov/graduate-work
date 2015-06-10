#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <QDebug>
#include "dv.h"
#ifndef LK_MATH
#define LK_MATH
/*!
\file
\brief Заголовочный файл с описанием функций.
* Данный файл содержит в себе определения функций отвечающих за подсчет статистических показателей.
*/
double expectation(VF2d &var, size_t size);
double variance(VF2d &var, size_t size);
double std_deviation(VF2d &var, size_t size);
void   show(QString varname, VF2d &var, int size);

#endif // LK_MATH

