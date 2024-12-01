#include <iostream>
#include "Requests.h"
#include "State.h"

int main() {

    /*cpr::Response res = ClientRequests::GetMap();

    if (res.status_code == 200) {
        std::cout << "The request was successful!" << std::endl;
        std::cout << "Map data (raw JSON):" << std::endl;
        std::cout << res.text << std::endl;  
    }
    else {
        std::cerr << "Error retrieving the map! Status code: " << res.status_code << std::endl;
    }*/

    ClientState state;
    int choice;

    do {
        std::cout << "1. Create Player\n";
        std::cout << "2. Get Player Details\n";
        std::cout << "3. Delete Player\n";
        std::cout << "4. Update Player\n";
        std::cout << "5. Start Game\n";
        std::cout << "6. Reset Game\n";
        std::cout << "7. Show Leaderboard\n";
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
        else if (choice == 5) {
            auto response = ClientRequests::StartGame();
            if (response.status_code == 200) {
                std::cout << "Game started successfully!\n";
            }
            else {
                std::cout << "Error starting game: " << response.text << "\n";
            }
        }
        else if (choice == 6) {
            auto response = ClientRequests::ResetGame();
            if (response.status_code == 200) {
                std::cout << "Game reset successfully!\n";
            }
            else {
                std::cout << "Error resetting game: " << response.text << "\n";
            }
        }
        else if (choice == 7) {
            auto response = ClientRequests::GetLeaderboard();
            if (response.status_code == 200) {
                std::cout << "Leaderboard:\n" << response.text << "\n";
            }
            else {
                std::cout << "Error retrieving leaderboard: " << response.text << "\n";
            }
        }
    } while (choice != 8);
    

    std::cout << "Goodbye!\n";
    return 0;
}
