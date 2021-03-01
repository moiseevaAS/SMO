#include "request.h"

MoiseevaAS_::Request::Request() {

    this->waitTime = 0.0;
    this->sourceNum = 0;
    this->requestNum = 0;
}

MoiseevaAS_::Request::Request(float waitTime, int sourceNum, int requestNum) {

    this->waitTime = waitTime;
    this->sourceNum = sourceNum;
    this->requestNum = requestNum;
}

float MoiseevaAS_::Request::getWaitTime() const { return waitTime; }

int MoiseevaAS_::Request::getSourceNum() const { return sourceNum; }

int MoiseevaAS_::Request::getRequestNum() const { return requestNum; }

void MoiseevaAS_::Request::setWaitTime(float value) { waitTime = value; }
