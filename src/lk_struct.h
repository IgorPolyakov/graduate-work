#ifndef LK_STRUCT_H
#define LK_STRUCT_H
//*Заголовочный файл с описанием используемых структур

/*!
 * \brief The SubSize struct
 *Данный файл содержит в себе определения основных
 *структур, используемых в программе
 */
struct SubSize {
    int xCore;          //!< Координата пикселя по x
    int yCore;          //!< Координата пикселя по y
    int radiusCode;     //!< Радиус окна, т.е. если radiusCode = 2, то сетка 5х5
    int xMax;           //!< Размер изображения по горизонтали
    int yMax;           //!< Размер изображения по вертикали
};

/*!
 * \brief The pointerToLvlPyramid struct
 * Тут находятся указатели на выделенную память. Указатель указывает на масштабированные копии
 */
struct pointerToLvlPyramid {
    int* l1;            //!< Первый уровень
    int* l2;            //!< Второй уровень
    int* l3;            //!< Третий уровень
};
#endif
