#ifndef DEVICE_H
#define DEVICE_H

#include <list>
#include <vector>
#include <utility>

#include "request.h"

namespace MoiseevaAS_ {
class Device {
public:

     int cursorDev = 0;

    Device();
    Device(int size, float lambda);

    bool isFreeDevice();
    int addNewReq(float currentTime, Request request);
    std::list<std::pair<Request, int>> readyDevices(float currentTime);

private:

    int size;
    float lambda;
    std::vector<std::pair<float, Request> *> devices; //для пошагового

    };
}

#endif
