#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
using namespace std;

#include "History.h"
#include "globals.h"

History::History(int nRows, int nCols)
{
	m_rows = nRows;
	m_cols = nCols;

	for (int r = 0; r < nRows; r++) {
		for (int c = 0; c < nCols; c++) {
			m_hits[r][c] = 0;
		}
	}
}

bool History::record(int r, int c)
{
	if (r <= 0 || c <= 0 || r > MAXROWS || c > MAXCOLS)
	{
		return false;
	}

	m_hits[r-1][c-1] = m_hits[r-1][c-1] + 1;

	return true;
}

void History::display() const
{
		// Position (row,col) of the history coordinate system is represented in
		// the array element grid[row-1][col-1]
		char grid[MAXROWS][MAXCOLS];
		int r, c;

		// Fill the grid with dots
		for (r = 0; r < m_rows; r++)
			for (c = 0; c < m_cols; c++)
				grid[r][c] = '.';

		// Add the appropriate counts
		for (int r = 0; r < m_rows; r++)
		{
			for (int c = 0; c < m_cols; c++)
			{
				for (int h = 0; h < m_hits[r][c]; h++) 
				{			
					char& gridChar = grid[r][c];
					switch (gridChar)
					{
					case '.':  gridChar = 'A'; break;
					case 'Z':  break;
					default:   gridChar++; break;  // 'B' through 'Z'
					}
				}
			}
			
		}

		// Draw the grid
		clearScreen();
		for (r = 0; r < m_rows; r++)
		{
			for (c = 0; c < m_cols; c++)
				cout << grid[r][c];
			cout << endl;
		}
		cout << endl;

		// Write message, wait
}
