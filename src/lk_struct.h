#ifndef LK_STRUCT_H
#define LK_STRUCT_H
//*Заголовочный файл с описанием используемых структур

/*!
 * \brief The SubSize struct
 *Данный файл содержит в себе определения основных
 *структур, используемых в программе
 */
struct subSize {
    int xCore;          //!< Координата пикселя по x
    int yCore;          //!< Координата пикселя по y
    int radiusCode;     //!< Радиус окна, т.е. если radiusCode = 2, то сетка 5х5
    int xMax;           //!< Размер изображения по горизонтали
    int yMax;           //!< Размер изображения по вертикали
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
