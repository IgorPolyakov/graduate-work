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
$ sudo apt-get install qt-sdk libarchive-dev libqt5webkit5-dev 
```
## CMAKE
```sh
$ sudo apt-get install cmake
```
## HDF5 Software
```sh
$ wget http://www.hdfgroup.org/ftp/HDF5/current/src/hdf5-1.8.14.tar
$ tar -xvf hdf5-1.8.14.tar
$ cd hdf5-1.8.14
$ cmake CMakeLists.txt
$ make
$ sudo make install
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
