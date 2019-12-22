#ifndef GAME_H
#define GAME_H

#include <string>
#include "Side.h"
#include "Board.h"
#include "Player.h"

using namespace std;

class Game
{
public:
	Game(const Board& b, Player* south, Player* north);
	void display() const;
	void status(bool& over, bool& hasWinner, Side& winner) const;
	bool move();
	void play();
	int beans(Side s, int hole) const;

	Board m_board;

private:

	Player* m_southPtr;
	Player* m_northPtr;

	bool m_over;
	bool m_hasWinner;
	Side m_winner;

	Side currentPlayer;
	bool turnOver;
};





#endif