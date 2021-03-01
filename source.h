#ifndef SOURCE_H
#define SOURCE_H

#include <cmath>

#include "request.h"

namespace MoiseevaAS_ {
class Source {
public:

    Source();
    Source(float alpha, float betta, int amountOfSources);

    MoiseevaAS_::Request getNextReq();

private:

    float alpha, betta, *waitTime;
    int amountOfSources, *requestNums;

    };
}

#endif
