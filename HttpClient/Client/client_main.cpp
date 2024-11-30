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
        std::cout << "3. Delete Player\n";
        std::cout << "4. Update Player\n";
        std::cout << "Choose an option: ";
        std::cin >> choice;

        if (choice == 1) {
            std::string name;

            std::cout << "Enter player name: ";
            std::cin >> name;

            auto response = ClientRequests::CreatePlayer(name);
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
        else if (choice == 3) {
            std::string name;
            std::cout << "Enter player name to delete: ";
            std::cin >> name;

            auto response = ClientRequests::DeletePlayer(name);
            if (response.status_code == 200) {
                std::cout << "Player deleted successfully!\n";
            }
            else {
                std::cout << "Error deleting player: " << response.text << "\n";
            }
        }
        else if (choice == 4) {
            std::string name;
            int newScore, newPoints;

            std::cout << "Enter player name to update: ";
            std::cin >> name;

            std::cout << "Enter new score: ";
            std::cin >> newScore;

            std::cout << "Enter new points: ";
            std::cin >> newPoints;

            auto response = ClientRequests::UpdatePlayer(name, newScore, newPoints);
            if (response.status_code == 200) {
                std::cout << "Player updated successfully!\n";
            }
            else {
                std::cout << "Error updating player: " << response.text << "\n";
            }
        }
    } while (choice != 5);
    

    std::cout << "Goodbye!\n";
    return 0;
}
