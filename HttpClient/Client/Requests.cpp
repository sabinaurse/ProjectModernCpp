#include "Requests.h"

namespace ClientRequests {
    cpr::Response CreatePlayer(const std::string& name, const std::string& password) {
        return cpr::Get(cpr::Url{ "http://localhost:18080/addPlayer/" + name + "/" + password });
    }

    cpr::Response GetPlayer(const std::string& name) {
        return cpr::Get(cpr::Url{ "http://localhost:18080/getPlayer/" + name });
    }
}
