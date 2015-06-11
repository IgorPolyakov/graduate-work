#include <QString>
bool g_isDebug;         //!< g_isDebug − Включить режим вывода в консоль отладочной информации
int g_sizeWindowSeach;  //!< g_sizeWindowSeach − Размер окна поиска
int g_stepForGrid;      //!< g_stepForGrid − Размер шага сетки (через какое расстояние повторяются точки поиска)
int g_iteration;        //!< g_iteration − Число итераций (уточнений)
int g_interpolation;    //!< g_interpolation − Выбор метода интерполирования (0 - Б-сплайн, 1 - Билинейный, 3 - Бикубический)
double g_fastProgBar;   //!< g_fastProgBar − Переменная для отображения текущего прогресса исполнения программы
double g_slowProgBar;   //!< g_slowProgBar − Переменная для отображения общего прогресса исполнения программы
QString g_outputFolder; //!< g_outputFolder − Директория для сохранения результатов работы
