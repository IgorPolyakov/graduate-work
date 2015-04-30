#ifndef LK_STRUCT_H
#define LK_STRUCT_H
//*Заголовочный файл с описанием используемых структур

/*!
 * \brief The SubSize struct
 *Данный файл содержит в себе определения основных
 *структур, используемых в программе
 */
struct subSize {
    int cx;             //!cx Координата пикселя по x
    int cy;             //!cy Координата пикселя по y
    int rc;             //!rc Радиус окна, т.е. если radiusCode = 2, то сетка 5х5
    int step;           //!step - Шаг между векторами
};

/*!
 * \brief The imageInform struct
 * Хранение информации о размерах изображения
 */
struct imageInform {
    int height;         //!< Размер изображения по высоте
    int width;         //!< Размер изображения по ширине
};

#endif
