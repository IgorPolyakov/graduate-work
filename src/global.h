/*!
\file
\brief Заголовочный файл с описанием глобальных переменных

Данный файл содержит в себе определения глобальных переменных,
 используемых в программе
*/
#ifndef GLOBAL_H
#define GLOBAL_H
#include <QString>

extern bool
g_isDebug;
extern int
g_sizeWindowSeach;
extern int
g_stepForGrid;
extern int
g_iteration;
extern int
g_interpolation;
extern QString
g_outputFolder;
extern double
g_fastProgBar;
extern double
g_slowProgBar;

#endif // GLOBAL_H
