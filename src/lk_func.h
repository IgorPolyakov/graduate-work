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
* \brief computeGrid
 * \param [in] image − исходное изображение()
 * \param [in] arrGrayPrevious − указатель на массив яркостей первого кадра
 * \param [in] arrGrayNext − указатель на массив яркостей второго кадра
 * \return изображение с нанесенным по верх векторным полем
 */
QImage computeGrid(QImage image, int** arrGrayPrevious, int** arrGrayNext);

/*! Вычисление вектора оптического потока. Для одного пикселя!
 * \brief computeOptFlow
 * \param [in] initialWindow − структура содержащая расположение пикселя, размер изображения и радиус поиска
 * \param [in] arrGrayPrevious − указатель на массив яркостей первого кадра
 * \param [in] arrGrayNext − указатель на массив яркостей второго кадра
 * \return вектор оптического потока
 */
double* computeOptFlow(SubSize* initialWindow, int** arrGrayPrevious, int** arrGrayNext);

/*! Произведение матрицы на вектор.
 * \brief multiplicMtrxAndVectr
 * \param [in] **array − указатель на массив
 * \param [in] *vector − указатель на вектор
 * \return  *tmp − результат произведения
 */
double* multiplicMtrxAndVectr(double **array, int* vector);

/*! Преобразование изображения в массив яркостей
 * \brief getArrBright
 * \param [in] image − исходное изображение
 * \return  *tmp − указатель на полученный массив
 */
int** getArrBright(QImage image);

/*! Функцияя освобождения памяти для массивов типа float
* \brief freeMemoryFloat
 * \param [in] **trash − массив для освобождения
 * \param [in] size − размер массива
 */
void freeMemoryFloat(double** trash, int size);

/*! Функцияя освобождения памяти для массивов типа int
 * \brief freeMemoryInt
 * \param [in] **trash − массив для освобождения
 * \param [in] size − размер массива
 */
void freeMemoryInt(int** trash, int size);

/*! Получение информации о входном изображении
 * \brief getImageInfo
 * \param [in] image − Изображение
 * \param [in] path − Путь к нему
 */
void getImageInfo(QImage image, QString path);

/*! Нахождения обратной матрицы.
 * \brief inversion
 * \param [in] **A − Указатель на массив
 * \param [in] N − Его размер
 */
void inversion(double **A, int N);

/*!
 * \brief joinImage
 * Объединение трех изображений(первого, второго и первого с нанесенным поверх веторным полем)
 * \param [in] img1 − Первое изображение
 * \param [in] img2 − Второе изображение
 * \param [in] img3 − Первого с нанесенным поверх веторным полем
 * \param [in] info − Имя сохраняемого файла
 */
void joinImage(QImage img1, QImage img2, QImage img3, QString info);

/*! Функция масштабирует изображения, на вход указатель массив яркостей
 * \brief resizeImage
 * \param [in] image − исходное изображение
 * \param [in] arrGrayPrevious − указатель на массив яркостей первого кадра
 * \param [in] levelPyramid − Колличество уменьшиных копий изображения
 * \param [in] arrGrayNext − указатель на массив яркостей второго кадра
 * \return изображение с нанесенным по верх векторным полем
 */
void resizeImage(QImage image, int** arrGrayPrevious, int** arrGrayNext, int levelPyramid);

/*! Для построения пирамиды, масштабированных изображений, нужно выделить память, чем эта функция и занимается.
 * \brief getMemoryForPyramid
 * \param [in] pointToPyramid
 */
//void getMemoryForPyramid(pointerToLvlPyramid pointToPyramid);

#endif
