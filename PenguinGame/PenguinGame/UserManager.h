#pragma once
#include <iostream>
#include <unordered_map>

class UserManager {
private:
    std::unordered_map<std::string, std::string> users;  // Stocăm numele utilizatorului și parola

public:
    void addUser(const std::string& username, const std::string& password) {
        users[username] = password;
    }

    bool login(const std::string& username, const std::string& password) {
        if (users.find(username) != users.end() && users[username] == password) {
            return true;
        }
        return false;
    }
};


