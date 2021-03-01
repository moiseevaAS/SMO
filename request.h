#ifndef REQUEST_H
#define REQUEST_H

namespace MoiseevaAS_ {
class Request {
public:

    Request();
    Request(float waitTime, int sourceId, int requestNumber);

    int getSourceNum() const;
    int getRequestNum() const;
    float getWaitTime() const;

    void setWaitTime(float value);

private:

    int sourceNum, requestNum;
    float waitTime;

    };
}

#endif
