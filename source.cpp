#include <QApplication>

#include "source.h"

MoiseevaAS_::Source::Source() {

    this->alpha = 0;
    this->betta = 0;
    this->amountOfSources = 0;

    this->waitTime = new float[this->amountOfSources];
    this->requestNums = new int[this->amountOfSources];
}

MoiseevaAS_::Source::Source(float alpha, float betta, int amountOfSources) {  //float lambda

    this->alpha = alpha;
    this->betta = betta;
    this->amountOfSources = amountOfSources;

    this->waitTime = new float[amountOfSources];
    this->requestNums = new int[amountOfSources];

    for (int i = 0; i < amountOfSources; i++) {
        this->requestNums[i] = 0;
        this->waitTime[i] = -1;
    }
}

MoiseevaAS_::Request MoiseevaAS_::Source::getNextReq() {

    for (int i = 0; i < amountOfSources; i++) {
        if (waitTime[i] <= 0) {
            requestNums[i]++;
            waitTime[i] = ((float)qrand()/(float)RAND_MAX)*(betta - alpha) + alpha; //равномерное распр
        }
    }

    std::pair<int, float> min = std::make_pair(0, waitTime[0]);
    for (int i = 0; i < amountOfSources; i++) {
        if (waitTime[i] < min.second) {
            min = std::make_pair(i, waitTime[i]);
        }
    }

    for (int i = 0; i < amountOfSources; i++) {
        waitTime[i] -= min.second;
    }

    return Request(min.second, min.first + 1, requestNums[min.first]);
}




