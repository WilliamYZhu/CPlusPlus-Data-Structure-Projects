#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "Board.h"
#include "Side.h"

using namespace std;

class Player
{
public:
	Player(string name);
	string name() const;
	virtual bool isInteractive() const;
	virtual int chooseMove(const Board& b, Side s) const = 0;
	virtual ~Player() {}

private:
	string m_name;
};

class HumanPlayer : public Player
{
public:
	HumanPlayer(string name);
	virtual bool isInteractive() const;
	virtual int chooseMove(const Board& b, Side s) const;
	virtual ~HumanPlayer();
};

class BadPlayer : public Player
{
public:
	BadPlayer(string name);
	virtual int chooseMove(const Board& b, Side s) const;
	virtual ~BadPlayer();
};

class SmartPlayer : public Player
{
public:
	SmartPlayer(string name);
	virtual int chooseMove(const Board& b, Side s) const;
	virtual ~SmartPlayer();
private:
	int treeSearch(Board b, Side s, const Side originalSide, int depth, int depthLimit) const;
	//a helper function for SmartPlayer::chooseMove that will be called recursively
	int evaluation(const Board& b, Side s) const;
	//an evaluation function that return the difference between s's pot and the opponent's pot (s - opponent)
};












#endif