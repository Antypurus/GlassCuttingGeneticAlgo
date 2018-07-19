#pragma once
#include <vector>

#include "Board.h"
#include "Piece.h"

using std::vector;

class PiecesPlacement
{
private:
	vector<iart::Piece> pieces;
	iart::Board::BOARD_HEIGTH boardHeigth;
	iart::Board::BOARD_LENGTH boardLength;
	double boardArea;
	unsigned int numAllBoards;
	unsigned int numAvailableBoards;
	double lastBoardClearArea;

public:
	PiecesPlacement(const vector<iart::Piece> pieces, const iart::Board board);
	void place();
	double evaluatePlacementGA() const;
	double getFreeArea() const;
	double evaluatePlacementSA() const;
};