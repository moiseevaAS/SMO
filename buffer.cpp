#include "buffer.h"

MoiseevaAS_::Buffer::Buffer() {

    this->size = 0;
}

MoiseevaAS_::Buffer::Buffer(int size) {

    this->size = size;
    for (int i = 0; i < size; i++) {
        buffers.push_back(nullptr);
    }
}

bool MoiseevaAS_::Buffer::isFreeBuffer() {

    bool res = false;
    for (int i = 0; i < buffers.size(); i++) {
        if (buffers.at(i) == nullptr)
            res = true;
    }
    return res;
}

bool MoiseevaAS_::Buffer::isEmptyBuffer() {

    bool res = true;
    for (int i = 0; i < buffers.size(); i++) {
        if (buffers.at(i) != nullptr)
            res = false;
    }
    return res;
}

int MoiseevaAS_::Buffer::addNewReq(MoiseevaAS_::Request req) {

    for (int i = 0; i < buffers.size(); i++) {


        if (buffers.at(i) == nullptr) {
            buffers.at(i) = new Request(req.getWaitTime(),
                                             req.getSourceNum(),
                                             req.getRequestNum()); 
            return i;
        }

    }
    return -1;
}

std::pair<MoiseevaAS_::Request, int> MoiseevaAS_::Buffer::deleteReq() {
 int max=0;
    for (int i = 0; i < buffers.size()-1; i++) {
        if  (buffers.at(i)->getWaitTime() > buffers.at(i+1)->getWaitTime()) {
           max=i;
        } else max = i+1;
    }
    Request req = *buffers.at(max);
    buffers.at(max) = nullptr;
    return std::make_pair(req, max);
    return std::make_pair(Request(), -1);
}

std::pair<MoiseevaAS_::Request, int> MoiseevaAS_::Buffer::getReq() {

    std::size_t reqToGet = 0;
    bool selected = false;

    for (std::size_t i = 0; i < buffers.size(); i++) {

        if (cursor >= buffers.size() || cursor < 0) {
            cursor = 0;
        }
       //если буфер под курсором не пустой то берем его и курсор ставим на след
        if (buffers.at(cursor) != nullptr) {
             if (!selected) {
                 reqToGet = cursor;
                 selected = true;
                 cursor++;
               }
            } else cursor++;


            }

    Request req = *buffers.at(reqToGet);
    buffers.at(reqToGet) = nullptr;
    if (selected)
        return std::make_pair(req, reqToGet);
    else
        return std::make_pair(MoiseevaAS_::Request(), -1);
}

