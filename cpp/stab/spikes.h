#ifndef SPIKES_H
#define SPIKES_H

#include <vector>
#include <string>

#include "../aer/timestamp.h"

class Spikes
{
public:
    Spikes();

    void push(timestamp t);

    int length();

    inline timestamp operator()(const int i) {
        return spikes[i];
    }

    //json toJson();
    std::string toString();

private:
    std::vector<timestamp> spikes;
};

#endif // SPIKES_H
