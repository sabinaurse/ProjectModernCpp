#pragma once
#include <vector>
#include "Penguin.h"
#include "Player.h"
#include "GameBoard.h"
#include <algorithm>


class Game
{
public:
	void StartGame();
	void EndGame();
	void Update();
	void AddPenguin(Player* player);
	void AddPlayer(Player* player);
	Player* GetWinner();
	Penguin* GetPenguinForPlayer(const Player& player);
	std::vector<Player*>& GetPlayers() { return m_players; }
	void ShowLeaderboard();
private:
	GameBoard m_gameBoard;
	std::vector<Player*> m_players;
	std::vector<Penguin*> m_penguins;
	bool m_isGameOver;
};

