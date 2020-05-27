#ifndef _FFT_COOLY_TUKEY_h
#define _FFT_COOLY_TUKEY_h

#include <iostream>
#include <complex>
#define MAX 200

class DSP {
  static int log2(int N);
  static int check(int n);
  static int reverse(int N, int n);
  static void sort(std::complex<double>* f1, int N);
  static void transform(std::complex<double>* f, int N);
  static void FFT(std::complex<double>* f, int N, double d);
};

#endif
