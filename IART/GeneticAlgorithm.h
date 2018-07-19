#pragma once
#include <vector>
#include "Board.h"
#include "RandomNumberGenerator.h"

namespace iart
{
	namespace GA
	{
		class Gene;
	}
	class Piece;
}

namespace iart
{
	class GeneticAlgorithm
	{
	public:
		double currPopFitness = 0.0;
		double currBestFitness = 0.0;
	private:
		std::vector<GA::Gene> m_genes_;
		Board m_board_;
		utils::RandomNumberGenerator m_generator_;		
		utils::RandomNumberGenerator randomCrossingPointGenerate;

	public:
		GeneticAlgorithm(std::vector<std::vector<Piece>> population, Board board);
		double CalculatePopulationFitness();
		std::vector<GA::Gene> crossoverPopulation();
		static GA::Gene crossingFunction(const GA::Gene& gene1, GA::Gene& gene2, unsigned long long crossingPoint);
		std::pair<GA::Gene, GA::Gene> crossGenes(const GA::Gene gene1, const GA::Gene gene2);
		std::vector<GA::Gene>& getGenes();
	};
}
