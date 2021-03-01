#include "controller.h"

MoiseevaAS_::Controller::Controller() {

    this->alpha = 0;
    this->betta = 0;
    this->lambda = 0;
    this->sourcesAmount = 0;
    this->buffersAmount = 0;
    this->devicesAmount = 0;
    this->requestsNumber = 0;
}

std::list<std::string> MoiseevaAS_::Controller::work(Interpreter &interpreter) {

    //общая инициализация
    interpreter.init(); //чистим все предыдущ получ данные
    interpreter.config(sourcesAmount, buffersAmount, devicesAmount); //обнуляем массивы

    std::list<std::string> resultList;

    MoiseevaAS_::Source *sources = new MoiseevaAS_::Source(alpha, betta, sourcesAmount);
    MoiseevaAS_::Buffer *buffers = new MoiseevaAS_::Buffer(buffersAmount);
    MoiseevaAS_::Device *devices = new MoiseevaAS_::Device(devicesAmount, lambda);

    float currentTime = 0;

    //основной цикл программы, обработать все заявки
    for (int i = 0; i < requestsNumber; i++) {
        Request nextRequest = sources->getNextReq();
        currentTime += nextRequest.getWaitTime();
        nextRequest.setWaitTime(currentTime);

        std::list<std::pair<Request, int>> doneRequests = devices->readyDevices(currentTime);

        if (doneRequests.size() > 0) {
            for (auto it = doneRequests.begin(); it != doneRequests.end(); ++it) {
                resultList.push_back("Device №" + std::to_string((*it).second) + " became free in " + std::to_string((*it).first.getWaitTime()));
                interpreter.deleteRequestFromDevice((*it).first, (*it).second);

                //берем из буфера и передаем на девайс
                if (!buffers->isEmptyBuffer()) {
                    std::pair<Request, int> pair2 = buffers->getReq();
                    Request request2 = pair2.first;
                    interpreter.getRequestFromBuff(Request((*it).first.getWaitTime(),
                                                         request2.getSourceNum(),
                                                         request2.getRequestNum()), pair2.second);

                    int deviceNum2 = devices->addNewReq(std::max(request2.getWaitTime(), (*it).first.getWaitTime()), request2);
                    resultList.push_back("Request " + std::to_string(request2.getSourceNum()) + "." + std::to_string(request2.getRequestNum())
                                         + " downloaded in device №" + std::to_string(deviceNum2));
                    interpreter.addRequestToDevice(Request(std::max(request2.getWaitTime(), (*it).first.getWaitTime()),
                                                         request2.getSourceNum(),
                                                         request2.getRequestNum()), (*it).second);
                }
            }
        }

        std::string tmpResStr = "Generated request " + std::to_string(nextRequest.getSourceNum()) + "." + std::to_string(nextRequest.getRequestNum());
        interpreter.newRequestGenerate(Request(currentTime,
                                               nextRequest.getSourceNum(),
                                               nextRequest.getRequestNum()));
        //прибор занят
        if (!devices->isFreeDevice()) {

            //в буфере есть место
            if (buffers->isFreeBuffer()) {
                int bufferNum = buffers->addNewReq(nextRequest);
                tmpResStr += " and download in buffer №" + std::to_string(bufferNum + 1);
                interpreter.addRequestToBuff(Request(currentTime,
                                                   nextRequest.getSourceNum(),
                                                   nextRequest.getRequestNum()), bufferNum);

            //в буфере нет места
            } else {

                std::pair<Request, int> pair4 = buffers->deleteReq();
                Request deletedRequest = pair4.first;
                resultList.push_back("Request " + std::to_string(deletedRequest.getSourceNum()) + "."
                                     + std::to_string(deletedRequest.getRequestNum())
                                     + " deleted from buffer");
                interpreter.deleteRequestFromBuff(Request(currentTime,
                                                        deletedRequest.getSourceNum(),
                                                        deletedRequest.getRequestNum()), pair4.second);
                int bufferNum = buffers->addNewReq(nextRequest);
                tmpResStr += " and denied";
                interpreter.addRequestToBuff(Request(currentTime,
                                                   nextRequest.getSourceNum(),
                                                   nextRequest.getRequestNum()), bufferNum);
            }

        //прибор свободен
        } else {
            int deviceNum = devices->addNewReq(currentTime, nextRequest);
            tmpResStr += " and download in device №" + std::to_string(deviceNum);

            if (buffers->isFreeBuffer()) {
                int bufferNum = buffers->addNewReq(nextRequest);
                interpreter.addRequestToBuff(Request(currentTime,
                                                   nextRequest.getSourceNum(),
                                                   nextRequest.getRequestNum()), bufferNum);

                if (!buffers->isEmptyBuffer()) {
                    std::pair<Request, int> pair3 = buffers->getReq();
                    Request request2 = pair3.first;
                    interpreter.getRequestFromBuff(Request(currentTime,
                                                         request2.getSourceNum(),
                                                         request2.getRequestNum()), pair3.second);
                    interpreter.addRequestToDevice(Request(currentTime,
                                                         nextRequest.getSourceNum(),
                                                         nextRequest.getRequestNum()), deviceNum);
                }
            }
        }
        resultList.push_back(tmpResStr);
    }
    interpreter.commit(currentTime);
    return resultList;
}

void MoiseevaAS_::Controller::setAlpha(float value) { alpha = value; }

void MoiseevaAS_::Controller::setBetta(float value) { betta = value; }

void MoiseevaAS_::Controller::setLambda(float value) { lambda = value; }

void MoiseevaAS_::Controller::setDevicesAmount(int value) { devicesAmount = value; }

void MoiseevaAS_::Controller::setRequestsNumber(int value) { requestsNumber = value; }

void MoiseevaAS_::Controller::setBuffersAmount(int value) { buffersAmount = value; }

void MoiseevaAS_::Controller::setSourcesAmount(int value) { sourcesAmount = value; }

