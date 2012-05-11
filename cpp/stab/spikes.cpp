#include "spikes.h"

#include <sstream>

Spikes::Spikes()
{
}

void Spikes::push(timestamp t)
{
    spikes.push_back(t);
}

int Spikes::length()
{
    return spikes.size();
}

std::string Spikes::toString()
{
    std::stringstream ss;

    ss << "[ ";

    for (timestamp t : spikes)
    {
        ss << t << " ";
    }

    ss << "]";

    return ss.str();
}
