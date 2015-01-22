/*
 * conv2.c
 *
 * Code generation for function 'conv2'
 *
 * C source code generated on: Wed Jan 21 21:50:20 2015
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "lucas_kanade_matlab.h"
#include "conv2.h"

/* Function Definitions */

/*
 *
 */
void conv2(const real_T arg1[10000], const real_T arg2[4], real_T c[10201])
{
  int32_T jc;
  int32_T ja1;
  int32_T ja2;
  int32_T ic;
  int32_T ia1;
  int32_T ia2;
  real_T s;
  int32_T i9;
  int32_T ja;
  int32_T b_ja;
  int32_T jb;
  int32_T i10;
  int32_T ia;
  int32_T b_ia;
  for (jc = 0; jc < 101; jc++) {
    if (2 < 2 + jc) {
      ja1 = jc;
    } else {
      ja1 = 1;
    }

    if (100 < 1 + jc) {
      ja2 = 99;
    } else {
      ja2 = jc;
    }

    for (ic = 0; ic < 101; ic++) {
      if (2 < 2 + ic) {
        ia1 = ic;
      } else {
        ia1 = 1;
      }

      if (100 < 1 + ic) {
        ia2 = 99;
      } else {
        ia2 = ic;
      }

      s = 0.0;
      i9 = (ja2 - ja1) + 1;
      for (ja = -1; ja + 1 <= i9; ja++) {
        b_ja = ja1 + ja;
        jb = jc - b_ja;
        i10 = (ia2 - ia1) + 1;
        for (ia = -1; ia + 1 <= i10; ia++) {
          b_ia = ia1 + ia;
          s += arg1[b_ia + 100 * b_ja] * arg2[(ic - b_ia) + (jb << 1)];
        }
      }

      c[ic + 101 * jc] = s;
    }
  }
}

/* End of code generation (conv2.c) */
