#pragma once
#include <vector>
#include <utility>
#include <string>   

namespace iart
{
	class Board;
	class Piece;
}

namespace iart
{
	class GlassCutter
	{
	public:
		static std::pair<Board, std::vector<Piece>> get_pieces(const std::string& filename);
		static std::vector<std::vector<Piece>> get_n_shuffled_vectors(std::vector<Piece> pieces, unsigned int number_shuffles);
	};
}
