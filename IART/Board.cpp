#include "Board.h"

namespace iart
{
	Board::Board(const BOARD_LENGTH length, const BOARD_HEIGTH heigth, const BOARD_NUMBER num_boards)
	{
		this->m_length_ = length;
		this->m_heigth_ = heigth;
		this->m_num_boards_ = num_boards;
	}

	Board::BOARD_LENGTH Board::get_length() const
	{
		return this->m_length_;
	}

	Board::BOARD_HEIGTH Board::get_heigth() const
	{
		return this->m_heigth_;
	}

	Board::BOARD_NUMBER Board::get_num_boards() const
	{
		return this->m_num_boards_;
	}

	Board::BOARD_AREA Board::get_board_area() const
	{
		return this->m_heigth_ * this->m_length_;
	}
}