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
double expectation(double &var, int size);
double variance(double &var, int size);
double std_deviation(double &var, int size);
void   show(double *var, int size);

#endif // LK_MATH
