#pragma once
#include <string>
#include <cpr/cpr.h>

namespace ClientRequests {
    cpr::Response CreatePlayer(const std::string& name);
    cpr::Response GetPlayer(const std::string& name);
    cpr::Response DeletePlayer(const std::string& name);
    cpr::Response UpdatePlayer(const std::string& name, int newScore, int newPoints);
    cpr::Response GetMap();
    cpr::Response StartGame();
    cpr::Response ResetGame();
}
