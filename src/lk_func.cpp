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
 * \brief calcLvlPyramid − Автоматическое вычисление колличества уровней пирамиды на основе размеров изображения
 * \param [in] cx − Ширина изображения
 * \param [in] cy − Высота изображения
 * \param [in] isPyramid − Вычислять ли уровень пирамиды
 * \return [out] − Колличество уровней пирамиды
 */
int calcLvlPyramid(int cx, int cy, bool isPyramid)
{
    int lvlPyramid = 0, tmp = 0;
    if (isPyramid)
    {
        (cx > cy) ? tmp = cy : tmp = cx;

        while ((tmp-(2*(g_sizeWindowSeach + 2))-1)/g_stepForGrid >= 1)
        {
            lvlPyramid++;
            tmp = tmp/2;
        }

        lvlPyramid--;
    }
    else
        lvlPyramid = 0;
    return lvlPyramid;
}

/*!
 * \brief computeGrid − Строит сетку с ранее заданным шагом, в точках
 * пересечения ищется вектор оптического потока.
 * \param [in] leftImg − указатель на массив яркостей первого кадра
 * \param [in] rightImg − указатель на массив яркостей второго кадра
 * \param [in] prev − указатель на векторное поле содержащий предыдущий уровень пирамиды
 * \param [in] info − имя слоя содержащего векторное поле смещений
 * \return [out] двумерный массив содержащий векторное поле, в формате VF
 */
VF2d* computeGrid(Data2Db* leftImg, Data2Db* rightImg, VF2d* prev, QString info)
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

    subSize* kernel = new subSize;
    kernel->rc = g_sizeWindowSeach;
    kernel->step = g_stepForGrid;
    int Vcx = (leftImg->cx()-(2*(kernel->rc+2))-1)/g_stepForGrid;
    int Vcy = (leftImg->cy()-(2*(kernel->rc+2))-1)/g_stepForGrid;
    VF2d *vf = new VF2d("Vector_Field",Vcx, Vcy, g_stepForGrid, g_stepForGrid, g_sizeWindowSeach+1, g_sizeWindowSeach+1, DV_ALIGNMENT);
    Data2Db *state = new Data2Db("Vector_State",vf->cx(),vf->cy());
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
            double progDelt= (89.0/(Vcy * Vcx));
            printProgressBar(progDelt, 0);

            kernel->y_1 = (i * vf->grid().y) + vf->origin().y;
            kernel->x_1 = (j * vf->grid().x) + vf->origin().x;
            kernel->y_2 = (i * vf->grid().y) + vf->origin().y + vf->lines()[i][j][1];
            kernel->x_2 = (j * vf->grid().x) + vf->origin().x + vf->lines()[i][j][0];
            /*TODO:  Cделать проверку выхода за границы*/
            computeOptFlow(kernel, leftImg, rightImg, vf->lines()[i][j], d_mid_x,  d_mid_y,  d_max_x,  d_min_x,  d_min_y,  d_max_y,  d_abs_x,  d_abs_y);
            state->lines()[i][j] = 1;
        }
    }

    QFile writeDeltaX(g_outputFolder + "/dX_"+ info + ".txt");
    QFile writeDeltaY(g_outputFolder + "/dY_"+ info + ".txt");
    writeDeltaX.open(QIODevice::WriteOnly);
    writeDeltaY.open(QIODevice::WriteOnly);
    QTextStream deltXtext(&writeDeltaX);
    QTextStream deltYtext(&writeDeltaY);

    if (g_isDebug)
    {
        double deli = Vcx * Vcy;
        for (int m = 0; m < g_iteration; ++m)
        {
            deltXtext << (d_mid_x[m]/deli)+(d_abs_y[m]/deli) << "\n";// << "\t" << (d_abs_x[m]/deli) << "\t" << d_max_x[m] << "\t" << d_min_x[m] << "\n";
            //deltXtext << m << "\t" << (d_mid_x[m]/deli) << "\t" << (d_abs_x[m]/deli) << "\t" << d_max_x[m] << "\t" << d_min_x[m] << "\n";
            //deltYtext << m << "\t" << (d_mid_y[m]/deli) << "\t" << (d_abs_y[m]/deli) << "\t" << d_max_y[m] << "\t" << d_min_y[m] << "\n";
            //vectXtext << m << "\t" << v_mid_x[m]/deli << "\t" << v_min_x[m] << "\t" << v_max_x[m] << "\n";
            //vectYtext << m << "\t" << v_mid_y[m]/deli << "\t" << v_min_y[m] << "\t" << v_max_y[m] << "\n";
        }
    }

    writeDeltaX.close();
    writeDeltaY.close();

    delete d_mid_x;
    delete d_mid_y;
    delete d_max_x;
    delete d_min_x;
    delete d_max_y;
    delete d_min_y;
    delete d_abs_y;
    delete d_abs_x;

    delete kernel;
    return vf;
}

/*!
 * \brief computeOptFlow − Вычисление вектора оптического потока
 * \param [in] kernel − структура содержащая сведения о местонахождении
 * пикселя, размерах окна поиска и прочего
 * \param [in] leftImg − массив яркостей первого кадра
 * \param [in] rightImg − массив яркостей второго кадра
 * \param [in] dv − векторное поле предыдущего поля
 * \return [out] vf − вектор оптического потока
 */
Vec2d computeOptFlow(subSize* kernel, Data2Db* leftImg, Data2Db* rightImg, Vec2d& dv, double* d_mid_x, double* d_mid_y, double* d_max_x, double* d_min_x, double* d_min_y, double* d_max_y, double* d_avg_x, double* d_avg_y)
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
        double RX[4] = {0.0};
        double RY[4] = {0.0};
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

                tmpX = ((double)leftImg->lines()[yy][xx + 1] - (double)leftImg->lines()[yy][xx - 1]) / 2.0;
                tmpY = ((double)leftImg->lines()[yy + 1][xx] - (double)leftImg->lines()[yy - 1][xx]) / 2.0;
                iX  += tmpX * tmpX;
                iY  += tmpY * tmpY;
                iXY += tmpX * tmpY;

                tmpT = ((double)rightBlock.lines()[kernel->rc + i][kernel->rc + j]) - ((double)leftImg->lines()[yy][xx]);
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

        if (delta[0] >  d_max_x[k])
            d_max_x[k] = delta[0];

        if (delta[1] > d_max_y[k])
            d_max_y[k] = delta[1];

        if (delta[0] < d_min_x[k])
            d_min_x[k] = delta[0];

        if (delta[1] < d_min_y[k])
            d_min_y[k] = delta[1];
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
 * \param [in] image − Cтруктура содержащая сведения о размерах масштабируемого изображения
 * \param [in] kK − Коэффициент уменьшения изображения
 * \param [in] prefix − Префикс имени для создаваемого слоя
 * \return [out] − указатель на массив масштабированных изображений
 */
Data2Db* resizeImage(Data2Db* image, int kK, QString prefix)
{
    int newWidth = (image->cx()/kK);
    int newHeight= (image->cy()/kK);
    int tmp = 0;
    Data2Db* smallImg = new Data2Db(QString(prefix + "_%1").arg(kK).toLocal8Bit().data() ,newWidth, newHeight);
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
 * \param [in] pref − Префикс имени для создаваемого слоя
 * \return [out] Список изображений
 */
std::vector<Data2Db*>* createPyramid_v2(Data2Db* img, int lvl_pyramid, QString pref)
{
    int r = 1;
    std::vector<Data2Db*> *listImg = new std::vector<Data2Db*>;
    img->set_name(pref.toLocal8Bit().data());
    listImg->push_back(img); //первый уровень(оригинал)
    for (int i = 0; i < lvl_pyramid; i++) {
        r = r + r;
        listImg->push_back(resizeImage(img, r, pref));
    }
    return listImg;
}
/*!
 * \brief saveVfResult − Сохранение векторного поля в формате VF. Просмотр возможен в программе df−cl
 * \param [in] vf − Указатель на векторное поле
 * \param [in] info − Имя сохраняемого файла
 */
void saveVfResult(VF2d &vf, QString info)
{
    if (WriteVF(QString(g_outputFolder + "/" + info + ".vf").toLocal8Bit().data(), &vf) == 0) {
        qDebug() << "Correct \""<< info << "\" write data";
    } else {
        qDebug() << "Error : can't write vf data";
    }
    writeHdf5File(QString(g_outputFolder + "/" + info + ".h5"), vf, false);
}

/*!
 * \brief writeHdf5File − Сохранение всех результатов в формат HD5F
 * \param filename − Имя сохраняемого файла
 * \param layer − Указатель на слой
 * \param writeMode − Режим записи для данного слоя
 */
void writeHdf5File(QString filename, ProtoData2D &layer, bool writeMode)
{
    if (writeMode) {
        H5File file = H5File(filename.toLocal8Bit().data(), H5F_ACC_TRUNC);
        Group firstgroup( file.createGroup( "/0" ));
        Group group( firstgroup.createGroup( "Data" ));

        WriteProtoData2D(group, layer);

        group.close();
        firstgroup.close();
        file.close();
    }
    else {
        H5File file = H5File(filename.toLocal8Bit().data(), H5F_ACC_RDWR);
        Group firstgroup( file.openGroup("/0"));
        Group group( firstgroup.openGroup( "Data" ));

        WriteProtoData2D(group, layer);

        group.close();
        firstgroup.close();
        file.close();
    }
}

/*!
 * \brief printProgressBar − Функция ответственная за отрисовку прогресс бара при использовании графического интерфейса
 * \param fastProgBar − Промежуточный уровень
 * \param slowProgBar − Полный уровень
 */
void printProgressBar(double fastProgBar, double slowProgBar)
{
    g_fastProgBar += fastProgBar;
    g_slowProgBar += slowProgBar;
    //std::cout << round(g_fastProgBar) << "," << round(g_slowProgBar) << "," << std::endl;
}

/*!
 * \brief derivativeVectorField − Вычисление поля деформации
 * \param vf − Векторное поле
 * \param info − Имя сохраняемого файла
 */
void derivativeVectorField(VF2d &vf, QString info)
{
    Data2Dd e_xx_tmp("Defom_Exx", vf.cx(), vf.cy(), vf.grid().x, vf.grid().y, vf.origin().x, vf.origin().y);
    Data2Dd e_yy_tmp("Defom_Eyy", vf.cx(), vf.cy(), vf.grid().x, vf.grid().y, vf.origin().x, vf.origin().y);
    Data2Dd e_xy_tmp("Defom_Exy", vf.cx(), vf.cy(), vf.grid().x, vf.grid().y, vf.origin().x, vf.origin().y);
    Data2Dd  g_z_tmp("Defom_SSI", vf.cx(), vf.cy(), vf.grid().x, vf.grid().y, vf.origin().x, vf.origin().y);
    Data2Dd   w_z_tmp("Defom_Wz", vf.cx(), vf.cy(), vf.grid().x, vf.grid().y, vf.origin().x, vf.origin().y);
    double *why = new double[e_xx_tmp.len()];

    double e_xx = 0.0, e_yy = 0.0, e_xy = 0.0, g_i = 0.0, w_z = 0.0;
    for (int i = 0; i < vf.cy(); ++i)
    {
        for (int j = 0; j < vf.cx(); ++j)
        {
            int i_p1 = i + 1 < vf.cy() ? i + 1 : i;
            int i_m1 = i - 1 >= 0 ? i - 1 : i;
            int j_p1 = j + 1 < vf.cx() ? j + 1 : j;
            int j_m1 = j - 1 >= 0 ? j - 1 : j;

            e_xx = (vf.lines()[i][j_p1][0] - vf.lines()[i][j_m1][0])/(2.0 * vf.grid().x);
            e_yy = (vf.lines()[i_p1][j][1] - vf.lines()[i_m1][j][1])/(2.0 * vf.grid().y);
            e_xy = 0.5*((vf.lines()[i_p1][j][0] - vf.lines()[i_m1][j][0])/(2.0 * vf.grid().y) + (vf.lines()[i][j_p1][1] - vf.lines()[i][j_m1][1])/(2.0 * vf.grid().x));
            w_z  = 0.5*((vf.lines()[i][j_p1][1] - vf.lines()[i][j_m1][1])/(2.0 * vf.grid().x) - (vf.lines()[i_p1][j][0] - vf.lines()[i_m1][j][0])/(2.0 * vf.grid().y));

            //! 0.81649658092 - Это sqrt(2/3)
            g_i = 0.81649658092 * sqrt(pow((e_xx - e_yy),2) + pow((e_xx),2) + pow(e_yy ,2) + (1.5)*(pow(e_xy, 2)));
            e_xx_tmp.lines()[i][j] = e_xx;
            why[i*vf.cy() + j] = e_xx;
            e_yy_tmp.lines()[i][j] = e_yy;
            e_xy_tmp.lines()[i][j] = e_xy;
            g_z_tmp.lines()[i][j]  = g_i;
            w_z_tmp.lines()[i][j]  = w_z;
        }
    }
    show(why, e_xx_tmp.len());

    delete why;
    Data2Dd deform_exx("Defom_Exx", vf.grid().x * (vf.cx()-1), vf.grid().y * (vf.cy()-1), 1, 1, vf.origin().x, vf.origin().y);
    BSplineResize2D<double>(e_xx_tmp.lines(),e_xx_tmp.cx(),e_xx_tmp.cy(), deform_exx.lines(), deform_exx.cx(), deform_exx.cy());
    Data2Dd deform_eyy("Defom_Eyy", vf.grid().x * (vf.cx()-1), vf.grid().y * (vf.cy()-1), 1, 1, vf.origin().x, vf.origin().y);
    BSplineResize2D<double>(e_yy_tmp.lines(),e_yy_tmp.cx(),e_yy_tmp.cy(), deform_eyy.lines(), deform_eyy.cx(), deform_eyy.cy());
    Data2Dd deform_exy("Defom_Exy", vf.grid().x * (vf.cx()-1), vf.grid().y * (vf.cy()-1), 1, 1, vf.origin().x, vf.origin().y);
    BSplineResize2D<double>(e_xy_tmp.lines(),e_xy_tmp.cx(),e_xy_tmp.cy(), deform_exy.lines(), deform_exy.cx(), deform_exy.cy());
    Data2Dd deform_g_z("Defom_SSI", vf.grid().x * (vf.cx()-1), vf.grid().y * (vf.cy()-1), 1, 1, vf.origin().x, vf.origin().y);
    BSplineResize2D<double>(g_z_tmp.lines(),g_z_tmp.cx(),g_z_tmp.cy(), deform_g_z.lines(), deform_g_z.cx(), deform_g_z.cy());
    Data2Dd deform_w_z("Defom_Wz", vf.grid().x * (vf.cx()-1), vf.grid().y * (vf.cy()-1), 1, 1, vf.origin().x, vf.origin().y);
    BSplineResize2D<double>(w_z_tmp.lines(),w_z_tmp.cx(),w_z_tmp.cy(), deform_w_z.lines(), deform_w_z.cx(), deform_w_z.cy());

    writeHdf5File(QString(g_outputFolder + "/" + info + ".h5"), deform_exx, false);
    writeHdf5File(QString(g_outputFolder + "/" + info + ".h5"), deform_eyy, false);
    writeHdf5File(QString(g_outputFolder + "/" + info + ".h5"), deform_exy, false);
    writeHdf5File(QString(g_outputFolder + "/" + info + ".h5"), deform_g_z, false);
    writeHdf5File(QString(g_outputFolder + "/" + info + ".h5"), deform_w_z, false);
}
