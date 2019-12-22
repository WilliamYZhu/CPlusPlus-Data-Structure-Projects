#include <string>
#include <iostream>
#include "Side.h"
#include "Game.h"

Game::Game(const Board& b, Player* south, Player* north)
	: m_board(b), m_southPtr(south), m_northPtr(north), m_over(false), m_hasWinner(false), currentPlayer(SOUTH), turnOver(false)	
{}

void Game::display() const
{
	cout << endl;
	if (currentPlayer == NORTH && m_over == false)			//display title correctly
	{
		cout << "North's Turn" << endl;
		if (m_northPtr->isInteractive() == true)
			cout << "Player to Move" << endl;
	}	
	else if (currentPlayer == SOUTH && m_over == false)
	{
		cout << "South's Turn" << endl;
		if (m_southPtr->isInteractive() == true)
			cout << "Player to Move" << endl;
	}
	if (m_over == true )
		cout << "Game Finished!" << endl;


	int nHoles = m_board.holes();			//displaying the board
	cout << "    ";
	for (int i = 1; i < nHoles + 1; i++)		//north holes
		cout << m_board.beans(NORTH, i) << " ";
	cout << endl << m_board.beans(NORTH, 0);	//north pot on the next line
	for (int i = 0; i < nHoles; i++)				
	{
		if (nHoles < 10)						//adjust spacing between two pots
			cout << "  ";
		else if (nHoles >= 10 && nHoles < 100)
			cout << "   ";
		else
			cout << "    ";
	}
	cout << "     " << m_board.beans(SOUTH, 0) << endl;		//south pot
	cout << "    ";
	for (int i = 1; i < nHoles + 1; i++)					//south holes
		cout << m_board.beans(SOUTH, i) << " ";
	
	cout << endl;
	string str = "    South: " + m_southPtr->name() + "    |    " + "North: " + m_northPtr->name(); //player names

	for (size_t i = 0; i < str.size() + 4; i++)			//display player names
		cout << "-";
	cout << endl << str << endl;
	for (size_t i = 0; i < str.size() + 4; i++)
		cout << "-";
	cout << endl << endl;
}	

void Game::status(bool& over, bool& hasWinner, Side& winner) const
{
	over = false;					//by default, both are false
	hasWinner = false;
	if(m_board.beansInPlay(NORTH) == 0 || m_board.beansInPlay(SOUTH) == 0)		//if either side is empty
	{
		over = true;				//the game is over
		//the cleaning board part is implemented in Game::move()
		if (m_board.beans(NORTH, 0) > m_board.beans(SOUTH, 0))		//if one side has more beans in pot than the other, set winner
		{
			hasWinner = true;
			winner = NORTH;
		}
		else if (m_board.beans(NORTH, 0) < m_board.beans(SOUTH, 0))
		{
			hasWinner = true;
			winner = SOUTH;
		}
		else                             //else it's a tie
		{
			hasWinner = false;
		}
	}
}

bool Game::move()
{
	if (m_over)					//if game over, do not move
		return false;

	if (m_board.beansInPlay(NORTH) == 0 || m_board.beansInPlay(SOUTH) == 0)			//if either side is empty
		return false;
	
	int moveChose;
	int endHole;
	Side endSide;
	while (!turnOver)
	{
		if (currentPlayer == SOUTH)
		{
			if (m_southPtr->isInteractive() == false)
			{
				cout << "---Press Enter for Computer to Move---" << endl;		//prompt for enter
				cin.ignore(10000, '\n');	
			}
			moveChose = m_southPtr->chooseMove(m_board, SOUTH);
			if (!m_board.sow(SOUTH, moveChose, endSide, endHole))		//if invalid hole number
				continue;
		}
		else
		{
			if (m_northPtr->isInteractive() == false)
			{
				cout << "---Press Enter for Computer to Move---" << endl;		//prompt for enter
				cin.ignore(10000, '\n');
			}
			moveChose = m_northPtr->chooseMove(m_board, NORTH);
			if (!m_board.sow(NORTH, moveChose, endSide, endHole))		//if invalid hole number
				continue;
		}
		turnOver = true;

		status(m_over, m_hasWinner, m_winner);
		if (endHole == POT && endSide == currentPlayer && m_over == false)			//if endup in your own pot, do another turn
		{
			turnOver = false;
			display();
			continue;
		}
		if (endHole != POT && m_board.beans(endSide, endHole) == 1 && endSide == currentPlayer && m_board.beans(opponent(endSide), endHole) != 0)	//if land on empty hole on your own side
		{
			m_board.moveToPot(opponent(currentPlayer), endHole, currentPlayer);		//clearn up the board
			m_board.moveToPot(currentPlayer, endHole, currentPlayer);
		}
	}

	currentPlayer = opponent(currentPlayer);
	status(m_over, m_hasWinner, m_winner);
	if (m_over == true)
	{
		for (int i = 1; i < m_board.holes() + 1; i++)		//clean up board
		{
			m_board.moveToPot(NORTH, i, NORTH);
			m_board.moveToPot(SOUTH, i, SOUTH);
		}
	}
	display();
	turnOver = false;
	return true;
}

void Game::play()
{
	status(m_over, m_hasWinner, m_winner);
	display();
	while (m_over == false)
	{
		status(m_over, m_hasWinner, m_winner);
		if (!move())			//if move failed because no move is available
		{
			break;
		}
	}

	status(m_over, m_hasWinner, m_winner);				//check status one final time

	if (m_hasWinner)
	{
		cout << endl << "=====================================================" << endl << "                The Winner is ";
		if (m_winner == SOUTH)
			cout << "South!" << endl;
		else
			cout << "North!" << endl;
		cout << "=====================================================" << endl;
	}
	else
	{
		cout << endl << "=====================================================" << endl << "                      A Tie!";
		cout << endl << "=====================================================" << endl;
	}
	}

int Game::beans(Side s, int hole) const
{
	return m_board.beans(s, hole);
}




