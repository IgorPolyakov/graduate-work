#include "lk_math.h"

/*!
 * \brief expectation − Математи́ческое ожида́ние — среднее значение случайной величины
 * (это распределение вероятностей случайной величины, рассматривается в теории вероятностей)
 * \param [in] var − Ссылка на массив данных.
 * \param [in] size − Длинна массива.
 * \return [out] Матожидание
 */
double expectation(double *var, int size)
{
    double sum = 0;
    int i;

    for(i = 0; i < size; ++i)
        sum += var[i];
    return sum / size;
}

/*!
 * \brief variance − Дисперсия случайной величины — мера разброса данной случайной величины,
 * то есть её отклонения от математического ожидания.
 * \param [in] var − Ссылка на массив данных.
 * \param [in] size − Длинна массива.
 * \return [out] − Дисперсия
 */
double variance(double *var, int size)
{
    double sum = 0;
    double mean = expectation(var, size);

    int i;

    for(i = 0; i < size; ++i)
        sum += (var[i] - mean) * (var[i] - mean);

    return sum / size;
}

/*!
 * \brief std_deviation − Среднеквадратическое отклонение
 * в теории вероятностей и статистике наиболее распространённый
 * показатель рассеивания значений случайной величины относительно её математического ожидания.
 * \param [in] var − Ссылка на массив данных.
 * \param [in] size − Длинна массива.
 * \return [out] − Среднеквадратическое отклонение.
 */
double std_deviation(double *var, int size)
{
    return sqrt(variance(var, size));
}

/*!
 * \brief show − Функция счета статистических показателей
 * \param [in] varname − Имя массива.
 * \param [in] var − Ссылка на массив данных.
 * \param [in] size − Длинна массива.
 */
void show(double *var, int size)
{
    //qDebug() << "Given variable " << varname <<" with uniform distribution:";

    qDebug() << "!Мат. Ожидание: " << expectation(var, size);
    qDebug() << "!Дисперсия: " << variance(var, size);
    qDebug() << "!Среднеквадратическое отклонение: " << std_deviation(var, size);
}
