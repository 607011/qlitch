// Copyright (c) 2008-2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#include "mersenne_twister.h"
#include <QtCore/QDebug>
#include <QDateTime>

namespace MT {


    void MersenneTwister::seed(quint32 _Seed)
    {
        quint32 r = _Seed;
        quint32 s = 3402U;
        for (int i = 0; i < N; ++i) {
            r = 509845221U * r + 3U;
            s *= s + 1U;
            y[i] = s + (r >> 10);
        }
        index = 0;
        warmup();
    }


    void MersenneTwister::warmup(void)
    {
        const int N = 10000 + QDateTime::currentDateTime().time().msec();
        for (int i = 0; i < N; ++i)
            (*this)();
    }


    quint32 MersenneTwister::operator()()
    {
        if (index >= N) {
            quint32 h;
            for (int k = 0 ; k < N-M ; ++k) {
                h = (y[k] & HI) | (y[k+1] & LO);
                y[k] = y[k+M] ^ (h >> 1) ^ A[h & 1];
            }
            for (int k = N-M ; k < N-1 ; ++k) {
                h = (y[k] & HI) | (y[k+1] & LO);
                y[k] = y[k+(M-N)] ^ (h >> 1) ^ A[h & 1];
            }
            h = (y[N-1] & HI) | (y[0] & LO);
            y[N-1] = y[M-1] ^ (h >> 1) ^ A[h & 1];
            index = 0;
        }

        quint32 e = y[index++];
        e ^= (e >> 11);
        e ^= (e << 7) & 0x9d2c5680;
        e ^= (e << 15) & 0xefc60000;
        e ^= (e >> 18);
        return e;
    }


    const quint32 MersenneTwister::A[2] = { 0U, 0x9908b0dfU };
}
