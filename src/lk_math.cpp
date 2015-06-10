#include "lk_math.h"

#define SIZE(ARR) (sizeof(ARR) / sizeof(*ARR))

/*!
 * \brief expectation − Математи́ческое ожида́ние — среднее значение случайной величины
 * (это распределение вероятностей случайной величины, рассматривается в теории вероятностей)
 * \param [in] var − Ссылка на массив данных.
 * \param [in] size − Длинна массива.
 * \return [out] Матожидание
 */
double expectation(VF2d &var, size_t size)
{
    double sum = 0;
    size_t i;

    for(i = 0; i < size; ++i)
        sum += (double)var.data()[i][0];
        //qDebug() << var.data()[3];
    return sum / size;
}

/*!
 * \brief variance − Дисперсия случайной величины — мера разброса данной случайной величины,
 * то есть её отклонения от математического ожидания.
 * \param [in] var − Ссылка на массив данных.
 * \param [in] size − Длинна массива.
 * \return [out] − Дисперсия
 */
double variance(VF2d &var, size_t size)
{
    double sum = 0;
    double mean = expectation(var, size);

    size_t i;

    for(i = 0; i < size; ++i)
        sum += (var.data()[i][0] - mean) * (var.data()[i][0] - mean);

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
double std_deviation(VF2d &var, size_t size)
{
    return sqrt(variance(var, size));
}

/*!
 * \brief show − Функция счета статистических показателей
 * \param [in] varname − Имя массива.
 * \param [in] var − Ссылка на массив данных.
 * \param [in] size − Длинна массива.
 */
void show(QString varname, VF2d &var, int size)
{
    qDebug() << "Given variable " << varname <<" with uniform distribution:\n";

    printf("Мат. Ожидание: %.5f\n", expectation(var, size));
    printf("Дисперсия: %.5f\n", variance(var, size));
    printf("Среднеквадратическое отклонение: %.5f\n", std_deviation(var, size));
}
