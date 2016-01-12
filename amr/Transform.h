#ifndef TRANSFORM_H
#define TRANSFORM_H

class Transform {
public:
    // Discrete Fourier Transform
    void DFT1d(double *re, double* im, int N, int dir);

    // Fast Fourier Transform
    void FFT1d(double *re, double* im, int N, int dir);

private:
};


#endif // TRANSFORM_H
