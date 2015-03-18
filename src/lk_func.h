#include <QPainter>
#include <QImage>
#include <QDebug>
#include "lk_struct.h"
#include "global.h"
/*!
\file
\brief Заголовочный файл с описанием функций

Данный файл содержит в себе определения основных
функций, используемых в программе
*/

#ifndef LK_FUNC_H
#define LK_FUNC_H

/*! Строит сетку по верх изображения, в точках пересечения ищется вектор оптического потока
 * \param [in] QImage image − исходное изображение()
 * \param [in] int** arrGrayPrevious − указатель на массив яркостей первого кадра
 * \param [in] int** arrGrayNext − указатель на массив яркостей второго кадра
 * \return изображение с нанесенным по верх векторным полем
 */
QImage computeGrid(QImage image, int** arrGrayPrevious, int** arrGrayNext);

/*! Вычисление вектора отпического потока. Для одного пикселя!
 * \param [in] SubSize* initialWindow − структура содержащая расположение пикселя, размер изображения и радиус поиска
 * \param [in] int** arrGrayPrevious − указатель на массив яркостей первого кадра
 * \param [in] int** arrGrayNext − указатель на массив яркостей второго кадра
 * \return вектор оптического потока
 */
double* computeOptFlow(SubSize* initialWindow, int** arrGrayPrevious, int** arrGrayNext);

/*! Произведение матрицы на вектор.
 * \param [in] double **array − указатель на массив
 * \param [in] int* vector − указатель на вектор
 * \return  double* tmp − результат произведения
 */
double* multiplicMtrxAndVectr(double **array, int* vector);

/*! Преобразование изображения в массив яркостей
 * \param [in] QImage image − исходное изображение
 * \return  double* tmp − указатель на полученный массив
 */
int** getArrBright(QImage image);

/*! Функцияя освобождения памяти для массивов типа float
 * \param [in] double** trash − массив для освобождения
 * \param [in] int size − размер массива
 */
void freeMemoryFloat(double** trash, int size);

/*! Функцияя освобождения памяти для массивов типа int
 * \param [in] int** trash − массив для освобождения
 * \param [in] int size − размер массива
 */
void freeMemoryInt(int** trash, int size);

/*! Получение информации о входном изображении
 * \param [in] QImage image − Изобразение
 * \param [in] QString path − Путь к нему
 */
void getImageInfo(QImage image, QString path);

/*! Получение обратной матрицы
 * \param [in] double **A − Указатель на массив
 * \param [in] int A − Его размер
 */
void inversion(double **A, int N);

/*! Обьеденение трех изображений(первого, второго и первого с нанесенным поверх веторным полем)
 */
void joinImage(QImage img1, QImage img2, QImage img3, QString info);

#endif
