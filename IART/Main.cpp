#include "GlassCutter.h"
#include <iostream>
#include "Board.h";
#include "Piece.h";
#include "PiecesPlacement.h"
#include "SimulatedAnnealing.h"
#include "Gene.h"
#include "GeneticAlgorithm.h"
#include <fstream>
#include "SimulatedAnnealing.h"
#include <cmath>



vector<vector<vector<Piece>>> getBoardsMatrixes(iart::Board board, vector<Piece> pieces)
{
	vector<vector<vector<Piece>>> boardsMatrixes;
	iart::Board::BOARD_HEIGTH boardHeigth = board.get_heigth();
	iart::Board::BOARD_LENGTH boardLength = board.get_length();
	double maxCurrHeigth = 0.0;
	int lastLineChange = 0;

	unsigned int numAvailableBoards = board.get_num_boards();
	numAvailableBoards--; //first m_board_ it starting to being used

	vector<vector<Piece>> currBoardMatrix;
	vector<Piece> currLinePieces;
	size_t numPieces = pieces.size();
	for (size_t i = 0; i < numPieces; i++)
	{
		Piece &piece = pieces[i];
		if (piece.is_paired())
		{
			currLinePieces.push_back(piece);
			continue;
		}
		if (piece.get_type() == Piece::PIECE_TYPE::tri && i < numPieces - 1)
		{
			Piece &nextPiece = pieces[i + 1];
			if (nextPiece.get_type() == Piece::PIECE_TYPE::tri)
			{
				piece.set_paired(true);
				nextPiece.set_paired(true);

				if (nextPiece.get_length() > piece.get_length())
				{
					Piece aux = piece;
					piece = nextPiece;
					nextPiece = aux;
				}
			}
		}
		const Piece::PIECE_LENGTH	pieceLength = piece.get_length();
		const Piece::PIECE_HEIGTH	pieceHeigth = piece.get_heigth();

		boardLength -= pieceLength;
		if (boardLength < 0)
		{
			if (piece.is_paired())
			{
				piece.set_paired(false);
				if (i < numPieces - 1)
					pieces[i + 1].set_paired(false);
			}

			i--;
			lastLineChange = i;
			boardLength = board.get_length();
			if (maxCurrHeigth < pieceHeigth)
				maxCurrHeigth = pieceHeigth;
			boardHeigth -= maxCurrHeigth;
			if (boardHeigth < 0)
			{
				numAvailableBoards--;
				if (numAvailableBoards <= 0)
				{
					std::cout << "Number of boards is less than possible for the given pieces!";
					exit(-1);
				}
			}

			maxCurrHeigth = 0.0;
			currBoardMatrix.push_back(currLinePieces);
			currLinePieces.clear();
			if (boardHeigth < 0)
			{
				boardsMatrixes.push_back(currBoardMatrix);
				currBoardMatrix.clear();
				boardHeigth = board.get_heigth();
			}
			continue;
		}

		if (maxCurrHeigth < pieceHeigth)
		{
			maxCurrHeigth = pieceHeigth;
			if (boardHeigth < maxCurrHeigth)
			{
				numAvailableBoards--;
				if (numAvailableBoards <= 0)
				{
					std::cout << "Number of boards is less than possible for the given pieces!";
					exit(-1);
				}

				if (piece.is_paired())
				{
					piece.set_paired(false);
					if (i < numPieces - 1)
						pieces[i + 1].set_paired(false);
				}

				i = lastLineChange;
				boardHeigth = board.get_heigth();
				boardLength = board.get_length();
				maxCurrHeigth = 0.0;

				currBoardMatrix.push_back(currLinePieces);
				currLinePieces.clear();
				boardsMatrixes.push_back(currBoardMatrix);
				currBoardMatrix.clear();

				continue;
			}
		}

		currLinePieces.push_back(piece);
	}

	if (currLinePieces.size() != 0)
		currBoardMatrix.push_back(currLinePieces);
	if (currBoardMatrix.size() != 0)
		boardsMatrixes.push_back(currBoardMatrix);

	return boardsMatrixes;
}

void drawJoinedTriangles(vector<std::string> &boardPaint, unsigned int &currHorizontalPos, unsigned int &currVerticalPos, const Piece &triangle1, const Piece &triangle2)
{
	Piece largerTriangle;
	Piece thinTriangle;
	if (triangle1.get_length() >= triangle2.get_length())
	{
		largerTriangle = triangle1;
		thinTriangle = triangle2;
	}
	else
	{
		largerTriangle = triangle2;
		thinTriangle = triangle1;
	}

	//draw first horizontal line - largerTriangle
	for (int i = 1; i < largerTriangle.get_length() + 1; i++)
	{
		boardPaint[currVerticalPos][currHorizontalPos + i] = '_';
	}

	//draw horizontal line - thinTriangle
	unsigned int verticalPos = currVerticalPos + thinTriangle.get_heigth();
	unsigned int lastPosHorizontal = currHorizontalPos + largerTriangle.get_length();
	for (int i = 0; i < thinTriangle.get_length(); i++)
	{
		boardPaint[verticalPos][lastPosHorizontal - i] = '_';
	}

	//draw last vertical line - thinTriangle
	unsigned int stringLastPos = currHorizontalPos + largerTriangle.get_length();
	for (int i = 0; i < thinTriangle.get_heigth(); i++)
	{
		boardPaint[currVerticalPos + i + 1][currHorizontalPos + stringLastPos] = '|';
	}

	//draw first vertical line - largerTriangle
	for (int i = 0; i < largerTriangle.get_heigth(); i++)
	{
		boardPaint[currVerticalPos + i + 1][currHorizontalPos] = '|';
	}

	//draw oblique line - largerTriangle
	unsigned int lastVerticalPos = currVerticalPos + largerTriangle.get_heigth();
	unsigned int lower = std::min(largerTriangle.get_length(), largerTriangle.get_heigth());
	for (int i = 0; i < lower; i++)
	{
		boardPaint[lastVerticalPos - i][currHorizontalPos + i] = '/';
	}

	boardPaint[currVerticalPos + 1][currHorizontalPos + 1] = largerTriangle.getName();
	boardPaint[verticalPos - 1][lastPosHorizontal - 1] = thinTriangle.getName();
	currHorizontalPos += largerTriangle.get_length();
}

void drawRectangle(vector<std::string> &boardPaint, unsigned int &currHorizontalPos, unsigned int &currVerticalPos, const Piece &piece)
{
	//draw first horizontal line
	boardPaint[currVerticalPos + 1][currHorizontalPos + 1] = piece.getName();
	for (int i = 1; i < piece.get_length(); i++)
	{
		boardPaint[currVerticalPos][currHorizontalPos + i] = '_';
	}

	//draw last horizontal line
	unsigned int heigthLastPos = currVerticalPos + piece.get_heigth();
	for (int i = 0; i < piece.get_length(); i++)
	{
		boardPaint[heigthLastPos][currHorizontalPos + i] = '_';
	}

	//draw first vertical line
	for (int i = 0; i < piece.get_heigth(); i++)
	{
		boardPaint[currVerticalPos + i + 1][currHorizontalPos] = '|';
	}

	//draw last vertical line
	unsigned int stringLastPos = currHorizontalPos + piece.get_length();
	for (int i = 0; i < piece.get_heigth(); i++)
	{
		boardPaint[currVerticalPos + i + 1][stringLastPos] = '|';
	}

	currHorizontalPos += piece.get_length();
}

void drawTriangle(vector<std::string> &boardPaint, unsigned int &currHorizontalPos, unsigned int &currVerticalPos, const Piece &piece)
{
	//draw first horizontal line
	boardPaint[currVerticalPos + 1][currHorizontalPos + 1] = piece.getName();
	for (int i = 1; i < piece.get_length(); i++)
	{
		boardPaint[currVerticalPos][currHorizontalPos + i] = '_';
	}

	//draw first vertical line
	for (int i = 0; i < piece.get_heigth(); i++)
	{
		boardPaint[currVerticalPos + i + 1][currHorizontalPos] = '|';
	}

	//draw oblique line
	unsigned int lastVerticalPos = currVerticalPos + piece.get_heigth();
	unsigned int lower = std::min(piece.get_length(), piece.get_heigth());
	for (int i = 0; i < lower; i++)
	{
		boardPaint[lastVerticalPos - i][currHorizontalPos + i] = '/';
	}

	currHorizontalPos += piece.get_length();
}

unsigned getLineHeigth(const vector<Piece>& pieces)
{
	unsigned int higherHeight = 0;
	for (size_t i = 0; i < pieces.size(); i++)
	{
		if (pieces[i].get_heigth() > higherHeight)
			higherHeight = pieces[i].get_heigth();
	}

	return higherHeight;
}

vector<std::string> drawBoard(vector<vector<Piece>> &boardMatrix, const unsigned int &boardLength, const unsigned int &boardHeigth)
{
	std::string string = std::string(boardLength * 2, ' ');
	vector<std::string> boardPaint(boardHeigth * 2, string);
	unsigned int currVerticalPos = 0;
	unsigned int currHorizontalPos = 0;
	Piece board = Piece(boardLength, boardHeigth, Piece::quad);
	drawRectangle(boardPaint, currHorizontalPos, currVerticalPos, board);
	currVerticalPos = 0;
	currHorizontalPos = 0;

	size_t boardMatrixSize = boardMatrix.size();
	for (size_t i = 0; i < boardMatrixSize; i++)
	{
		vector<Piece> pieces = boardMatrix[i];
		currHorizontalPos = 0;
		size_t numPieces = pieces.size();
		for (size_t j = 0; j < numPieces; j++)
		{
			Piece &piece = pieces[j];
			if (piece.get_type() == Piece::quad)
				drawRectangle(boardPaint, currHorizontalPos, currVerticalPos, piece);
			else
			{
				if (piece.is_paired())
				{
					drawJoinedTriangles(boardPaint, currHorizontalPos, currVerticalPos, piece, pieces[j + 1]);
					j++;
				}
				else
					drawTriangle(boardPaint, currHorizontalPos, currVerticalPos, piece);
			}

		}

		currVerticalPos += getLineHeigth(pieces);
	}

	return boardPaint;
}


vector<vector<std::string>> drawBoards(vector<vector<vector<Piece>>> &boardsMatrixes, const unsigned int &boardLength, const unsigned int &boardHeigth)
{
	vector<vector<std::string>> boardsPaint;
	size_t boardsMatrixesSize = boardsMatrixes.size();
	for (size_t i = 0; i < boardsMatrixesSize; i++)
	{
		vector<std::string> boardPaint = drawBoard(boardsMatrixes[i], boardLength, boardHeigth);
		boardsPaint.push_back(boardPaint);
	}

	return boardsPaint;
}

void drawBoardsString(const iart::Board board, const vector<Piece> &pieces)
{
	vector<vector<vector<Piece>>> boardMatrixes = getBoardsMatrixes(board, pieces);
	vector<vector<std::string>> boardsToDraw = drawBoards(boardMatrixes, board.get_length(), board.get_heigth());

	size_t numBoards = boardsToDraw.size();
	for (size_t i = 0; i < numBoards; i++)
	{
		size_t numBoardLines = boardsToDraw[i].size();
		for (size_t j = 0; j < numBoardLines; j++)
		{
			std::cout << boardsToDraw[i][j] << "\n";
		}
	}
}

void geneticAlghorithm(const std::pair<iart::Board, vector<Piece>> &pair, const unsigned int populationNumber, const unsigned int maxGenerationsNumber)
{
	const vector<vector<Piece>> population = iart::GlassCutter::get_n_shuffled_vectors(pair.second, populationNumber);
	iart::GeneticAlgorithm genetic_algorithm = iart::GeneticAlgorithm(population, pair.first);

	std::vector<iart::GA::Gene> genes;
	for (unsigned int i = 0; i < maxGenerationsNumber; ++i)
	{
		genes = genetic_algorithm.crossoverPopulation();
	}

	drawBoardsString(pair.first, genes[genes.size() - 1].getChromosomes());
	std::cout << "Done!\n";
}

double tempFunc(double x)
{
	return -log10(x) + 1.5;
}

void simulatedAnnealing(const std::pair<iart::Board, vector<Piece>> &pair)
{
	SimulatedAnnealing annealing;
	const vector<Piece> ret = annealing.run(tempFunc, pair.second, pair.first);
	drawBoardsString(pair.first, ret);
	std::cout << "Done!\n";
}

void main(int argc, char** argv)
{
	if(argc < 3 || (argc < 5 && argv[1] == "GA"))
	{
		std::cout << "Usage: GA <filepath> <population number> <num generations>" << "\n";
		std::cout << "or: SA <filepath>" << "\n";
		getchar();
		exit(-1);
	}
	
	using namespace iart;
	std::string filepath = argv[2];
	const std::pair<Board, vector<Piece>> pair = GlassCutter::get_pieces(filepath);

	const std::string alghorithm = std::string(argv[1]);
	if (alghorithm == "GA")
	{
		const unsigned int populationNumber = atoi(argv[3]);
		const unsigned int maxGenerationsNumber = atoi(argv[4]);
		geneticAlghorithm(pair, populationNumber, maxGenerationsNumber);
	}
	else if (alghorithm == "SA")
		simulatedAnnealing(pair);
	else
	{
		std::cout << "Usage: GA <filepath> <population number> <max generations number>" << "\n";
		std::cout << "or: SA <filepath>" << "\n";
		getchar();
		exit(-1);
	}

	getchar();
}
