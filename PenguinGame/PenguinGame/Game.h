#pragma once
#include <vector>
#include "Penguin.h"
#include "Player.h"
#include "GameBoard.h"
#include <algorithm>


class Game
{
public:
	// Game() : m_isGameOver(false) {}
	void StartGame();
	void EndGame();
	void RestartGame();
	void CheckForCollisions();
	void ShowLeaderboard();
	//void Update();

	void AddPenguin(Player* player);
	void AddPlayer(Player* player);
	void InitializePlayers();

	Player* GetWinner();
	Penguin* GetPenguinForPlayer(const Player& player);
	std::vector<Player*>& GetPlayers() { return m_players; }

private:
	GameBoard m_gameBoard;
	std::vector<Player*> m_players;
	std::vector<Penguin*> m_penguins;
	bool m_isGameOver{ false };
};

