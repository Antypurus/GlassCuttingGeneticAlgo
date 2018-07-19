#include "Gene.h"
#include "piece.h";
#include <random>
#include "PiecesPlacement.h"
#include "RandomNumberGenerator.h"

namespace iart
{
	namespace GA
	{
		utils::RandomNumberGenerator Gene::randomMutationTypeGenerate = utils::RandomNumberGenerator(static_cast<unsigned long long>(1), static_cast<unsigned long long>(3));
		utils::RandomNumberGenerator Gene::randomMutationGenerate = utils::RandomNumberGenerator(static_cast<unsigned long long>(1), static_cast<unsigned long long>(100));

		Gene::Gene(const CHROMOSOMES &pieces)
		{
			this->m_pieces_ = pieces;
			this->randomPieceNumberGenerate = utils::RandomNumberGenerator(static_cast<unsigned long long>(0), pieces.size() - 1);
		}

		void Gene::calculateFitness(Board board)
		{
			PiecesPlacement place(this->m_pieces_,board);
			place.place();
			this->m_gene_fitness_ = place.evaluatePlacementGA();
		}

		Gene::FITNESS Gene::getGeneFitness() const
		{
			return this->m_gene_fitness_;
		}

		Gene::CHROMOSOMES& Gene::getChromosomes()
		{
			return this->m_pieces_;
		}

		void Gene::setGeneFitness(FITNESS fitness)
		{
			this->m_gene_fitness_ = fitness;
		}

		void Gene::mutate()
		{
			//check if is to mutate
			const short mutation = randomMutationGenerate(0l);
			if (mutation > 100 * MUTATION_PROBABILITY)
				return;
			
			const short mutationType = randomMutationTypeGenerate(0l);
			if (mutationType == 1)
				swapMutation();
			else if (mutationType == 2)
				moveMutation();
			else
				invertMutation();
		}

		bool Gene::operator<(const Gene &gene2) const
		{
			return m_gene_fitness_ < gene2.getGeneFitness();
		}

		void Gene::swapMutation()
		{
			const unsigned int index1 = randomPieceNumberGenerate(0l);
			unsigned int index2 = randomPieceNumberGenerate(0l);
			while(index2 == index1)
				index2 = randomPieceNumberGenerate(0l);

			iter_swap(m_pieces_.begin() + index1, m_pieces_.begin() + index2);
		}

		void Gene::moveMutation()
		{
			const unsigned int pieceToMoveCurrIndex = randomPieceNumberGenerate(0l);
			unsigned int pieceToMoveNewIndex = randomPieceNumberGenerate(0l);
			while(pieceToMoveCurrIndex == pieceToMoveNewIndex)
				pieceToMoveNewIndex = randomPieceNumberGenerate(0l);

			const Piece piece = m_pieces_[pieceToMoveCurrIndex];
			m_pieces_.insert(m_pieces_.begin() + pieceToMoveNewIndex, piece);
			vector<Piece>::iterator eraseIt;
			if (pieceToMoveCurrIndex < pieceToMoveNewIndex)
				eraseIt = m_pieces_.begin() + pieceToMoveCurrIndex;
			else
				eraseIt = m_pieces_.begin() + pieceToMoveCurrIndex + 1;
			m_pieces_.erase(eraseIt);
		}

		void Gene::invertMutation()
		{
			const unsigned int pieceToRotateIndex = randomPieceNumberGenerate(0l);
			m_pieces_[pieceToRotateIndex].rotate();
		}
	}
}
