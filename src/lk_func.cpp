#include "lk_func.h"
#define SIZE_MAT_INV 2

/*!
 * \brief inversion − Нахождения обратной матрицы(2 x 2).
 * \param [in] A − Ссылка на массив
 */
void inversion(Matx22d &A)
{
    double temp;
    double **E = new double *[A.rows];
    for (int i = 0; i < A.rows; i++)
        E[i] = new double [A.rows];
    for (int i = 0; i < A.rows; i++)
        for (int j = 0; j < A.rows; j++) {
            E[i][j] = 0.0;
            if (i == j)
                E[i][j] = 1.0;
        }
    for (int k = 0; k < A.rows; k++) {
        temp = A(k,k);
        for (int j = 0; j < A.rows; j++) {
            A(k,j) /= temp;
            E[k][j] /= temp;
        }
        for (int i = k + 1; i < A.rows; i++) {
            temp = A(i,k);
            for (int j = 0; j < A.rows; j++) {
                A(i,j) -= A(k,j) * temp;
                E[i][j] -= E[k][j] * temp;
            }
        }
    }
    for (int k = A.rows - 1; k > 0; k--) {
        for (int i = k - 1; i >= 0; i--) {
            temp = A(i,k);
            for (int j = 0; j < A.rows; j++) {
                A(i,j) -= A(k,j) * temp;
                E[i][j] -= E[k][j] * temp;
            }
        }
    }
    for (int i = 0; i < A.rows; i++)
        for (int j = 0; j < A.rows; j++)
            A(i,j) = E[i][j];
    for (int i = 0; i < A.rows; i++)
        delete [] E[i];
    delete [] E;
}


/*!
 * \brief calcLvlPyramid - Автоматическое вычисление колличества уровней пирамиды для обрабатываемого изображения
 * \param [in] cx - Ширина
 * \param [in] cy - Высота
 * \return [out] - Колличество уровней пирамиды
 */
int calcLvlPyramid(int cx, int cy, bool isPyramid)
{
    int lvl_pyramid = 0, tmp = 0;
    if (isPyramid)
    {
        if (cx > cy) {
            tmp = cy;
            while ((tmp-(2*(g_sizeWindowSeach + 2))-1)/g_stepForGrid >= 1)
            {
                lvl_pyramid++;
                tmp = tmp/2;
            }
        }
        else {
            tmp = cx;
            while ((tmp-(2*(g_sizeWindowSeach + 2))-1)/g_stepForGrid >= 1)
            {
                lvl_pyramid++;
                tmp = tmp/2;
            }
        }
        lvl_pyramid--;
    }
    else
        lvl_pyramid = 0;
    return lvl_pyramid;
}

/*!
 * \brief computeGrid − Строит сетку по верх изображения, в точках
 * пересечения ищется вектор оптического потока
 * \param [in] leftImg − указатель на массив яркостей первого кадра
 * \param [in] rightImg − указатель на массив яркостей второго кадра
 * \param [in] prev − указатель на векторное поле содержащий предыдущий уровень пирамиды
 * \return [out] двумерный массив содержащий векторное поле, в формате VF
 */
VF2d* computeGrid(Data2Db* leftImg, Data2Db* rightImg, VF2d* prev)
{
    /* WARNING: Переделать по нормальному, структура или т.п*/
    double *d_mid_x = new double[g_iteration];
    double *d_mid_y = new double[g_iteration];
    double *d_max_x = new double[g_iteration];
    double *d_min_x = new double[g_iteration];
    double *d_max_y = new double[g_iteration];
    double *d_min_y = new double[g_iteration];
    double *d_abs_y = new double[g_iteration];
    double *d_abs_x = new double[g_iteration];

    for (int f_var = 0; f_var < g_iteration; ++f_var) {
        d_mid_x[f_var] = d_mid_y[f_var] = d_abs_x[f_var] = d_abs_y[f_var] = 0;
        d_min_x[f_var] = d_min_y[f_var] = 1000.00;
        d_max_x[f_var] = d_max_y[f_var] = -1000.00;

    }

    double *v_mid_x = new double[g_iteration];
    double *v_mid_y = new double[g_iteration];
    double *v_max_x = new double[g_iteration];
    double *v_min_x = new double[g_iteration];
    double *v_max_y = new double[g_iteration];
    double *v_min_y = new double[g_iteration];

    for (int f_var = 0; f_var < g_iteration; ++f_var) {
        v_mid_x[f_var] = v_mid_y[f_var] = 0;
        v_min_x[f_var] = v_min_y[f_var] = 1000.00;
        v_max_x[f_var] = v_max_y[f_var] = -1000.00;

    }


    subSize* kernel = new subSize;
    kernel->rc = g_sizeWindowSeach;
    kernel->step = g_stepForGrid;
    int Vcx = (leftImg->cx()-(2*(kernel->rc+2))-1)/g_stepForGrid;
    int Vcy = (leftImg->cy()-(2*(kernel->rc+2))-1)/g_stepForGrid;
    VF2d *vf = new VF2d(Vcx, Vcy, g_stepForGrid, g_stepForGrid, g_sizeWindowSeach+1, g_sizeWindowSeach+1, DV_ALIGNMENT);
    Data2Db *state = new Data2Db("state",vf->cx(),vf->cy());
    vf->ad() = s_ptr<ProtoData2D>(state);
    if (prev)
    {
        for (int i = 0; i < vf->cy(); ++i)
        {
            for (int j = 0; j < vf->cx(); ++j)
            {
                int tmpX = (j *(prev->cx()-1))/(vf->cx()-1), tmpY = (i * (prev->cy()-1))/(vf->cy()-1);
                vf->lines()[i][j] = prev->lines()[tmpY][tmpX] * 2.0;
            }
        }
    }
    for (int i = 0; i < Vcy; ++i)
    {
        for (int j = 0; j < Vcx; ++j)
        {
            g_fastProgBar += (Vcx * Vcy)/50;//((50 * ((i*Vcx)+j))/(Vcy*Vcx));
            printProgressBar();
            kernel->y_1 = (i * vf->grid().y) + vf->origin().y;
            kernel->x_1 = (j * vf->grid().x) + vf->origin().x;
            kernel->y_2 = (i * vf->grid().y) + vf->origin().y + vf->lines()[i][j][1];
            kernel->x_2 = (j * vf->grid().x) + vf->origin().x + vf->lines()[i][j][0];
            /*TODO:  Cделать проверку выхода за границы*/
            computeOptFlow(kernel, leftImg, rightImg, vf->lines()[i][j], d_mid_x,  d_mid_y,  d_max_x,  d_min_x,  d_min_y,  d_max_y,  d_abs_x,  d_abs_y,  v_mid_x,  v_mid_y,  v_max_x,  v_min_x,  v_min_y,  v_max_y);
            state->lines()[i][j] = 1;
        }
    }

    QFile writeDeltaX(g_outputFolder + "//deltaX.txt");
    QFile writeDeltaY(g_outputFolder + "//deltaY.txt");
    writeDeltaX.open(QIODevice::WriteOnly);
    writeDeltaY.open(QIODevice::WriteOnly);
    QTextStream deltXtext(&writeDeltaX);
    QTextStream deltYtext(&writeDeltaY);

    QFile writeVectorX(g_outputFolder + "//vectorX.txt");
    QFile writeVectorY(g_outputFolder + "//vectorY.txt");
    writeVectorX.open(QIODevice::WriteOnly);
    writeVectorY.open(QIODevice::WriteOnly);
    QTextStream vectXtext(&writeVectorX);
    QTextStream vectYtext(&writeVectorY);

    if (g_isDebug)
    {
        double deli = Vcx * Vcx;
        for (int m = 0; m < g_iteration; ++m)
        {
            deltXtext << m << "\t" << (d_mid_x[m]/deli) << "\t" << (d_abs_x[m]/deli) << "\t" << d_max_x[m] << "\t" << d_min_x[m] << "\n";
            deltYtext << m << "\t" << (d_mid_y[m]/deli) << "\t" << (d_abs_y[m]/deli) << "\t" << d_max_y[m] << "\t" << d_min_y[m] << "\n";
            vectXtext << m << "\t" << v_mid_x[m]/deli << "\t" << v_min_x[m] << "\t" << v_max_x[m] << "\n";
            vectYtext << m << "\t" << v_mid_y[m]/deli << "\t" << v_min_y[m] << "\t" << v_max_y[m] << "\n";
        }
    }

    writeDeltaX.close();
    writeDeltaY.close();
    writeVectorX.close();
    writeVectorY.close();

    delete d_mid_x;
    delete d_mid_y;
    delete d_max_x;
    delete d_min_x;
    delete d_max_y;
    delete d_min_y;
    delete d_abs_y;
    delete d_abs_x;

    delete v_mid_x;
    delete v_mid_y;
    delete v_max_x;
    delete v_min_x;
    delete v_max_y;
    delete v_min_y;

    delete kernel;
    return vf;
}
/*!
 * \brief computeOptFlow − Вычисление вектора оптического потока
 * \param [in] kernel − структура содержащая сведения о местонахождении
 * пикселя, размерах окна поиска и прочего
 * \param [in] leftImg − массив яркостей первого кадра
 * \param [in] rightImg − массив яркостей второго кадра
 * \return [out] vf − вектор оптического потока
 */
Vec2d computeOptFlow(subSize* kernel, Data2Db* leftImg, Data2Db* rightImg, Vec2d& dv, double* d_mid_x, double* d_mid_y, double* d_max_x, double* d_min_x, double* d_min_y, double* d_max_y, double* d_avg_x, double* d_avg_y, double* v_mid_x, double* v_mid_y, double* v_max_x, double* v_min_x, double* v_min_y, double* v_max_y)
{
    double iY = 0.0,   iX = 0.0,   iTX = 0.0, iTY = 0.0, iXY = 0.0;
    double tmpX = 0.0, tmpY = 0.0, tmpT = 0.0;
    Vec2d delta(0,0);
    double thdelta = 0.001;
    for (int k = 0; k < g_iteration; ++k) //Отсчитываем число итераций, для уточнения вектора
    {
        double x2 = kernel->x_1 + dv[0];
        double y2 = kernel->y_1 + dv[1];
        kernel->x_2 = x2;
        kernel->y_2 = y2;
        x2 -= kernel->x_2;
        y2 -= kernel->y_2;

        /*!
         * B-spline interpolation
         *  or
         * Bilinear interpolation
         */
        Data2Dd rightBlock(kernel->rc*2+1, kernel->rc*2+1);
        double RX[4] = {0};
        double RY[4] = {0};
        bicubic_bspline_coefs(RX, RY, x2, y2);
        for (int ii = 0; ii < kernel->rc*2+1; ++ii)
        {
            for (int jj = 0; jj < kernel->rc*2+1; ++jj)
            {
                int xx = jj+kernel->x_2-kernel->rc;
                int yy = ii+kernel->y_2-kernel->rc;

                if (xx < 1) xx = 1;
                if (xx > rightImg->cx()-3) xx = rightImg->cx()-3;
                if (yy < 1) yy = 1;
                if (yy > rightImg->cy()-3) yy = rightImg->cy()-3;

                if (x2 < 0.0001 && y2 < 0.0001)
                    rightBlock.lines()[ii][jj] = rightImg->lines()[yy][xx];
                else
                    switch (g_interpolation) {
                    case 0:
                        rightBlock.lines()[ii][jj] = BicubicBspline2d<uchar>(rightImg->lines(), rightImg->cx(), rightImg->cy(), xx, yy, x2, y2, RX, RY);
                        break;
                    case 1:
                        rightBlock.lines()[ii][jj] = (uchar)bilinearInterpolation(x2, y2, rightImg->lines()[yy][xx], rightImg->lines()[yy][xx+1], rightImg->lines()[yy+1][xx], rightImg->lines()[yy+1][xx+1], xx, yy);
                        break;
                    case 2:
                        double arr[4][4];
                        for (int iii = 0; iii < 4; ++iii) {
                            for (int jjj = 0; jjj < 4; ++jjj) {
                                arr[iii][jjj]= rightImg->lines()[yy+iii-1][xx+jjj-1];
                            }
                        }
                        rightBlock.lines()[ii][jj] = (uchar)bicubicInterpolate(arr,x2,y2);
                        break;
                    default:
                        rightBlock.lines()[ii][jj] = BicubicBspline2d<uchar>(rightImg->lines(), rightImg->cx(), rightImg->cy(), xx, yy, x2, y2, RX, RY);
                        break;
                    }
                /* TODO: Добавить интерполяцию на основе разложения в ряд Тейлора*/
            }
        }

        for (int i = -kernel->rc; i <= kernel->rc; i++)
        {
            for (int j = -kernel->rc; j <= kernel->rc; j++)
            {
                int xx = kernel->x_1 + j;
                int yy = kernel->y_1 + i;
                if (xx<1) xx = 1;
                if (xx+1>leftImg->cx()-1) xx = leftImg->cx()-2;
                if (yy<1) yy = 1;
                if (yy+1>leftImg->cy()-1) yy = leftImg->cy()-2;

                tmpX = ((double)leftImg->lines()[yy][xx - 1] - (double)leftImg->lines()[yy][xx + 1]) / 2.0;
                tmpY = ((double)leftImg->lines()[yy - 1][xx] - (double)leftImg->lines()[yy + 1][xx]) / 2.0;
                iX  += tmpX * tmpX;
                iY  += tmpY * tmpY;
                iXY += tmpX * tmpY;

                tmpT = ((double)leftImg->lines()[yy][xx] - (double)rightBlock.lines()[kernel->rc + i][kernel->rc + j]);
                iTX += tmpX * tmpT;
                iTY += tmpY * tmpT;
            }
        }
        Matx22d A(iX, iXY, iXY, iY);
        Vec2d b(-iTX,-iTY);
        inversion(A);
        delta = A*b;
        dv += delta;

        /* start */
        d_mid_x[k] += delta[0];
        d_mid_y[k] += delta[1];

        d_avg_x[k] += fabs(delta[0]);
        d_avg_y[k] += fabs(delta[1]);

        if(delta[0] >  d_max_x[k])
            d_max_x[k] = delta[0];

        if(delta[1] > d_max_y[k])
            d_max_y[k] = delta[1];

        if(delta[0] < d_min_x[k])
            d_min_x[k] = delta[0];

        if(delta[1] < d_min_y[k])
            d_min_y[k] = delta[1];

        v_mid_x[k] += dv[0];
        v_mid_y[k] += dv[1];

        if(dv[0] > v_max_x[k])
            v_max_x[k] = dv[0];

        if(dv[1] > v_max_y[k])
            v_max_y[k] = dv[1];

        if(delta[0] < v_min_x[k])
            v_min_x[k] = dv[0];

        if(delta[1] < v_min_y[k])
            v_min_y[k] = dv[1];
        /* end */

        if (fabs(delta[0]) < thdelta && fabs(delta[1]) < thdelta) break;
    }
    return dv;
}
/*!
 * \brief multiplicMtrxAndVectr − Произведение матрицы на вектор
 * \param [in] **array − указатель на массив
 * \param [in] *vector − указатель на вектор
 * \return [out] tmp − результат произведения
 */
double* multiplicMtrxAndVectr(double** array, int* vector)
{
    double* tmp = new double[SIZE_MAT_INV];
    for (int cnt = 0; cnt < SIZE_MAT_INV; ++cnt) {
        tmp[cnt] = 0;
    }
    for (int i = 0; i < SIZE_MAT_INV; i++) {
        for (int j = 0; j < SIZE_MAT_INV; j++) {
            tmp[i] += array[i][j] * (double)vector[j];
        }
    }
    return tmp;
}

/*!
 * \brief resizeImage − Функция масштабирования изображения, для построения пирамиды уменьшенных изображений
 * \param [in] image − структура содержащая сведения о размерах масштабируемого изображения
 * \param [in] kK − Коэффициент уменьшения изображения
 * \return [out] − указатель на массив масштабированных изображений
 */
Data2Db* resizeImage(Data2Db* image, int kK)
{
    int newWidth = (image->cx()/kK);
    int newHeight= (image->cy()/kK);
    int tmp = 0;
    Data2Db* smallImg = new Data2Db(newWidth, newHeight);
    for (int i = 0; i < newHeight; i++) {
        for (int j = 0; j < newWidth; j++) {
            for (int ii = 0; ii < kK-1; ++ii) {
                for (int jj = 0; jj < kK-1; ++jj) {
                    tmp = image->lines()[kK * i + ii][ kK * j + jj];
                }
            }
            smallImg->lines()[i][j] = tmp;
            tmp = 0;
        }
    }
    return smallImg;
}
/*!
 * \brief createPyramid_v2 − Выделение памяти для пирамиды изображений
 * \param [in] img − Указатель на оригинальное изображение
 * \param [in] lvl_pyramid − Уровень пирамиды
 * \return [out] Список изображений
 */
std::vector<Data2Db*>* createPyramid_v2(Data2Db* img, int lvl_pyramid)
{
    int r = 1;
    std::vector<Data2Db*> *listImg = new std::vector<Data2Db*>;
    listImg->push_back(img); //первый уровень(оригинал)
    for (int i = 0; i < lvl_pyramid; i++) {
        r = r + r;
        listImg->push_back(resizeImage(img, r));
        g_fastProgBar += (int)((i*10)/lvl_pyramid);
        printProgressBar();
    }
    if(lvl_pyramid==0)
        g_fastProgBar += 10;
    printProgressBar();
    return listImg;
}
/*!
 * \brief saveVfResult − Сохранение векторного поля в формате VF. Просмотр возможен в программе df−cl
 * \param [in] vf − Указатель на векторное поле
 * \param [in] info - Имя сохраняемого файла
 */
void saveVfResult(VF2d &vf, QString info)
{
    if (WriteVF(QString(g_outputFolder + "/" + info + ".vf").toLocal8Bit().data(), &vf) == 0) {
        qDebug() << "Correct write data";
    } else {
        qDebug() << "Error : can't write vf data";
    }
}

void printProgressBar()
{
    //std::cout << g_fastProgBar << "," << g_slowProgBar << "," << std::endl;
}
