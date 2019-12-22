#include <string>
#include "Side.h"
#include "Player.h"
#include <algorithm>
#include <iostream>
#include <cstdlib> 

using namespace std;

Player::Player(std::string name) 
	: m_name(name)
{
	
}

string Player::name() const
{
	return m_name;
}

bool Player::isInteractive() const
{
	return false;
}


////////////////////////////////////////////////


HumanPlayer::HumanPlayer(string name) : Player(name)
{

}
bool HumanPlayer::isInteractive() const
{
	return true;
}
int HumanPlayer::chooseMove(const Board& b, Side s) const
{
	if (b.beansInPlay(s) == 0)
		return -1;
	
	bool madeValidMove = false;
	int holeNumber = -1;
	while (!madeValidMove)
	{
		cout << "---Sow from (q/";								//prompt user
		for (int i = 1; i < b.holes() + 1; i++)
		{
			cout << to_string(i);
			if (i < b.holes())
				cout << "/";
		}

		cout << ")---" << endl;
		string action;
		getline(cin, action);

		if (action.size() == 0)								//if nothign is entered
		{
			cout << "Nothing is Entered!" << endl;
			continue;
		}
		if (action[0] == 'q')								//if user entered q for quit
			exit(0);
		holeNumber = action[0] - '0';						//else, get the number
		if (holeNumber > b.holes() || holeNumber == 0 || b.beans(s, holeNumber) == 0)	//if number is invalid
		{
			cout << "Invalid Number!" << endl;
			continue;
		}
			
		madeValidMove = true;			//set flag to terminate while loop
		cout << endl;
	}
	return holeNumber;
}
HumanPlayer::~HumanPlayer()
{

}


////////////////////////////////////////////////


BadPlayer::BadPlayer(string name) : Player(name)
{

}
int BadPlayer::chooseMove(const Board& b, Side s) const
{
	if (b.beansInPlay(s) == 0 || b.beansInPlay(opponent(s)) == 0)
		return -1;
	int moveChose = 0;
	for (int i = 1; i < b.holes() + 1; i++)
	{
		if (b.beans(s, i) != 0)
		{
			moveChose = i;
			break;
		}
	}
	return moveChose;
}
BadPlayer::~BadPlayer()
{

}

////////////////////////////////////////////////


SmartPlayer::SmartPlayer(string name) : Player(name)
{

}
int SmartPlayer::chooseMove(const Board& b, Side s) const			
{
	if (b.beansInPlay(s) == 0 || b.beansInPlay(opponent(s)) == 0)			//if either side empty
		return -1;
	Board newBoard(b);						//have a changable copy of the board
	
	int depth = 1;
	int depthLimit = 7;

	int* currentMovesExp = new int[b.holes()];	//have a array that record the expected value of all moves (including invalid ones)
	for (int i = 0; i < b.holes(); i++)
		currentMovesExp[i] = 0;
	vector<int> movesIndex;						//have another vector that record the index of valid moves
	
	for (int i = 1; i < b.holes() +1; i++)		//i is in interface numbering
	{
		if (b.beans(s, i) > 0)
		{
			Board leafBoard(b);
			Side endSide;
			int endHole;

			leafBoard.sow(s, i, endSide, endHole);		//simulate sowing

			if (endHole == 0 && endSide == s)			//if endup in your own pot, do another round with the same side
			{
				currentMovesExp[i - 1] = treeSearch(leafBoard, s, s, depth + 1, depthLimit);
			}
			else if (endHole != 0 && leafBoard.beans(endSide, endHole) == 1 && endSide == s && leafBoard.beans(opponent(endSide), endHole) != 0)   //capture, then switch side
			{
				leafBoard.moveToPot(opponent(s), endHole, s);			
				leafBoard.moveToPot(s, endHole, s);
				currentMovesExp[i - 1] = treeSearch(leafBoard, opponent(s), s, depth + 1, depthLimit);
			}
			else    //switch side
			{
				currentMovesExp[i - 1] = treeSearch(leafBoard, opponent(s), s, depth + 1, depthLimit);		
			}
			movesIndex.push_back(i - 1);
		}
	}

	int* validValueList = new int[movesIndex.size()];			//have another array that only record valid values
	for (size_t i = 0; i < movesIndex.size(); i++)
	{
		validValueList[i] = currentMovesExp[movesIndex[i]];
	}

	//at this level, we are only interested in maximizing the expected value
	if (s == SOUTH)	//reverse array if SOUTH to always output the highest value closest to the hole
	{
		reverse(validValueList + 0, validValueList + movesIndex.size());		//reverse
		int maxIndexForShortList = movesIndex.size() - 1 - (max_element(validValueList, validValueList + movesIndex.size()) - validValueList);		//find the index of maximum value in condensed array
		int maxIndex = movesIndex[maxIndexForShortList];			//find the corresponding move index
		delete currentMovesExp;
		delete validValueList;
		return maxIndex + 1;			//go back to interface numbering
	}
	else
	{
		int maxIndexForShortList = (max_element(validValueList, validValueList + movesIndex.size()) - validValueList);      //find the index of minimum value in condensed array
		int maxIndex = movesIndex[maxIndexForShortList];		//find the corresponding move index
		delete currentMovesExp;						
		delete validValueList;
		return maxIndex + 1;			//go back to interface numbering
	}
}

int SmartPlayer::treeSearch(Board parentBoard, Side s, const Side originalSide, int depth, int depthLimit) const	//explanation in header file
{
	if (depth == depthLimit || parentBoard.beansInPlay(SOUTH) == 0 || parentBoard.beansInPlay(NORTH) == 0)		//if reached the depth limit or if either side is empty
	{		
		if (parentBoard.beansInPlay(SOUTH) == 0 || parentBoard.beansInPlay(NORTH) == 0)
		{
			for (int i = 1; i < parentBoard.holes() + 1; i++)		//clean up board
			{
				parentBoard.moveToPot(NORTH, i, NORTH);
				parentBoard.moveToPot(SOUTH, i, SOUTH);
			}
		}
		return evaluation(parentBoard, originalSide);
	}
		
	int nHoles = parentBoard.holes();		

	int* valueList = new int[nHoles];	//record expected value of all moves, all entries initialize to 0
	for (int i = 0; i < nHoles; i++)	
		valueList[i] = 0;
	vector<int> indexList;				//record index of valid moves

	for (int i = 1; i < nHoles + 1; i++)		//i is in interface numbering
	{
		if (parentBoard.beans(s, i) > 0)
		{
			Board leafBoard(parentBoard);
			Side endSide;
			int endHole;

			leafBoard.sow(s, i, endSide, endHole);		//simulate sowing

			if (endHole == 0 && endSide == s)			//if endup in your own pot, do another round with the same side							
			{
				valueList[i - 1] = treeSearch(leafBoard, s, originalSide, depth + 1, depthLimit);
			}
			else if (endHole != 0 && leafBoard.beans(endSide, endHole) == 1 && endSide == s && leafBoard.beans(opponent(endSide), endHole) != 0)
			{
				leafBoard.moveToPot(opponent(s), endHole, s);			//capture, then switch side
				leafBoard.moveToPot(s, endHole, s);
				valueList[i-1] = treeSearch(leafBoard, opponent(s), originalSide, depth + 1, depthLimit);
			}
			else
			{
				valueList[i - 1] = treeSearch(leafBoard, opponent(s), originalSide, depth + 1, depthLimit);		//switch side
			}
			indexList.push_back(i - 1);
		}
	}
			
	int* validValueList = new int[indexList.size()];		//have another array to record only valid moves
	for (size_t i = 0; i < indexList.size(); i++)
	{
		validValueList[i] = valueList[indexList[i]];			
	}
	if (s == originalSide)
	{
		int maxValue = *max_element(validValueList, validValueList + indexList.size());		//find the maximum value in the condensed array
		delete valueList;
		delete validValueList;
		return maxValue;
	}
	else
	{
		int minValue = *min_element(validValueList, validValueList + indexList.size());		//find the minimum value in the condensed array
		delete valueList;
		delete validValueList;
		return minValue;
	}
}

int SmartPlayer::evaluation(const Board& b, Side s) const		//explanation in header file
{
	return b.beans(s, 0) - b.beans(opponent(s), 0);						//return the difference between s side's pot and the opponent's pot
}

SmartPlayer::~SmartPlayer()
{

}