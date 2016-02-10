#include <math.h>
#include <iostream>

#include "transform.h"

bool Transform::Forward(const complex *const Input, complex *const Output, const unsigned int N)
{
    if (!Input || !Output || N < 1 || N & (N - 1))
        return false;

    Rearrange(Input, Output, N);
    Perform(Output, N);

    return true;
}

bool Transform::Forward(complex *const Data, const unsigned int N)
{
    if (!Data || N < 1 || N & (N - 1))
        return false;

    Rearrange(Data, N);
    Perform(Data, N);

    return true;
}


bool Transform::Inverse(const complex *const Input, complex *const Output, const unsigned int N, const bool Scale /* = true */)
{
    if (!Input || !Output || N < 1 || N & (N - 1))
        return false;

    Rearrange(Input, Output, N);
    Perform(Output, N, true);

    if (Scale)
        Transform::Scale(Output, N);

    return true;
}


bool Transform::Inverse(complex *const Data, const unsigned int N, const bool Scale /* = true */)
{
    if (!Data || N < 1 || N & (N - 1))
        return false;

    Rearrange(Data, N);
    Perform(Data, N, true);

    if (Scale)
        Transform::Scale(Data, N);

    return true;
}

void Transform::Rearrange(const complex *const Input, complex *const Output, const unsigned int N)
{
    unsigned int Target = 0;

    for (unsigned int Position = 0; Position < N; ++Position)
    {
        Output[Target] = Input[Position];

        unsigned int Mask = N;

        while (Target & (Mask >>= 1))
            Target &= ~Mask;

        Target |= Mask;
    }
}

void Transform::Rearrange(complex *const Data, const unsigned int N)
{
    unsigned int Target = 0;

    for (unsigned int Position = 0; Position < N; ++Position)
    {
        if (Target > Position)
        {
            const complex Temp(Data[Target]);
            Data[Target] = Data[Position];
            Data[Position] = Temp;
        }

        unsigned int Mask = N;

        while (Target & (Mask >>= 1))
            Target &= ~Mask;

        Target |= Mask;
    }
}

void Transform::Perform(complex *const Data, const unsigned int N, const bool Inverse /* = false */)
{
    const double pi = Inverse ? 3.14159265358979323846 : -3.14159265358979323846;

    for (unsigned int Step = 1; Step < N; Step <<= 1)
    {
        const unsigned int Jump = Step << 1;
        const double delta = pi / double(Step);
        const double Sine = sin(delta * .5);
        const complex Multiplier(-2. * Sine * Sine, sin(delta));
        complex Factor(1.);

        for (unsigned int Group = 0; Group < Step; ++Group)
        {
            for (unsigned int Pair = Group; Pair < N; Pair += Jump)
            {
                const unsigned int Match = Pair + Step;
                const complex Product(Factor * Data[Match]);
                Data[Match] = Data[Pair] - Product;
                Data[Pair] += Product;
            }
            Factor = Multiplier * Factor + Factor;
        }
    }
}

void Transform::Scale(complex *const Data, const unsigned int N)
{
    const double Factor = 1. / double(N);
    for (unsigned int Position = 0; Position < N; ++Position)
        Data[Position] *= Factor;
}
