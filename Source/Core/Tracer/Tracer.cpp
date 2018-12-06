/*
  ==============================================================================

    Tracer.cpp
    Created: 6 Dec 2018 3:04:53pm
    Author:  bowen

  ==============================================================================
*/

#include "Tracer.h"

std::shared_ptr<Tracer> Tracer::parse(StringArray& cmd, std::unordered_map<String, std::shared_ptr<void>>& env)
{
    if (cmd[1] == "RayCast")
        return std::make_shared<RayCast>(RayCast());
    else if (cmd[1] == "Whitted")
        return std::make_shared<Whitted>(Whitted(std::stoi(cmd[2].toStdString())));
    else if (cmd[1] == "PathTrace")
        return std::make_shared<PathTrace>(PathTrace(std::stoi(cmd[2].toStdString())));
    else if (cmd[1] == "PhotonMapTrace")
        return std::make_shared<PhotonMapTrace>(PhotonMapTrace(std::stoi(cmd[2].toStdString())));
    else
        throw std::invalid_argument("[TRACER] Type not found.");
}
