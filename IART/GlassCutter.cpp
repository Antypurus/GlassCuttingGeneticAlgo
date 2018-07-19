#include "GlassCutter.h"

#include <iostream> 
#include <fstream>
#include <sstream>
#include <algorithm> 
#include <random>
#include <chrono> 
#include "Piece.h"
#include "Board.h"

using std::string;
using std::pair;
using std::vector;
using std::cout;
using iart::Piece;
using iart::Board;

namespace iart
{
	pair<Board, vector<Piece>> GlassCutter::get_pieces(const string &filename)
	{
		vector<Piece>pieces;
		Board::BOARD_LENGTH boardLength = 0;
		Board::BOARD_HEIGTH boardHeigth = 0;
		int num_boards;

		std::ifstream file(filename);
		if (file.is_open())
		{
			auto counter = 0;
			string str;
			char name = 'A';
			while (std::getline(file, str))
			{
				std::istringstream stream(str);
				if (counter == 0)
				{
					stream >> boardLength;
					stream >> boardHeigth;
					stream >> num_boards;
				}
				else
				{
					string type;
					Piece::PIECE_LENGTH pieceLength = 0;
					Piece::PIECE_HEIGTH pieceHeigth = 0;
					int num_pieces;
					stream >> type;
					if (type != "quad" && type != "tri")
					{
						cout << "Invalid type for piece!";
						exit(-1);
					}

					stream >> pieceLength;
					if (pieceLength > boardLength)
					{
						cout << "Invalid m_board_ length! Piece length cannot be higher than m_board_ length.";
						exit(-1);
					}
					stream >> pieceHeigth;
					if (pieceHeigth > boardHeigth)
					{
						cout << "Invalid m_board_ height! Piece height cannot be higher than m_board_ height.";
						exit(-1);
					}
					stream >> num_pieces;

					for (size_t i = 0; i < num_pieces; i++)
						pieces.emplace_back(pieceLength, pieceHeigth, type, name);
					name = static_cast<char>(name + 1);
				}
				counter++;
			}
			if (counter == 0)
			{
				cout << "Invalid File Format";
				exit(-1);
			}
		}
		else
		{
			cout << "Failed To Open File";
			exit(-1);
		}

		Board board(boardLength, boardHeigth, num_boards);
		return std::make_pair(board, pieces);
	}

	vector<vector<Piece>> GlassCutter::get_n_shuffled_vectors(vector<Piece> pieces, unsigned int number_shuffles)
	{
		vector<vector<Piece>> shuffled_pieces;
		std::random_device seed; //obtain a truly random seed
		std::default_random_engine generator(seed());
		for (unsigned int i = 0; i < number_shuffles; i++)
		{
			shuffle(pieces.begin(), pieces.end(), generator);
			shuffled_pieces.push_back(pieces);
		}

		return shuffled_pieces;
	}
}