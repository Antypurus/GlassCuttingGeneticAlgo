#include <iostream>

#include "PiecesPlacement.h"

using namespace std;
using namespace iart;


PiecesPlacement::PiecesPlacement(const vector<Piece> pieces, const Board board)
{
	this->pieces = pieces;
	this->boardHeigth = board.get_heigth();
	this->boardLength = board.get_length();
	this->boardArea = board.get_board_area();
	this->numAllBoards = board.get_num_boards();
	this->numAvailableBoards = board.get_num_boards();
}

void PiecesPlacement::place()
{
	const Board::BOARD_HEIGTH defaultBoardHeigth = this->boardHeigth;
	const Board::BOARD_LENGTH defaultBoardLength = this->boardLength;
	double maxCurrHeigth = 0.0;
	int lastLineChange = 0;
	bool currPiecesPaired = false;
	Piece::PIECE_LENGTH	pieceLength;
	Piece::PIECE_HEIGTH	pieceHeigth;
	numAvailableBoards--; //first m_board_ it starting to being used

	size_t numPieces = pieces.size();
	for (size_t i = 0; i < numPieces; i++)
	{
		Piece &piece = pieces[i];
		if (piece.get_type() == Piece::PIECE_TYPE::tri && i < numPieces - 1 && piece.is_paired() == false)
		{
			Piece &nextPiece = pieces[i + 1];
			if (nextPiece.get_type() == Piece::PIECE_TYPE::tri)
			{
				piece.set_paired(true);
				nextPiece.set_paired(true);
				currPiecesPaired = true;

				if (nextPiece.get_length() > piece.get_length())
					piece = nextPiece;
			}
		}

		pieceLength = piece.get_length();
		pieceHeigth = piece.get_heigth();

		boardLength -= pieceLength;
		if (boardLength < 0)
		{
			i--;
			lastLineChange = i;
			boardLength = defaultBoardLength;
			boardHeigth -= maxCurrHeigth;
			maxCurrHeigth = 0.0;
		}

		if (maxCurrHeigth < pieceHeigth)
		{
			maxCurrHeigth = pieceHeigth;
			if (boardHeigth < maxCurrHeigth)
			{
				numAvailableBoards--;
				if (numAvailableBoards <= 0)
				{
					cout << "Number of boards is less than possible for the given pieces!";
					lastBoardClearArea = 0;
					numAvailableBoards = 0;
					return;
				}

				i = lastLineChange;
				boardHeigth = defaultBoardHeigth;
				boardLength = defaultBoardLength;
				maxCurrHeigth = 0.0;
				continue;
			}
		}

		if (currPiecesPaired)
		{
			i++;
			currPiecesPaired = false;
		}
	}

	//make internal values correct after processing
	//last m_board_ clear area
	boardHeigth -= maxCurrHeigth;
	lastBoardClearArea = boardLength * maxCurrHeigth + boardHeigth * defaultBoardLength;
}

double PiecesPlacement::evaluatePlacementGA() const
{
	return getFreeArea();
}

double PiecesPlacement::getFreeArea() const
{
	return numAvailableBoards * boardArea + lastBoardClearArea;
}

double PiecesPlacement::evaluatePlacementSA() const
{
	return (numAllBoards * boardArea) - getFreeArea();
}