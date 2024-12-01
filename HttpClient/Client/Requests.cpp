#include "Requests.h"

namespace ClientRequests {
    cpr::Response CreatePlayer(const std::string& name) {
        return cpr::Get(cpr::Url{ "http://localhost:18080/addPlayer/" + name});
    }

    cpr::Response GetPlayer(const std::string& name) {
        return cpr::Get(cpr::Url{ "http://localhost:18080/getPlayer/" + name });
    }

    cpr::Response DeletePlayer(const std::string& name) {
        return cpr::Delete(cpr::Url{ "http://localhost:18080/deletePlayer/" + name });
    }

    cpr::Response UpdatePlayer(const std::string& name, int newScore, int newPoints) {
        return cpr::Post(
            cpr::Url{ "http://localhost:18080/updatePlayer" },
            cpr::Header{ {"Content-Type", "application/json"} },
            cpr::Body{ R"({"name":")" + name + R"(", "score":)" + std::to_string(newScore) + R"(, "points":)" + std::to_string(newPoints) + R"(})" }
        );
    }

    cpr::Response GetMap() {
        return cpr::Get(cpr::Url{ "http://localhost:18080/getMap" });
    }

    cpr::Response StartGame()
    {
        return cpr::Post(cpr::Url{ "http://localhost:18080/startGame" });
    }
    cpr::Response ResetGame()
    {
        return cpr::Post(cpr::Url{ "http://localhost:18080/resetGame" });
    }
}
