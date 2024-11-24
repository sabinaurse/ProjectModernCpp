#pragma once
#include <string>
#include <cpr/cpr.h>

namespace ClientRequests {
    cpr::Response CreatePlayer(const std::string& name, const std::string& password);
    cpr::Response GetPlayer(const std::string& name);
}
