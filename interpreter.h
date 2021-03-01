#ifndef ANALYTICS_H
#define ANALYTICS_H

#include <set>
#include <map>
#include <vector>
#include <string>

#include "request.h"

namespace MoiseevaAS_ {
class Interpreter {
public:
    class Archive {
    public:

        Archive();
        Archive(std::vector<std::string> buffers, std::vector<std::string> devices,
                  std::string message, float currentTime);

        float currentTime;
        const std::string EMPTY = "null";
        std::string message;
        std::vector<std::string> buffers, devices;

    };

    Interpreter();

    void newRequestGenerate(Request request);
    void addRequestToBuff(Request request, int buff);
    void deleteRequestFromBuff(Request request, int buff);
    void getRequestFromBuff(Request request, int buff);
    void addRequestToDevice(Request request, int device);
    void deleteRequestFromDevice(Request request, int device);

    void init();
    bool config(int sourcesAmount, int buffersAmount, int devicesAmount);
    void commit(float allWorkTime);

    std::vector<int>    getProcessedReq();
    std::vector<int>    getDeniedReq();
    std::vector<float>  getTimeInSystem();
    std::vector<float>  getWaitTime();
    std::vector<float>  getProcessTime();
    std::vector<float>  getDispTOW();
    std::vector<float>  getDispTOP();
    std::vector<float>  getPFailure();
    std::vector<float>  getDeviceLoad();

    std::vector<Archive> getSteps();
    int getLinesAmount();

    int getSourcesAmount();
    int getBuffersAmount();
    int getDevicesAmount();

private:

    enum Event {

        NEW_REQ,
        ADD_TO_BUFF,
        DELETE_FROM_BUFF,
        GET_FROM_BUFF,
        ADD_TO_DEVICE,
        DELETE_FROM_DEVICE,

    };

    int sourcesAmount, buffersAmount, devicesAmount;

    std::vector<std::pair<std::pair<Request, int>, Event>> interpretation;

    // AUTO
    std::vector<int>    processedReq, deniedReq;
    std::vector<float>  timeInSystem, waitTime, processTime,
                        dispTOW, dispTOP, pFailure, deviceLoad;

    // STEP

    std::vector<Archive> steps;

    };
}

#endif
