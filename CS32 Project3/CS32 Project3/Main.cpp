

#include <string>
#include <iostream>
#include <cassert>

#include "Player.h"
#include "Game.h"
#include "Board.h"

using namespace std;


int main()
{	
	SmartPlayer smart1("smart1");
	SmartPlayer smart2("smart2");
	BadPlayer bad("bad");
	HumanPlayer human("human");

	Board b00(6, 4);
	Game g00(b00, &smart1, &smart2);
	g00.play();


	//////Board constructor test
	Board b0(0, -10);
	assert(b0.holes() == 1 && b0.beans(NORTH, 1) == 0 && b0.beans(SOUTH, 1) == 0);


	//////Board hole numbering test
	Board b1(3, 4);				
	assert(b1.beans(SOUTH, 1) == 4);
	assert(b1.beans(SOUTH, 2) == 4);
	assert(b1.beans(SOUTH, 3) == 4);
	assert(b1.beans(SOUTH, 0) == 0);
	assert(b1.beans(NORTH, 1) == 4);
	assert(b1.beans(NORTH, 2) == 4);
	assert(b1.beans(NORTH, 3) == 4);
	assert(b1.beans(NORTH, 0) == 0);		//right numbering

	//////Board::beansInPlay
	assert(b1.beansInPlay(NORTH) == 12);
	assert(b1.beansInPlay(SOUTH) == 12);

	//////Board::sow 		
	Board b2(3, 5);				
	Side endSide;
	int endHole;
		
	b2.sow(SOUTH, 3, endSide, endHole);
	assert(b2.beans(SOUTH, 1) == 6);
	assert(b2.beans(SOUTH, 2) == 5);
	assert(b2.beans(SOUTH, 3) == 0);
	assert(b2.beans(SOUTH, 0) == 1);
	assert(b2.beans(NORTH, 1) == 6);
	assert(b2.beans(NORTH, 2) == 6);
	assert(b2.beans(NORTH, 3) == 6);
	assert(b2.beans(NORTH, 0) == 0);						//should skip opponent's hole
	
	assert(b2.sow(NORTH, 0, endSide, endHole) == false);
	assert(b2.sow(SOUTH, 0, endSide, endHole) == false);	//should not sow from pot

	assert(b2.sow(SOUTH, 3, endSide, endHole) == false);	//should not sow empty hole


	//////Board::moveToPot 
	assert(b2.moveToPot(SOUTH, 1, NORTH) == true && b2.beans(SOUTH, 1) == 0 && b2.beans(NORTH, 0) == 6);
	assert(b2.moveToPot(SOUTH, 1, SOUTH) == true);			//can still move from empty hole
	assert(b2.moveToPot(NORTH, 10, NORTH) == false);		//invalid index
	assert(b2.moveToPot(NORTH, 0, SOUTH) == false);			//cannot move from pot


	//////Board::setBeans 
	assert(b2.setBeans(NORTH, 1, 10) == true && b2.beans(NORTH, 1) == 10);
	assert(b2.setBeans(NORTH, 0, 10) == true && b2.beans(NORTH, 0) == 10);				//can set pot 
	assert(b2.setBeans(NORTH, 10, 3) == false);				//invalid index
	assert(b2.setBeans(NORTH, 1, -10) == false);			//invalid bean number

	//////Game::status
	Board b3(6, 0);
	b3.setBeans(NORTH, 0, 4);
	bool over; bool hasWinner; Side winner;
	
	Game g3(b3, &smart1, &smart2);
	g3.status(over, hasWinner, winner);
	assert(over == true && hasWinner == true && winner == NORTH);		//game is over because board empty


	//////Game::move
	assert(g3.move() == false);				//game already over
	
	Board b4(6, 0);
	Game g4(b4, &smart1, &smart2);
	assert(g4.move() == false);				//no move possible
	

	//////Game::display()
	//g4.display();         
	//g3.display();			//reallly nothing to test for, should terminate normally


	//////Game::beans()
	assert(g3.beans(NORTH, 2) == 0 && g3.beans(SOUTH, 2) == 0);	//should work the same as Board's beans function


	//////Game::play()
	//g4.play();
	//g3.play();				//should terminate normally


	//////HumanPlayer::chooseMove
	Board b5(6, 1);
	Side endSide5;
	int endHole5;
	//human.chooseMove(b5, NORTH);		
	//human.chooseMove(b5, SOUTH);		//should terminate normally


	//////BadPlayer::chooseMove
	assert(b5.sow(SOUTH, bad.chooseMove(b5, SOUTH), endSide5, endHole5));
	assert(b5.sow(NORTH, bad.chooseMove(b5, NORTH), endSide5, endHole5));		
	assert(b5.sow(SOUTH, bad.chooseMove(b5, SOUTH), endSide5, endHole5));
	assert(b5.sow(NORTH, bad.chooseMove(b5, NORTH), endSide5, endHole5));
	assert(b5.sow(SOUTH, bad.chooseMove(b5, SOUTH), endSide5, endHole5));
	assert(b5.sow(NORTH, bad.chooseMove(b5, NORTH), endSide5, endHole5));
	assert(b5.sow(SOUTH, bad.chooseMove(b5, SOUTH), endSide5, endHole5));
	assert(b5.sow(NORTH, bad.chooseMove(b5, NORTH), endSide5, endHole5));		//all moves should be legal

	//////SmartPlayer::chooseMove 
	Board b6(6, 0);
	//   1 0 0 0 3 0
	// 0             0				//as south, should first sow the 1 and then move 9 out of danger
	//   0 9 0 0 0 1				//as north, should first sow the 1 and then capture the 9
	b6.setBeans(SOUTH, 2, 9);
	b6.setBeans(SOUTH, 6, 1);
	b6.setBeans(NORTH, 1, 1);
	b6.setBeans(NORTH, 5, 3);
	assert(smart1.chooseMove(b6, SOUTH) == 6);
	assert(smart1.chooseMove(b6, NORTH) == 1);
	
	Board b7(6, 0);
	//   1 0  1 0 5 0
	// 0              0				//as south, should avoid the "temptation" of captring the 5 and move 20 out of danger
	//   0 20 0 1 0 0	
	b7.setBeans(SOUTH, 2, 20);
	b7.setBeans(SOUTH, 4, 1);
	b7.setBeans(NORTH, 1, 1);
	b7.setBeans(NORTH, 5, 5);
	b7.setBeans(NORTH, 3, 1);	
	assert(smart1.chooseMove(b7, SOUTH) == 2);

	//   1 0  1 0 20 0
	// 0               0				//in this case it's actually better to capture than to defend
	//   0 20 0 1 0  0	
	b7.setBeans(NORTH, 5, 20);
	
	assert(smart1.chooseMove(b7, SOUTH));
	 
	cerr << "Passed all test" << endl;

}
