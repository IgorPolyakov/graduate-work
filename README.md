# Lucas–Kanade
In computer vision, the Lucas–Kanade method is a widely used differential method for optical flow estimation developed by Bruce D. Lucas and Takeo Kanade. It assumes that the flow is essentially constant in a local neighbourhood of the pixel under consideration, and solves the basic optical flow equations for all the pixels in that neighbourhood, by the least squares criterion.

By combining information from several nearby pixels, the Lucas–Kanade method can often resolve the inherent ambiguity of the optical flow equation. It is also less sensitive to image noise than point-wise methods. On the other hand, since it is a purely local method, it cannot provide flow information in the interior of uniform regions of the image.

### Version
1.2
### Installation

You need Gulp installed globally:

```sh
$ sudo apt-get install qt-sdk
```

```sh
$ git clone git@github.com:IgorPolyakov/graduate-work.git
$ cd graduate-work
$ cmake CMakeList.txt
$ make
```

### Run
```sh
$ ./bin/lukas_kanade_qt  -l <First image> -r <Second image>
```
### Options
 - l — load left image
 - r — load right image
 - o — output directory
 - i — count iteration (1 by default)
 - w — size window search (3px by default)
 - g — step for grid (5px by default)
 - v — show version
 - h — show help
 - d — debug mod on

### Todo's

 - Write Tests
 - Rethink Github Save
 - Add Code Comments for doxygen

License
----

GNU GPL


**Repository created in order to write a graduate work on specialty 220301.**
