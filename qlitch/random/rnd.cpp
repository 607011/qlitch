#include "rnd.h"

#include <QDateTime>

MT::MersenneTwister rng;

namespace RAND {

void initialize(void)
{
    rng.seed(QDateTime::currentDateTime().toMSecsSinceEpoch());
}

}
