/*
 * pinv.c
 *
 * Code generation for function 'pinv'
 *
 * C source code generated on: Wed Jan 21 21:50:20 2015
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "lucas_kanade_matlab.h"
#include "pinv.h"

/* Function Declarations */
static void b_eml_xscal(real_T a, real_T x[4], int32_T ix0);
static real_T eml_div(real_T x, real_T y);
static void eml_xaxpy(real_T a, real_T y[4]);
static real_T eml_xdotc(const real_T x[4], const real_T y[4]);
static void eml_xgesvd(const real_T A[4], real_T U[4], real_T S[2], real_T V[4]);
static real_T eml_xnrm2(const real_T x[4]);
static void eml_xrot(real_T x[4], int32_T ix0, int32_T iy0, real_T c, real_T s);
static void eml_xrotg(real_T *a, real_T *b, real_T *c, real_T *s);
static void eml_xscal(real_T a, real_T x[4]);
static void eml_xswap(real_T x[4], int32_T ix0, int32_T iy0);

/* Function Definitions */

/*
 *
 */
static void b_eml_xscal(real_T a, real_T x[4], int32_T ix0)
{
  int32_T k;
  for (k = ix0; k <= ix0 + 1; k++) {
    x[k - 1] *= a;
  }
}

/*
 *
 */
static real_T eml_div(real_T x, real_T y)
{
  return x / y;
}

/*
 *
 */
static void eml_xaxpy(real_T a, real_T y[4])
{
  int32_T ix;
  int32_T iy;
  int32_T k;
  if (a == 0.0) {
  } else {
    ix = 0;
    iy = 2;
    for (k = 0; k < 2; k++) {
      y[iy] += a * y[ix];
      ix++;
      iy++;
    }
  }
}

/*
 *
 */
static real_T eml_xdotc(const real_T x[4], const real_T y[4])
{
  real_T d;
  int32_T ix;
  int32_T iy;
  int32_T k;
  d = 0.0;
  ix = 0;
  iy = 2;
  for (k = 0; k < 2; k++) {
    d += x[ix] * y[iy];
    ix++;
    iy++;
  }

  return d;
}

/*
 *
 */
static void eml_xgesvd(const real_T A[4], real_T U[4], real_T S[2], real_T V[4])
{
  real_T b_A[4];
  real_T Vf[4];
  int32_T kase;
  real_T nrm;
  real_T s[2];
  int32_T m;
  real_T e[2];
  int32_T q;
  real_T rt;
  real_T ztest;
  int32_T iter;
  real_T tiny;
  real_T snorm;
  boolean_T exitg3;
  int32_T qs;
  boolean_T exitg2;
  real_T sn;
  real_T varargin_1[5];
  boolean_T exitg1;
  real_T sqds;
  real_T b;
  for (kase = 0; kase < 4; kase++) {
    b_A[kase] = A[kase];
    U[kase] = 0.0;
    Vf[kase] = 0.0;
  }

  nrm = eml_xnrm2(A);
  if (nrm > 0.0) {
    if (A[0] < 0.0) {
      nrm = -nrm;
    }

    eml_xscal(eml_div(1.0, nrm), b_A);
    b_A[0]++;
    s[0] = -nrm;
  } else {
    s[0] = 0.0;
  }

  if (s[0] != 0.0) {
    eml_xaxpy(-eml_div(eml_xdotc(b_A, b_A), b_A[0]), b_A);
  }

  m = 1;
  s[1] = b_A[3];
  e[0] = b_A[2];
  e[1] = 0.0;
  for (kase = 0; kase < 2; kase++) {
    U[kase] = b_A[kase];
    U[2 + kase] = 0.0;
  }

  U[3] = 1.0;
  if (s[0] != 0.0) {
    eml_xaxpy(-eml_div(eml_xdotc(U, U), U[0]), U);
    for (kase = 0; kase < 2; kase++) {
      U[kase] = -U[kase];
    }

    U[0]++;
  } else {
    for (kase = 0; kase < 2; kase++) {
      U[kase] = 0.0;
    }

    U[0] = 1.0;
  }

  for (q = 1; q > -1; q += -1) {
    for (kase = 0; kase < 2; kase++) {
      Vf[kase + (q << 1)] = 0.0;
    }

    Vf[q + (q << 1)] = 1.0;
  }

  nrm = b_A[2];
  for (q = 0; q < 2; q++) {
    if (s[q] != 0.0) {
      rt = fabs(s[q]);
      ztest = eml_div(s[q], rt);
      s[q] = rt;
      if (q + 1 < 2) {
        nrm = eml_div(nrm, ztest);
      }

      b_eml_xscal(ztest, U, (q << 1) + 1);
    }

    if ((q + 1 < 2) && (nrm != 0.0)) {
      rt = fabs(nrm);
      ztest = eml_div(rt, nrm);
      nrm = rt;
      s[1] *= ztest;
      b_eml_xscal(ztest, Vf, 3);
    }

    e[0] = nrm;
  }

  iter = 0;
  tiny = eml_div(2.2250738585072014E-308, 2.2204460492503131E-16);
  snorm = 0.0;
  for (kase = 0; kase < 2; kase++) {
    nrm = fabs(s[kase]);
    ztest = fabs(e[kase]);
    if ((nrm >= ztest) || rtIsNaN(ztest)) {
    } else {
      nrm = ztest;
    }

    if ((snorm >= nrm) || rtIsNaN(nrm)) {
    } else {
      snorm = nrm;
    }
  }

  while ((m + 1 > 0) && (!(iter >= 75))) {
    q = m;
    exitg3 = FALSE;
    while (!((exitg3 == TRUE) || (q == 0))) {
      nrm = fabs(e[0]);
      if ((nrm <= 2.2204460492503131E-16 * (fabs(s[0]) + fabs(s[1]))) || (nrm <=
           tiny) || ((iter > 20) && (nrm <= 2.2204460492503131E-16 * snorm))) {
        e[0] = 0.0;
        exitg3 = TRUE;
      } else {
        q = 0;
      }
    }

    if (q == m) {
      kase = 4;
    } else {
      qs = m + 1;
      kase = m + 1;
      exitg2 = FALSE;
      while ((exitg2 == FALSE) && (kase >= q)) {
        qs = kase;
        if (kase == q) {
          exitg2 = TRUE;
        } else {
          nrm = 0.0;
          if (kase < m + 1) {
            nrm = fabs(e[0]);
          }

          if (kase > q + 1) {
            nrm += fabs(e[0]);
          }

          ztest = fabs(s[kase - 1]);
          if ((ztest <= 2.2204460492503131E-16 * nrm) || (ztest <= tiny)) {
            s[kase - 1] = 0.0;
            exitg2 = TRUE;
          } else {
            kase--;
          }
        }
      }

      if (qs == q) {
        kase = 3;
      } else if (qs == m + 1) {
        kase = 1;
      } else {
        kase = 2;
        q = qs;
      }
    }

    switch (kase) {
     case 1:
      ztest = e[0];
      e[0] = 0.0;
      qs = m;
      while (qs >= q + 1) {
        nrm = s[0];
        eml_xrotg(&nrm, &ztest, &rt, &sn);
        s[0] = nrm;
        eml_xrot(Vf, 1, (m << 1) + 1, rt, sn);
        qs = 0;
      }
      break;

     case 2:
      ztest = e[q - 1];
      e[q - 1] = 0.0;
      for (qs = q; qs + 1 <= m + 1; qs++) {
        eml_xrotg(&s[qs], &ztest, &rt, &sn);
        ztest = -sn * e[qs];
        e[qs] *= rt;
        eml_xrot(U, (qs << 1) + 1, ((q - 1) << 1) + 1, rt, sn);
      }
      break;

     case 3:
      varargin_1[0] = fabs(s[m]);
      varargin_1[1] = fabs(s[m - 1]);
      varargin_1[2] = fabs(e[m - 1]);
      varargin_1[3] = fabs(s[q]);
      varargin_1[4] = fabs(e[q]);
      kase = 1;
      sn = varargin_1[0];
      if (rtIsNaN(varargin_1[0])) {
        qs = 2;
        exitg1 = FALSE;
        while ((exitg1 == FALSE) && (qs < 6)) {
          kase = qs;
          if (!rtIsNaN(varargin_1[qs - 1])) {
            sn = varargin_1[qs - 1];
            exitg1 = TRUE;
          } else {
            qs++;
          }
        }
      }

      if (kase < 5) {
        while (kase + 1 < 6) {
          if (varargin_1[kase] > sn) {
            sn = varargin_1[kase];
          }

          kase++;
        }
      }

      rt = eml_div(s[m], sn);
      nrm = eml_div(s[m - 1], sn);
      ztest = eml_div(e[m - 1], sn);
      sqds = eml_div(s[q], sn);
      b = eml_div((nrm + rt) * (nrm - rt) + ztest * ztest, 2.0);
      nrm = rt * ztest;
      nrm *= nrm;
      ztest = 0.0;
      if ((b != 0.0) || (nrm != 0.0)) {
        ztest = sqrt(b * b + nrm);
        if (b < 0.0) {
          ztest = -ztest;
        }

        ztest = eml_div(nrm, b + ztest);
      }

      ztest += (sqds + rt) * (sqds - rt);
      nrm = sqds * eml_div(e[q], sn);
      while (q + 1 <= m) {
        eml_xrotg(&ztest, &nrm, &rt, &sn);
        nrm = rt * s[0];
        ztest = sn * e[0];
        e[0] = rt * e[0] - sn * s[0];
        b = s[1];
        s[1] *= rt;
        eml_xrot(Vf, 1, 3, rt, sn);
        s[0] = nrm + ztest;
        nrm = sn * b;
        eml_xrotg(&s[0], &nrm, &rt, &sn);
        ztest = rt * e[0] + sn * s[1];
        s[1] = -sn * e[0] + rt * s[1];
        nrm = sn * e[1];
        e[1] *= rt;
        eml_xrot(U, 1, 3, rt, sn);
        q = 1;
      }

      e[0] = ztest;
      iter++;
      break;

     default:
      if (s[q] < 0.0) {
        s[q] = -s[q];
        b_eml_xscal(-1.0, Vf, (q << 1) + 1);
      }

      while ((q + 1 < 2) && (s[0] < s[1])) {
        rt = s[0];
        s[0] = s[1];
        s[1] = rt;
        eml_xswap(Vf, 1, 3);
        eml_xswap(U, 1, 3);
        q = 1;
      }

      iter = 0;
      m--;
      break;
    }
  }

  for (qs = 0; qs < 2; qs++) {
    S[qs] = s[qs];
    for (kase = 0; kase < 2; kase++) {
      V[kase + (qs << 1)] = Vf[kase + (qs << 1)];
    }
  }
}

/*
 *
 */
static real_T eml_xnrm2(const real_T x[4])
{
  real_T y;
  real_T scale;
  int32_T k;
  real_T absxk;
  real_T t;
  y = 0.0;
  scale = 2.2250738585072014E-308;
  for (k = 0; k < 2; k++) {
    absxk = fabs(x[k]);
    if (absxk > scale) {
      t = scale / absxk;
      y = 1.0 + y * t * t;
      scale = absxk;
    } else {
      t = absxk / scale;
      y += t * t;
    }
  }

  return scale * sqrt(y);
}

/*
 *
 */
static void eml_xrot(real_T x[4], int32_T ix0, int32_T iy0, real_T c, real_T s)
{
  int32_T ix;
  int32_T iy;
  int32_T k;
  real_T y;
  real_T b_y;
  ix = ix0 - 1;
  iy = iy0 - 1;
  for (k = 0; k < 2; k++) {
    y = c * x[ix];
    b_y = s * x[iy];
    x[iy] = c * x[iy] - s * x[ix];
    x[ix] = y + b_y;
    iy++;
    ix++;
  }
}

/*
 *
 */
static void eml_xrotg(real_T *a, real_T *b, real_T *c, real_T *s)
{
  real_T roe;
  real_T absa;
  real_T absb;
  real_T scale;
  real_T ads;
  real_T bds;
  roe = *b;
  absa = fabs(*a);
  absb = fabs(*b);
  if (absa > absb) {
    roe = *a;
  }

  scale = absa + absb;
  if (scale == 0.0) {
    *s = 0.0;
    *c = 1.0;
    ads = 0.0;
    scale = 0.0;
  } else {
    ads = absa / scale;
    bds = absb / scale;
    ads = scale * sqrt(ads * ads + bds * bds);
    if (roe < 0.0) {
      ads = -ads;
    }

    *c = *a / ads;
    *s = *b / ads;
    if (absa > absb) {
      scale = *s;
    } else if (*c != 0.0) {
      scale = 1.0 / *c;
    } else {
      scale = 1.0;
    }
  }

  *a = ads;
  *b = scale;
}

/*
 *
 */
static void eml_xscal(real_T a, real_T x[4])
{
  int32_T k;
  for (k = 0; k < 2; k++) {
    x[k] *= a;
  }
}

/*
 *
 */
static void eml_xswap(real_T x[4], int32_T ix0, int32_T iy0)
{
  int32_T ix;
  int32_T iy;
  int32_T k;
  real_T temp;
  ix = ix0 - 1;
  iy = iy0 - 1;
  for (k = 0; k < 2; k++) {
    temp = x[ix];
    x[ix] = x[iy];
    x[iy] = temp;
    ix++;
    iy++;
  }
}

/*
 *
 */
void pinv(const real_T A[4], real_T X[4])
{
  int32_T k;
  real_T V[4];
  real_T s[2];
  real_T U[4];
  real_T S[4];
  int32_T r;
  int32_T vcol;
  int32_T br;
  real_T z;
  int32_T ic;
  int32_T ar;
  int32_T ib;
  int32_T ia;
  for (k = 0; k < 4; k++) {
    X[k] = 0.0;
  }

  eml_xgesvd(A, U, s, V);
  for (k = 0; k < 4; k++) {
    S[k] = 0.0;
  }

  for (k = 0; k < 2; k++) {
    S[k + (k << 1)] = s[k];
  }

  r = 0;
  k = 0;
  while ((k + 1 < 3) && (S[k + (k << 1)] > 2.0 * S[0] * 2.2204460492503131E-16))
  {
    r++;
    k++;
  }

  if (r > 0) {
    vcol = 0;
    for (br = 0; br + 1 <= r; br++) {
      z = 1.0 / S[br + (br << 1)];
      for (k = vcol; k + 1 <= vcol + 2; k++) {
        V[k] *= z;
      }

      vcol += 2;
    }

    for (vcol = 0; vcol < 4; vcol += 2) {
      for (ic = vcol; ic + 1 <= vcol + 2; ic++) {
        X[ic] = 0.0;
      }
    }

    br = -1;
    for (vcol = 0; vcol < 4; vcol += 2) {
      ar = 0;
      br++;
      k = (br + ((r - 1) << 1)) + 1;
      for (ib = br; ib + 1 <= k; ib += 2) {
        if (U[ib] != 0.0) {
          ia = ar;
          for (ic = vcol; ic + 1 <= vcol + 2; ic++) {
            ia++;
            X[ic] += U[ib] * V[ia - 1];
          }
        }

        ar += 2;
      }
    }
  }
}

/* End of code generation (pinv.c) */
