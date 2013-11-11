// Copyright (c) 2008-2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#ifndef __MERSENNETWISTER_H_
#define __MERSENNETWISTER_H_

#include <QtGlobal>
#include "abstract_random_number_generator.h"

namespace MT {

    class MersenneTwister : public randomtools::UIntRandomNumberGenerator
    {
    public:
        MersenneTwister(void) {}
        quint32 operator()();
        inline quint32 next(void) { return (*this)(); }
        void seed(quint32 _Seed = 9U);

    private:
        static const int N = 624;
        static const int M = 397;
        static const quint32 LO = 0x7fffffffU;
        static const quint32 HI = 0x80000000U;
        static const quint32 A[2];
        quint32 y[N];
        int index;

    private: // methods
        void warmup(void);
    };
}

#endif //  __MERSENNETWISTER_H_
