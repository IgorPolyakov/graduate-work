#ifndef LK_INTERPOLATION
#define LK_INTERPOLATION
double bilinearInterpolation (double delx, double dely, uchar q11, uchar q12, uchar q21, uchar q22, int x1, int y1);
double cubicInterpolate     (double p[4], double x);
double bicubicInterpolate   (double p[4][4], double x, double y);
double tricubicInterpolate  (double p[4][4][4], double x, double y, double z);
#endif // LK_INTERPOLATION

