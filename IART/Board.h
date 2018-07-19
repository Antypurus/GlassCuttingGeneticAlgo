#pragma once

namespace iart 
{
	class Board
	{
	public:
		typedef double	BOARD_LENGTH;
		typedef double	BOARD_HEIGTH;
		typedef double	BOARD_AREA;
		typedef size_t	BOARD_NUMBER;
	private:
		BOARD_HEIGTH m_heigth_ = 0;
		BOARD_LENGTH m_length_ = 0;
		BOARD_NUMBER m_num_boards_ = 0;

	public:
		Board(){}
		Board(const BOARD_LENGTH length, const BOARD_HEIGTH heigth, const BOARD_NUMBER num_boards);
		BOARD_LENGTH get_length() const;
		BOARD_HEIGTH get_heigth() const;
		BOARD_NUMBER get_num_boards() const;
		BOARD_AREA get_board_area() const;
	};
}
