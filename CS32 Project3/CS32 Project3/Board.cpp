#include <iostream>
#include "Side.h"
#include "Board.h"

using namespace std;


Board::Board(int nHoles, int nInitialBeansPerHole)
{
	if (nHoles <= 0)			//set m_nHole correctly
		m_nHoles = 1;
	else
		m_nHoles = nHoles;

	if (nInitialBeansPerHole < 0)		//set m_nInitialBeansPerHole correctly 
		m_nInitialBeansPerHole = 0;
	else
		m_nInitialBeansPerHole = m_nInitialBeansPerHole;

	for (int i = 0; i < nHoles; i++)			//add south holes
		m_array.push_back(nInitialBeansPerHole);
	m_array.push_back(0);						//add south pot

	for (int i = 0; i < nHoles; i++)			//add north holes
		m_array.push_back(nInitialBeansPerHole);
	m_array.push_back(0);						//add north pot	
}

int Board::holes() const
{
	return m_nHoles;
}

int Board::beans(Side s, int hole) const
{
	if (hole > m_nHoles)
		return -1;
	
	if (s == NORTH)				//correctly go between array numbering and interface numbering
		return m_array[m_array.size() - 1 - hole];
	else
	{
		if (hole == 0)
			return m_array[m_nHoles];
		else
			return m_array[hole - 1];
	}
}

int Board::beansInPlay(Side s) const
{
	int sum = 0;
	for (int i = 1; i < m_nHoles + 1; i++)
		sum += beans(s, i);
	return sum;					//sum of all beans in holes not counting pot
}

int Board::totalBeans() const
{
	return beans(SOUTH, 0) + beans(NORTH, 0) + beansInPlay(SOUTH) + beansInPlay(NORTH);
}

bool Board::sow(Side s, int hole, Side& endSide, int& endHole)
{
	if (hole == 0 || hole > m_nHoles || beans(s, hole) == 0)
		return false;

	int beansInHand = beans(s, hole);				//take beans in hand
	
	if (s == NORTH)									//set hole to zero, go from interface numbering to array numbering
		m_array[m_array.size() - 1 - hole] = 0;
	else
	{
		if (hole == 0)
			m_array[m_nHoles] = 0;
		else
			m_array[hole - 1] = 0;
	}


	int indexOfLast = m_array.size() - 1;		//the index of last item (north pot) 
	int index = 0;

	if (s == SOUTH)								//turn interface numbering to array numbering
		index = hole - 1;
	else
		index = (indexOfLast - hole);

	index++;
	while (beansInHand > 0)
	{
		if ((opponent(s) == NORTH && index == indexOfLast) || (opponent(s) == SOUTH && index == (m_nHoles)))	//if opponent's pot
		{
			index++;
		}
		else			//if does not land on the opponent's pot
		{
			index = index % (indexOfLast + 1);	//make it go circular (go from end back to beginning)
			m_array[index] += 1;
			beansInHand--;
			if (beansInHand > 0)				//if no beans left, stay where it was
				index++;
		}
	}

	if (index <= m_nHoles)
	{
		if (index == m_nHoles)
		{
			endSide = SOUTH;
			endHole = 0;					//array numbering to interface numbering
		}
		else
		{
			endSide = SOUTH;
			endHole = index + 1;			//array numbering to interface numbering
		}
	}
	else
	{
		endSide = NORTH;
		endHole = indexOfLast - index;		//array numbering to interface numbering
	}

	return true;
}

bool Board::moveToPot(Side s, int hole, Side potOwner)
{
	if (hole > m_nHoles || hole == 0)
		return false;
	
	int indexOfLast = m_array.size() - 1;		//the index of last item (north pot) 

	if (potOwner == NORTH)
		m_array[indexOfLast] += beans(s, hole);				//add beans to north pot
	else
		m_array[m_nHoles] += beans(s, hole);				//add beans to south pot

	if (s == NORTH)									//set hole to zero, go from interface numbering to array numbering
		m_array[m_array.size() - 1 - hole] = 0;
	else
	{
		if (hole == 0)
			m_array[m_nHoles] = 0;
		else
			m_array[hole - 1] = 0;
	}

	return true;
}

bool Board::setBeans(Side s, int hole, int beans)
{
	if (hole > m_nHoles || beans < 0)
		return false;
	
	if (s == NORTH)										//set hole to beans
		m_array[m_array.size() - 1 - hole] = beans;
	else
	{
		if (hole == 0)
			m_array[m_nHoles] = beans;
		else
			m_array[hole - 1] = beans;
	}

	return true;
}

