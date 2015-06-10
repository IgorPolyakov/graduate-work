#include "lk_interpolation.h"
/*!
 * \brief bilinearInterpolation − Билинейная интерполяция — в вычислительной математике
 * расширение линейной интерполяции для функций двух переменных.
 * \param [in] delx − Смещение по х
 * \param [in] dely − Смещение по у
 * \param [in] q11 − Значение функции в левом верхнем углу
 * \param [in] q12 − Значение функции в левом нижнем углу
 * \param [in] q21 − Значение функции в правом верхнем углу
 * \param [in] q22 − Значение функции в правом нижнем углу
 * \param [in] x1  − Координата верхнего левого угла по х
 * \param [in] y1  − Координата верхнего левого угла по у
 * \return [out] Интерполированное значение
 */
double bilinearInterpolation(double delx, double dely, uchar q11, uchar q12, uchar q21, uchar q22, int x1, int y1)
{
    int x2 = x1 + 1;
    int y2 = y1 + 1;
    double x = x1 + delx;
    double y = y1 + dely;

    double r1 = (((x2 - x)/(x2 - x1)) * q11) + (((x - x1)/(x2 - x1)) * q21);
    double r2 = (((x2 - x)/(x2 - x1)) * q12) + (((x - x1)/(x2 - x1)) * q22);
    double ri = (((y2 - y)/(y2 - y1)) *  r1) + (((y - y1)/(y2 - y1)) *  r2);
    return ri;
}

/*!
 * \brief cubicInterpolate − Кубическая интерполяция
 * \param [in] p −
 * \param [in] x −
 * \return [out] Интерполированное значение
 */
double cubicInterpolate (double p[4], double x) {
    return p[1] + 0.5 * x*(p[2] - p[0] + x*(2.0*p[0] - 5.0*p[1] + 4.0*p[2] - p[3] + x*(3.0*(p[1] - p[2]) + p[3] - p[0])));
}

/*!
 * \brief bicubicInterpolate − Бикубическая интерполяция
 * \param [in] p −
 * \param [in] x −
 * \param [in] y −
 * \return [out] Интерполированное значение
 */
double bicubicInterpolate (double p[4][4], double x, double y) {
    double arr[4];
    arr[0] = cubicInterpolate(p[0], y);
    arr[1] = cubicInterpolate(p[1], y);
    arr[2] = cubicInterpolate(p[2], y);
    arr[3] = cubicInterpolate(p[3], y);
    return cubicInterpolate(arr, x);
}
