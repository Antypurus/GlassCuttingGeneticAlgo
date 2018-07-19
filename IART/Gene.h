#pragma once
#include <vector>
#include "Board.h"
#include "RandomNumberGenerator.h"

#define MUTATION_PROBABILITY 0.2

namespace iart
{
	class Piece;
}

namespace iart
{
	namespace GA
	{
		class Gene
		{
		public:
			typedef std::vector<iart::Piece> CHROMOSOMES;
			typedef double FITNESS;
		private:
			CHROMOSOMES m_pieces_;
			FITNESS	m_gene_fitness_;
			utils::RandomNumberGenerator randomPieceNumberGenerate;
			static utils::RandomNumberGenerator randomMutationTypeGenerate;
			static utils::RandomNumberGenerator randomMutationGenerate;
			
		public:
			Gene(){}
			Gene(const CHROMOSOMES& pieces);
			void calculateFitness(Board board);
			FITNESS getGeneFitness()const;
			CHROMOSOMES& getChromosomes();
			void setGeneFitness(FITNESS fitness);
			void swapMutation();
			void moveMutation();
			void invertMutation();
			void mutate();
			bool operator<(const Gene& gene2) const;
		};
	}
}