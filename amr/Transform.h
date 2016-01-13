#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "complex.h"

class Transform {
public:
    static bool Forward(const complex *const Input, complex *const Output, const unsigned int N);
    static bool Forward(complex *const Data, const unsigned int N);

    static bool Inverse(const complex *const Input, complex *const Output, const unsigned int N, const bool Scale = true);
    static bool Inverse(complex *const Data, const unsigned int N, const bool Scale = true);

protected:
    static void Rearrange(const complex *const Input, complex *const Output, const unsigned int N);
    static void Rearrange(complex *const Data, const unsigned int N);

    static void Perform(complex *const Data, const unsigned int N, const bool Inverse = false);

    static void Scale(complex *const Data, const unsigned int N);
};


#endif // TRANSFORM_H
