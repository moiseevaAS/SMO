#include "interpreter.h"

MoiseevaAS_::Interpreter::Interpreter() { }

MoiseevaAS_::Interpreter::Archive::Archive() { }

MoiseevaAS_::Interpreter::Archive::Archive(std::vector<std::string> buffers, std::vector<std::string> devices,
                                            std::string message, float currentTime) {

    this->buffers = buffers;
    this->devices = devices;
    this->message = message;
    this->currentTime = currentTime;

}

void MoiseevaAS_::Interpreter::newRequestGenerate(MoiseevaAS_::Request request) {
    interpretation.push_back(std::make_pair(std::make_pair(request, -1), NEW_REQ));
}

void MoiseevaAS_::Interpreter::addRequestToBuff(MoiseevaAS_::Request request, int buff) {
    interpretation.push_back(std::make_pair(std::make_pair(request, buff), ADD_TO_BUFF));
}

void MoiseevaAS_::Interpreter::deleteRequestFromBuff(MoiseevaAS_::Request request, int buff) {
    interpretation.push_back(std::make_pair(std::make_pair(request, buff), DELETE_FROM_BUFF));
}

void MoiseevaAS_::Interpreter::getRequestFromBuff(MoiseevaAS_::Request request, int buff) {
    interpretation.push_back(std::make_pair(std::make_pair(request, buff), GET_FROM_BUFF));
}

void MoiseevaAS_::Interpreter::addRequestToDevice(MoiseevaAS_::Request request, int device) {
    interpretation.push_back(std::make_pair(std::make_pair(request, device), ADD_TO_DEVICE));
}

void MoiseevaAS_::Interpreter::deleteRequestFromDevice(MoiseevaAS_::Request request, int device) {
    interpretation.push_back(std::make_pair(std::make_pair(request, device), DELETE_FROM_DEVICE));
}

void MoiseevaAS_::Interpreter::init() {

    sourcesAmount = 0;
    buffersAmount = 0;
    devicesAmount = 0;
    interpretation.clear();
    processedReq.clear();
    deniedReq.clear();
    timeInSystem.clear();
    waitTime.clear();
    processTime.clear();
    dispTOW.clear();
    dispTOP.clear();
    pFailure.clear();
    deviceLoad.clear();
    steps.clear();
}

bool MoiseevaAS_::Interpreter::config(int sourcesAmount, int buffersAmount, int devicesAmount) {

    if (sourcesAmount > 0 && buffersAmount > 0 && devicesAmount > 0) {
        this->sourcesAmount = sourcesAmount;
        this->buffersAmount = buffersAmount;
        this->devicesAmount = devicesAmount;

        for (int i = 0; i < sourcesAmount; i++) {

            processedReq.push_back(0);
            deniedReq.push_back(0);
            timeInSystem.push_back(0);
            waitTime.push_back(0);
            processTime.push_back(0);
            dispTOW.push_back(0);
            dispTOP.push_back(0);
            pFailure.push_back(0);
        }

        for (int i = 0; i < devicesAmount; i++) {
            deviceLoad.push_back(0);
        }
        return true;
    } else
        return false;
}

void MoiseevaAS_::Interpreter::commit(float allWorkTime) {

    //объявление переменных
    std::map<std::string, std::pair<float, float>> tmpTimeOfWait;
    std::map<std::string, std::pair<float, float>> tmpTimeOfProcess;
    std::map<std::string, std::pair<float, float>> tmpTimeOfDeviceWork;

    std::vector<std::string> buffersVec;
    std::vector<std::string> devicesVec;
    std::string tmpMessage = "";

    //инициализация
    for (int i = 0; i < buffersAmount; i++) {
        buffersVec.push_back("null");
    }

    for (int i = 0; i < devicesAmount; i++) {
        devicesVec.push_back("null");
    }

    steps.push_back(Archive(buffersVec, devicesVec, tmpMessage, 0));

    //основной цикл событий
    for (auto it = interpretation.begin(); it!= interpretation.end(); ++it) {
        std::pair<Request, int> reqPair = (*it).first;

        switch ((*it).second) {

            case NEW_REQ: {
                tmpMessage += "Create request " +
                               std::to_string(reqPair.first.getSourceNum()) + "." +
                               std::to_string(reqPair.first.getRequestNum());
            }
                break;


            case ADD_TO_BUFF: {
                std::string tmpKey = std::to_string(reqPair.first.getSourceNum()) + "." +
                                     std::to_string(reqPair.first.getRequestNum());

                tmpTimeOfWait.insert(std::make_pair(tmpKey, std::make_pair(reqPair.first.getWaitTime(), 0)));

                for (size_t i = 0; i < buffersVec.size(); i ++) {
                    if (i == (reqPair.second)) {
                        buffersVec.at(i) = std::to_string(reqPair.first.getSourceNum()) + "." +
                                           std::to_string(reqPair.first.getRequestNum());
                    }
                }
                tmpMessage += " and load in buffer " + std::to_string(reqPair.second + 1);
                steps.push_back(Archive(buffersVec, devicesVec, tmpMessage, reqPair.first.getWaitTime()));
                tmpMessage.clear();
            }
                break;


            case DELETE_FROM_BUFF: {
                std::string tmpKey = std::to_string(reqPair.first.getSourceNum()) + "." +
                                     std::to_string(reqPair.first.getRequestNum());

                auto tmpIt = tmpTimeOfWait.find(tmpKey);
                if (tmpIt != tmpTimeOfWait.end()) {
                    tmpIt->second.second = reqPair.first.getWaitTime();
                }

                *(deniedReq.begin() + reqPair.first.getSourceNum() - 1) = (*(deniedReq.begin() + reqPair.first.getSourceNum() - 1)) + 1;

                for (size_t i = 0; i < buffersVec.size(); i ++) {
                    if (i == (reqPair.second)) {
                        buffersVec.at(i) = "null";
                    }
                }

                tmpMessage += ", request " + std::to_string(reqPair.first.getSourceNum()) + "." +
                                             std::to_string(reqPair.first.getRequestNum()) +
                                             " removed from buffer " +
                                             std::to_string(reqPair.second + 1);
                steps.push_back(Archive(buffersVec, devicesVec, tmpMessage, reqPair.first.getWaitTime()));
                tmpMessage.clear();
            }
                break;


            case GET_FROM_BUFF: {
                std::string tmpKey = std::to_string(reqPair.first.getSourceNum()) + "." +
                                     std::to_string(reqPair.first.getRequestNum());
                auto tmpIt = tmpTimeOfWait.find(tmpKey);
                if (tmpIt != tmpTimeOfWait.end()) {
                    tmpIt->second.second = reqPair.first.getWaitTime();
                }

                for (size_t i = 0; i < buffersVec.size(); i ++) {
                    if (i == (reqPair.second)) {
                        buffersVec.at(i) = "null";
                    }
                }
            }
                break;


            case ADD_TO_DEVICE: {
                std::string tmpKey = std::to_string(reqPair.first.getSourceNum()) + "." +
                                     std::to_string(reqPair.first.getRequestNum());

                tmpTimeOfDeviceWork.insert(std::make_pair(std::to_string(reqPair.second) + '.' + tmpKey,
                                           std::make_pair(reqPair.first.getWaitTime(), 0)));

                tmpTimeOfProcess.insert(std::make_pair(tmpKey,
                                        std::make_pair(reqPair.first.getWaitTime(), 0)));

                *(processedReq.begin() + reqPair.first.getSourceNum() - 1) = (*(processedReq.begin() + reqPair.first.getSourceNum() - 1)) + 1;

                for (size_t i = 0; i < devicesVec.size(); i++) {
                    if (i == (reqPair.second - 1)) {
                        devicesVec.at(i) = std::to_string(reqPair.first.getSourceNum()) + "." +
                                           std::to_string(reqPair.first.getRequestNum());
                    }
                }
                tmpMessage += "Device " + std::to_string(reqPair.second) + " load request " +
                                          std::to_string(reqPair.first.getSourceNum()) + "." +
                                          std::to_string(reqPair.first.getRequestNum());

                steps.push_back(Archive(buffersVec, devicesVec, tmpMessage, reqPair.first.getWaitTime()));
                tmpMessage.clear();
            }
                break;


            case DELETE_FROM_DEVICE: {
                std::string tmpKey = std::to_string(reqPair.first.getSourceNum()) + "." +
                                     std::to_string(reqPair.first.getRequestNum());

                auto tmpIt = tmpTimeOfDeviceWork.find(std::to_string(reqPair.second) + '.' + tmpKey);
                if (tmpIt != tmpTimeOfDeviceWork.end()) {
                    tmpIt->second.second = reqPair.first.getWaitTime();
                }

                auto tmpIt2 = tmpTimeOfProcess.find(tmpKey);
                if (tmpIt2 != tmpTimeOfProcess.end()) {
                    tmpIt2->second.second = reqPair.first.getWaitTime();
                }

                for (size_t i = 0; i < devicesVec.size(); i ++) {
                    if (i == (reqPair.second - 1)) {
                        devicesVec.at(i) = "null";
                    }
                }
                tmpMessage.clear();
            }
                break;
        }
    }

    // TIME
    std::vector<float> totalTOW; //wait
    std::vector<float> totalTOP; //process
    std::vector<float> totalDevWork;

    //инициализируем массивы всего времени ожидания и работы
    for (int i = 0; i < sourcesAmount; i++) {
        totalTOW.push_back(0);
        totalTOP.push_back(0);
    }

    //время работы приборов
    for (int i = 0; i < devicesAmount; i++) {
        totalDevWork.push_back(0);
    }

    //считаем время работы приборов
    for (auto it = tmpTimeOfDeviceWork.begin(); it != tmpTimeOfDeviceWork.end(); ++it) {
        std::string indexStr = "";
        for (char z: it->first) {
            if (z == '.') {
                break;
            } else {
                indexStr.push_back(z);
            }
        }
        int index = std::stoi(indexStr); //преобр строки в цел число
        std::pair<float, float> pairDevWork = it->second;
        if (pairDevWork.second == 0)
            continue;

        totalDevWork.at(index - 1) += std::max(pairDevWork.first, pairDevWork.second)
                - std::min(pairDevWork.first, pairDevWork.second);
    }

    //время ожидания
    for (auto it = tmpTimeOfWait.begin(); it != tmpTimeOfWait.end(); ++it) {
        std::string indexStr = "";
        for (char z: it->first) {
            if (z == '.') {
                break;
            } else {
                indexStr.push_back(z);
            }
        }
        int index = std::stoi(indexStr);
        std::pair<float, float> pairTOW = it->second;
        if (pairTOW.second == 0)
            continue;
        totalTOW.at(index - 1) += std::max(pairTOW.first, pairTOW.second) - std::min(pairTOW.first, pairTOW.second);
    }

    //время процесса
    for (auto it = tmpTimeOfProcess.begin(); it != tmpTimeOfProcess.end(); ++it) {
        std::string indexStr = "";
        for (char z: it->first) {
            if (z == '.') {
                break;
            } else {
                indexStr.push_back(z);
            }
        }
        int index = std::stoi(indexStr);
        std::pair<float, float> pairTOP = it->second;
        if (pairTOP.second == 0)
            continue;
        totalTOP.at(index - 1) += std::max(pairTOP.first, pairTOP.second) - std::min(pairTOP.first, pairTOP.second);
    }


    for (int i =0; i < deviceLoad.size(); i++) {
        deviceLoad.at(i) = (totalDevWork.at(i)/allWorkTime)*100;
    }

    for (int i =0; i < waitTime.size(); i++) {
        waitTime.at(i) = totalTOW.at(i)/tmpTimeOfWait.size();
    }

    for (int i =0; i < processTime.size(); i++) {
        processTime.at(i) = totalTOP.at(i)/tmpTimeOfProcess.size();
    }

    for (int i = 0; i < timeInSystem.size(); i++) {
        timeInSystem.at(i) = waitTime.at(i) + processTime.at(i);
    }

    for (int i = 0; i < pFailure.size(); i++) {
        float a = deniedReq.at(i);
        float b = processedReq.at(i);
        pFailure.at(i) = (a/(a + b))*100;
    }

    for (int i = 0; i < dispTOP.size(); i++) {
        dispTOP.at(i) = (processTime.at(i)/timeInSystem.at(i));
    }

    for (int i = 0; i < dispTOW.size(); i++) {
        dispTOW.at(i) = (waitTime.at(i)/timeInSystem.at(i));
    }
}

std::vector<MoiseevaAS_::Interpreter::Archive> MoiseevaAS_::Interpreter::getSteps() { return steps; }

int MoiseevaAS_::Interpreter::getLinesAmount() { return sourcesAmount + buffersAmount + devicesAmount; }

int MoiseevaAS_::Interpreter::getDevicesAmount() { return devicesAmount; }

int MoiseevaAS_::Interpreter::getBuffersAmount() { return buffersAmount; }

int MoiseevaAS_::Interpreter::getSourcesAmount() { return sourcesAmount; }

std::vector<float> MoiseevaAS_::Interpreter::getDeviceLoad() { return deviceLoad; }

std::vector<float> MoiseevaAS_::Interpreter::getPFailure() { return pFailure; }

std::vector<float> MoiseevaAS_::Interpreter::getDispTOP() { return dispTOP; }

std::vector<float> MoiseevaAS_::Interpreter::getDispTOW()  { return dispTOW; }

std::vector<float> MoiseevaAS_::Interpreter::getProcessTime() { return processTime; }

std::vector<float> MoiseevaAS_::Interpreter::getWaitTime() { return waitTime; }

std::vector<float> MoiseevaAS_::Interpreter::getTimeInSystem() { return timeInSystem; }

std::vector<int> MoiseevaAS_::Interpreter::getDeniedReq() { return deniedReq; }

std::vector<int> MoiseevaAS_::Interpreter::getProcessedReq() { return processedReq; }
