#pragma once
#include <string>

namespace iart 
{
	class Piece
	{
	public:
		typedef double	PIECE_LENGTH;
		typedef double	PIECE_HEIGTH;
		typedef double	PIECE_AREA;
		typedef bool	PIECE_ROTATION;
		typedef bool	PIECE_PAIRING;
		enum PIECE_TYPE { quad, tri };
	private:
		static unsigned int currUniqueIdentifier;
		unsigned int uniqueIdentifier = 0;
		PIECE_LENGTH	m_length_;
		PIECE_HEIGTH	m_heigth_;
		PIECE_TYPE		m_piece_type_;
		PIECE_ROTATION	m_rotated_;
		PIECE_PAIRING	m_paired_;
		char			name;
	public:
		Piece(){}
		Piece(const PIECE_LENGTH length, const PIECE_HEIGTH height, const PIECE_TYPE type);
		Piece(const PIECE_LENGTH length, const PIECE_HEIGTH height, const std::string type, const char name);
		unsigned int getUniqueIdentifier() const;
		PIECE_LENGTH get_length() const;
		PIECE_HEIGTH get_heigth() const;
		PIECE_TYPE get_type() const;
		char getName() const;
		PIECE_PAIRING is_paired() const;
		void set_paired(const PIECE_PAIRING paired);
		PIECE_ROTATION is_rotated()const;
		void rotate();
		PIECE_AREA get_area() const;

		bool operator== (const Piece &piece) const;
		bool operator< (const Piece &piece) const;
	};
}
