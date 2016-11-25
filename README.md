# Lucas–Kanade

Целью настоящей работы является разработка программного обеспечения (ПО) для оценки деформаций поверхностей твёрдых тел, а также проведение исследований алгоритмов и методов как на модельных, так и на реальных оптических изображениях.

В работе исследовано влияние метода интерполяции изображений с субпиксельной точностью с использованием итеративного подхода на расчёт оптического потока(векторного поля).

Проект выполнен с использованием следующих средств разработки: языка программирования C++(Qt), среды разработки QtCreator 3, Sublime 3. Система контроля версий git.

## Версия
1.4
## Установка
Для начала, необходимо настроить окружение:
## Qt
```sh
$ sudo apt-get install qt5-default libopencv-dev libqt5opengl5-dev libqt5quick5 qtcreator libnglib-dev  libarchive-dev libqt5webkit5-dev 
```
## CMAKE
```sh
$ sudo add-apt-repository ppa:george-edison55/cmake-3.x
$ sudo apt-get update
$ sudo apt-get install cmake cmake-qt-gui
```
## HDF5 Software
```sh
$ wget http://www.hdfgroup.org/ftp/HDF5/current/src/hdf5-1.8.16.tar.gz
$ tar -xvf hdf5-1.8.16.tar.gz
$ cd hdf5-1.8.16
$ mkdir build
$ cd build
$ cmake ../
$ make -j`nproc`
$ sudo make install
$ ctest -S HDF5config.cmake,BUILD_GENERATOR=Unix,STATIC_LIBRARIES=OFF -C Release -V -O hdf5.log
```
## DV(Deformation analys)
```sh
$ cp /media/flash_card/dv.zip
$ unzip dv.zip
$ cd dv/df-cl
$ cmake CMakeLists.txt
$ make
```
## Lucas-Kanade
```sh
$ git clone git@github.com:IgorPolyakov/graduate-work.git
$ cd graduate-work
$ cmake CMakeLists.txt
$ make
```
## Запуск
```sh
$ ./bin/lucas_kanade  -l <list_of_image>
```
## Опции
 - l — load list image
 - o — output directory
 - i — count iteration (1 by default)
 - w — size window search (3px by default)
 - g — step for grid (5px by default)
 - v — show version
 - h — show help
 - d — debug mod on

## Список дел
 - Добавить юнит-тесты

## Лицензия
----
GNU GPL

В случае если cmake возвращает ошибку, добавить переменную для сборки.
```
Could not find a package configuration file provided by "HDF5" with any of
  the following names:

    HDF5Config.cmake
    hdf5-config.cmake
```
-DHDF5_PATH=/usr/local/HDF_Group/HDF5/1.8.1#/share/cmake/

## Update
QtCreator::Tool -> Option -> Build&Run -> Kits -> Default -> Enviroment
add to window PATH=${PATH}:/usr/local/HDF_Group/HDF5/1.8.17/share/cmake

