#include "Piece.h"

namespace iart
{
	unsigned int Piece::currUniqueIdentifier = 1; // first id is 1

	Piece::Piece(const PIECE_LENGTH length, const PIECE_HEIGTH height, const PIECE_TYPE type)
	{
		this->uniqueIdentifier = this->currUniqueIdentifier;
		this->currUniqueIdentifier++;
		this->m_length_ = length;
		this->m_heigth_ = height;
		this->m_piece_type_ = type;
		this->m_paired_ = false;
		this->m_rotated_ = false;
	}

	Piece::Piece(const PIECE_LENGTH length, const PIECE_HEIGTH height, const std::string type, const char name)
	{
		this->uniqueIdentifier = this->currUniqueIdentifier;
		this->currUniqueIdentifier++;
		this->m_length_ = length;
		this->m_heigth_ = height;
		if (type == "quad")
		{
			this->m_piece_type_ = quad;
		}
		else if (type == "tri")
		{
			this->m_piece_type_ = tri;
		}

		this->m_paired_ = false;
		this->m_rotated_ = false;
		this->name = name;
	}

	unsigned int Piece::getUniqueIdentifier() const
	{
		return uniqueIdentifier;
	}

	Piece::PIECE_LENGTH Piece::get_length() const
	{
		if(m_rotated_)
			return this->m_heigth_;
		else
			return this->m_length_;
	}

	Piece::PIECE_HEIGTH Piece::get_heigth() const
	{
		if (m_rotated_)
			return this->m_length_;
		else
			return this->m_heigth_;
	}

	Piece::PIECE_TYPE Piece::get_type() const
	{
		return this->m_piece_type_;
	}

	char Piece::getName() const
	{
		return name;
	}

	Piece::PIECE_PAIRING Piece::is_paired() const
	{
		return this->m_paired_;
	}

	void Piece::set_paired(const Piece::PIECE_PAIRING paired)
	{
		this->m_paired_ = paired;
	}

	Piece::PIECE_ROTATION Piece::is_rotated()const
	{
		return this->m_rotated_;
	}

	void Piece::rotate()
	{
		if (this->m_rotated_)
		{
			this->m_rotated_ = false;
		}
		else
		{
			this->m_rotated_ = true;
		}
	}

	Piece::PIECE_AREA Piece::get_area() const
	{
		if (this->m_piece_type_ == quad)
		{
			return (this->m_heigth_*this->m_length_);
		}
		if (this->m_piece_type_ == tri)
		{
			return (this->m_heigth_*this->m_length_) / 2.0;
		}
		return 0.0;
	}

	bool Piece::operator== (const Piece &piece) const
	{
		if(piece.getUniqueIdentifier() == this->getUniqueIdentifier())
		{
			return true;
		}
		return false;
	}

	bool Piece::operator< (const Piece &piece) const
	{
		return this->uniqueIdentifier < piece.uniqueIdentifier;
	}
}