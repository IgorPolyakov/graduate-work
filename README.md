# Lucas–Kanade

In computer vision, the Lucas–Kanade method is a widely used differential method for optical flow estimation developed by Bruce D. Lucas and Takeo Kanade. It assumes that the flow is essentially constant in a local neighbourhood of the pixel under consideration, and solves the basic optical flow equations for all the pixels in that neighbourhood, by the least squares criterion.

By combining information from several nearby pixels, the Lucas–Kanade method can often resolve the inherent ambiguity of the optical flow equation. It is also less sensitive to image noise than point-wise methods. On the other hand, since it is a purely local method, it cannot provide flow information in the interior of uniform regions of the image.

## Version
1.2
## Installation
You need install some packet:
#### Qt
```sh
$ sudo apt-get install qt-sdk 
$ sudo apt-get install libarchive-dev libqt5webkit5-dev
```
#### HDF5 Software
```sh
cmake
$ wget http://www.hdfgroup.org/ftp/HDF5/current/src/hdf5-1.8.14.tar
$ tar -xvf hdf5-1.8.14.tar
$ cmake CMakeList.txt
$ make
$ sudo make install
```
#### DV(Deformation analys)
```sh
$ cp /media/flash_card/dv.zip
$ unzip dv.zip
$ cd dv/df-cl
$ cmake CMakeList.txt
$ make
```

#### Lucas-Kanade
```sh
$ git clone git@github.com:IgorPolyakov/graduate-work.git
$ cd graduate-work
$ cmake CMakeList.txt
$ make
```

## Run
```sh
$ ./bin/lukas_kanade_qt  -l <list_of_image>
```
## Options
 - l — load list image
 - o — output directory
 - i — count iteration (1 by default)
 - w — size window search (3px by default)
 - g — step for grid (5px by default)
 - v — show version
 - h — show help
 - d — debug mod on

## Todo's
 - Added pyramid
 - Write Tests
 - Add Code Comments for doxygen

License
----

GNU GPL

**Repository created in order to write a graduate work on specialty 220301.**
