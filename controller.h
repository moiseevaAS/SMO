#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <list>
#include <string>

#include "source.h"
#include "buffer.h"
#include "device.h"
#include "interpreter.h"

namespace MoiseevaAS_ {
class Controller {
public:

    Controller();
    std::list<std::string> work(Interpreter &interpreter);

    void setAlpha(float value);
    void setBetta(float value);
    void setLambda(float value);
    void setSourcesAmount(int value);
    void setBuffersAmount(int value);
    void setDevicesAmount(int value);
    void setRequestsNumber(int value);

private:

    float alpha, betta, lambda;
    int sourcesAmount, buffersAmount, devicesAmount, requestsNumber;

    };
}

#endif
