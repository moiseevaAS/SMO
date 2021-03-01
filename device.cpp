#include <math.h>
#include <algorithm>
#include <QApplication>

#include "device.h"

MoiseevaAS_::Device::Device() {

    this->size = 0;
    this->lambda = 0;
}

MoiseevaAS_::Device::Device(int size, float lambda) {

    this->size = size;
    this->lambda = lambda;

    for (int i = 0; i < size; i++) {
        devices.push_back(nullptr);
    }
}

bool MoiseevaAS_::Device::isFreeDevice() {

    bool res = false;
    for (int i = 0; i < devices.size(); i++) {
        if (devices.at(i) == nullptr)
            res = true;
    }
    return res;
}

//в этом методе добавить чтобы он начинал с указателя
int MoiseevaAS_::Device::addNewReq(float currentTime, MoiseevaAS_::Request req) {

    float workTime = currentTime + lambda * exp(((float)qrand()/(float)RAND_MAX) * -lambda);
            //(-1/lambda)*std::log(((float)qrand()/(float)RAND_MAX)); // пуасс распред

    for (int i = 0; i < devices.size(); i++) {

        if (cursorDev >= devices.size() || cursorDev < 0) {
            cursorDev = 0;
        }

        if (devices.at(cursorDev) == nullptr) {
            devices.at(cursorDev) = new std::pair<float, Request>(workTime, Request(req.getWaitTime(),
                                                                            req.getSourceNum(),
                                                                            req.getRequestNum()));
            cursorDev++;
            return cursorDev;
        }
        cursorDev++;
    }
    return -1;
}

std::list<std::pair<MoiseevaAS_::Request, int>> MoiseevaAS_::Device::readyDevices(float currentTime) {

    std::list<std::pair<Request, int>> tmpList;
    std::vector<std::pair<float, Request> *> tmpDev;

    for (int i = 0; i < devices.size(); i++) {
        if (devices.at(i) != nullptr) {
            if ((*devices.at(i)).first < currentTime) {
                tmpList.push_back(std::make_pair(Request((*devices.at(i)).first,
                                                         (*devices.at(i)).second.getSourceNum(),
                                                         (*devices.at(i)).second.getRequestNum()), i + 1));
                tmpDev.push_back(nullptr);
            } else
                tmpDev.push_back(devices.at(i));
        } else
            tmpDev.push_back(nullptr);
    }
    devices = tmpDev;
    return tmpList;
}
