#include <iostream>
#include "Requests.h"
#include "State.h"

int main() {
    ClientState state;
    int choice;

    do {
        std::cout << "1. Create Player\n";
        std::cout << "2. Get Player Details\n";
        std::cout << "3. Exit\n";
        std::cout << "Choose an option: ";
        std::cin >> choice;

        if (choice == 1) {
            std::string name, password;
            std::cout << "Enter player name: ";
            std::cin >> name;
            std::cout << "Enter player password: ";
            std::cin >> password;

            auto response = ClientRequests::CreatePlayer(name, password);
            if (response.status_code == 200) {
                std::cout << "Player created successfully!\n";
                state.SetCurrentPlayer(name);
            }
            else {
                std::cout << "Error creating player: " << response.text << "\n";
            }
        }
        else if (choice == 2) {
            std::string name;
            std::cout << "Enter player name: ";
            std::cin >> name;

            auto response = ClientRequests::GetPlayer(name);
            if (response.status_code == 200) {
                std::cout << "Player details:\n" << response.text << "\n";
            }
            else {
                std::cout << "Error retrieving player details: " << response.text << "\n";
            }
        }
    } while (choice != 3);

    std::cout << "Goodbye!\n";
    return 0;
}
