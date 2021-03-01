#ifndef BUFFER_H
#define BUFFER_H

#include <vector>

#include "request.h"


namespace MoiseevaAS_ {
class Buffer {
public:

    int cursor = 0;
    int last = 0;

    Buffer();
    Buffer(int size);

    bool isFreeBuffer();
    bool isEmptyBuffer();
    int addNewReq(Request request);
    std::pair<Request, int> getReq();
    std::pair<Request, int> deleteReq();

private:

    int size;
    std::vector<Request *> buffers; //для пошагового

    };
}

#endif
