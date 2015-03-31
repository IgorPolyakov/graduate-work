/*!
\file
\brief Заголовочный файл с описанием глобальных переменных

Данный файл содержит в себе определения глобальных переменных,
 используемых в программе
*/
#ifndef GLOBAL_H
#define GLOBAL_H
#include <QString>

extern bool g_isDebug;           //!< Глобальная переменная - Включить режим вывода в консоль отладочной информации
extern int g_sizeWindowSeach;    //!< Глобальная переменная - Размер окна поиска
extern int g_stepForGrid;        //!< Глобальная переменная - Размер шага сетки (через какое расстояние повторяются точки поиска)
extern int g_iteration;          //!< Глобальная переменная - Число итераций (уточнений)
extern QString g_outputFolder;     //!< Глобальная переменная - Путь для сохранения результатов работы

#endif // GLOBAL_H
