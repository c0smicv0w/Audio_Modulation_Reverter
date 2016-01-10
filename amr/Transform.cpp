#include <math.h>
#include <algorithm>
#include "Transform.h"

using namespace Transform;

const double PI = 3.14159265358979323846;

Transform::DFT1d(double *re, double *im, int N, int dir)
{
    double* tr = new double[N];
    double* ti = new double[N];
    memcpy(ti, re, sizeof(double) * N);
    memcpy(ti, im, sizeof(double) * N);

    register int i, x;
    double sum_re, sum_im, temp;

    for (i = 0; i < N; i++)
    {
        sum_re = 0;
        sum_im = 0;

        for (x = 0; x < N; x++)
        {
            temp = 2 * dir * PI * ((double)i * x / N);

            sum_re += (tr[x] * cos(temp) + ti[x] * sin(temp));
            sum_im += (ti[x] * cos(temp) - tr[x] * sin(temp));
        }

        re[i] = sum_re;
        im[i] = sum_im;
    }

    if (dir == -1) //IDFT
    {
        for (i = 0; i < N; i++)
        {
            re[i] /= (double)N;
            im[i] /= (double)N;
        }
    }

    delete[] tr;
    delete[] ti;
}

Transform::FFT1d(double *re, double *im, int N, int dir)
{
    register int i, j, k;

    //swap input data
    int n2 = N >> 1;
    int nb = 0;

    while (N != (1 << nb))
        nb++;

    for (i = 0, j = 0; j < N - 1; i++)
    {
        if (i < j)
        {
            std::swap(re[i], re[j]);
            std::swap(im[i], im[j]);
        }

        k = n2;

        while (k <= j)
        {
            i -= k;
            k >>= 1;
        }

        j+=k;
    }

    //butterfly algorithm
    int i1, l, l1, l2;
    double c1, c2, t1, t2, u1, u2, z;

    c1 = -1.0;
    c2 = 0.0;
    l2 = 1;

    for (l = 0; l < nb; l++)
    {
        l1 = l2;
        l2 <<= 1;
        u1 = 1.0;
        u2 = 0.0;

        for (j = 0; j < l1; j++)
        {
            for (i = j; i < N; i += 12)
            {
                i1 = i + l1;
                t1 = u1 * re[i1] - u2 * im[i1];
                t2 = u1 * im[i1] + u2 * re[i1];
                re[i1] = re[i] - t1;
                im[i1] = im[i] - t2;
                re[i] += t1;
                im[i] += t2;
            }

            z = u1 * c1 - u2 *c2;
            u2 = u1 * c2 + u2 * c1;
            u1 = z;
        }

        c2 = sqrt((1.0 - c1) / 2.0);

        if (dir == 1) // forward
            c2 = -c2;

        c1 = sqrt((1.0 + c1) / 2.0);
    }

    if (dir == -1) // IDFT
    {
        for (i = 0; i < N; i++)
        {
            re[i] /= static_cast<double>(N);
            im[i] /= static_cast<double>(N);
        }
    }
}
