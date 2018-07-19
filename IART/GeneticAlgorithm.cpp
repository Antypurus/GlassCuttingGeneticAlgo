#include "GeneticAlgorithm.h"
#include "Gene.h"
#include "piece.h"
#include <thread>
#include "RandomNumberGenerator.h"
#include "Roulete.h"
#include <set>
#include <iostream>
#include <algorithm>

using std::thread;
using std::vector;
using iart::GA::Gene;


iart::GeneticAlgorithm::GeneticAlgorithm(vector<vector<Piece>> population, Board board)
{
	const size_t populationSize = population.size();
	for (size_t i = 0; i < populationSize; i++)
	{
		const Gene gene = Gene(population[i]);
		m_genes_.push_back(gene);
	}
	this->m_board_ = board;
	this->m_generator_ = utils::RandomNumberGenerator(0.0, 1.0);
	if(populationSize != 0)
		randomCrossingPointGenerate = utils::RandomNumberGenerator(static_cast<unsigned long long>(0), population[0].size());
}

double iart::GeneticAlgorithm::CalculatePopulationFitness()
{
	double fitness = 0.0;
	std::vector<thread> threads;
	for (auto& i : this->m_genes_)
	{
		i.calculateFitness(this->m_board_);
	}
	for (size_t i = 0; i < threads.size(); ++i)
	{
		threads[i].join();
	}
	for (auto& i : this->m_genes_)
	{
		fitness += i.getGeneFitness();
	}
	return fitness;
}

#define elitism_ammount 4

std::vector<Gene> iart::GeneticAlgorithm::crossoverPopulation() //first element of return is the best Gene of this population, but the others are not ordered
{
	const double popFitness = this->CalculatePopulationFitness();
	std::sort(this->m_genes_.begin(), this->m_genes_.end());
	this->currBestFitness = this->m_genes_[this->m_genes_.size()-1].getGeneFitness();
	this->currPopFitness = popFitness / this->m_genes_.size();
	utils::Roulete roulete(this->m_genes_, popFitness);
	std::vector<Gene> newGenes;
	newGenes.reserve(this->m_genes_.size());
	unsigned int bestGeneValue = -1;
	size_t bestGeneIndex;

	for(unsigned int i=0;i<elitism_ammount;++i)
	{
		newGenes.emplace_back(this->m_genes_[this->m_genes_.size() - 1 - i]);
	}

	for (size_t i = 0; i < ((this->m_genes_.size()- elitism_ammount) / 2); ++i)
	{
		const double val1 = m_generator_(1.0);
		const double val2 = m_generator_(1.0);
		const Gene gene1 = roulete.getGene(val1);
		const Gene gene2 = roulete.getGene(val2);
		const std::pair<Gene, Gene> crossedGenes = crossGenes(gene1, gene2);
		Gene crossedGene1 = crossedGenes.first;
		crossedGene1.mutate();
		crossedGene1.calculateFitness(this->m_board_);
		Gene crossedGene2 = crossedGenes.second;
		crossedGene2.mutate();
		crossedGene2.calculateFitness(this->m_board_);
		newGenes.push_back(crossedGene1);
		newGenes.push_back(crossedGene2);

		if (crossedGene1.getGeneFitness() > bestGeneValue)
		{
			bestGeneIndex = newGenes.size() - 1 - 1;
			bestGeneValue = crossedGene1.getGeneFitness();
		}
		if (crossedGene2.getGeneFitness() > bestGeneValue)
		{
			bestGeneIndex = newGenes.size() - 1;
			bestGeneValue = crossedGene2.getGeneFitness();
		}
	}

	if (bestGeneValue != -1)
		std::move(newGenes.begin() + bestGeneIndex, newGenes.begin() + bestGeneIndex + 1, newGenes.begin());

	this->m_genes_ = newGenes;
	return newGenes;
}

std::pair<Gene, Gene> iart::GeneticAlgorithm::crossGenes(Gene gene1, Gene gene2)
{
	const unsigned long long int crossingPoint = randomCrossingPointGenerate(static_cast<int>(0));

	std::set<unsigned int> alreadyPlacedChromosomes;
	std::set<unsigned int> alreadyPlacedChromosomes2;

	Gene::CHROMOSOMES newChroms1(gene1.getChromosomes().size());
	Gene::CHROMOSOMES newChroms2(gene1.getChromosomes().size());

	for (unsigned int i = 0; i<crossingPoint; ++i)
	{
		newChroms1[i] = gene2.getChromosomes()[i];
		newChroms2[i] = gene1.getChromosomes()[i];
		alreadyPlacedChromosomes.insert(gene2.getChromosomes()[i].getUniqueIdentifier());
		alreadyPlacedChromosomes2.insert(gene1.getChromosomes()[i].getUniqueIdentifier());
	}

	std::vector<unsigned int>unfiledPositions;
	for (unsigned int i = crossingPoint; i<gene2.getChromosomes().size(); ++i)
	{
		//check if can execure the switch
		const auto ret = alreadyPlacedChromosomes.insert(gene2.getChromosomes()[i].getUniqueIdentifier());
		if (ret.second == false)
		{
			unfiledPositions.emplace_back(i);
			continue;
		}
		const auto ret2 = alreadyPlacedChromosomes2.insert(gene1.getChromosomes()[i].getUniqueIdentifier());
		if (ret2.second == false)
		{
			unfiledPositions.emplace_back(i);
			alreadyPlacedChromosomes.erase(ret.first);
			continue;
		}

		//can switch therefore switch
		newChroms1[i] = gene2.getChromosomes()[i];
		newChroms2[i] = gene1.getChromosomes()[i];
	}

	std::vector<Piece>missingChromsgene1;
	//check the missing chromosome for gene 1
	size_t size = gene1.getChromosomes().size();
	for (unsigned int i = 0; i<size; ++i)
	{
		auto ret = std::find(newChroms1.begin(), newChroms1.end(), gene1.getChromosomes()[i]);
		if (ret == newChroms1.end())
		{
			missingChromsgene1.push_back(gene1.getChromosomes()[i]);
		}
	}

	std::vector<Piece>missingChromsgene2;
	//check the missing chromosome for gene 2
	size = gene2.getChromosomes().size();
	for (unsigned int i = 0; i<size; ++i)
	{
		auto ret = std::find(newChroms2.begin(), newChroms2.end(), gene2.getChromosomes()[i]);
		if (ret == newChroms2.end())
		{
			missingChromsgene1.push_back(gene2.getChromosomes()[i]);
		}
	}

	//fill out the missing chromosome for gene 1
	for (unsigned int i = 0; i<unfiledPositions.size(); ++i)
	{
		const unsigned int index = unfiledPositions[i];
		newChroms1[index] = missingChromsgene1[i];
	}

	//fill out the missing chromosome for gene 2
	for (unsigned int i = 0; i<unfiledPositions.size(); ++i)
	{
		const unsigned int index = unfiledPositions[i];
		newChroms2[index] = missingChromsgene2[i];
	}

	Gene crossedGene1(newChroms1);
	Gene crossedGene2(newChroms2);

	return std::pair<Gene, Gene>(crossedGene1, crossedGene2);
}

std::vector<Gene>& iart::GeneticAlgorithm::getGenes()
{
	return this->m_genes_;
}

Gene iart::GeneticAlgorithm::crossingFunction(const Gene &gene1, Gene &gene2, const unsigned long long int crossingPoint)
{
	Gene crossedGene = Gene(gene1);
	Gene::CHROMOSOMES &crossedGeneChromosomes = crossedGene.getChromosomes();
	Gene::CHROMOSOMES &gene2Chromosomes = gene2.getChromosomes();
	const size_t numChromosomes = crossedGeneChromosomes.size();
	std::set<unsigned int> alreadyPlacedChromosomes;
	
	//set chromosomes until the crossingPoint
	for (size_t i = 0; i < crossingPoint; i++)
	{
		alreadyPlacedChromosomes.insert(crossedGeneChromosomes[i].getUniqueIdentifier());
	}


	//set chromosomes after the crossingPoint that can be set, i.e., if chromosomes repeated do not insert them
	vector<unsigned int> indexesNotFilled;
	for (size_t i = crossingPoint; i < numChromosomes; i++)
	{
		const std::pair<std::set<unsigned int>::iterator, bool> returnPair = alreadyPlacedChromosomes.insert(gene2Chromosomes[i].getUniqueIdentifier());
		if (returnPair.second == false)
			indexesNotFilled.push_back(i);
		else
			crossedGeneChromosomes[i] = gene2Chromosomes[i];
	}
	const unsigned int indexesNotFilledSize = indexesNotFilled.size();
	for (size_t i = 0; i < indexesNotFilledSize; i++)
	{
		crossedGeneChromosomes[indexesNotFilled[i]] = gene2Chromosomes[i];
	}
	return crossedGene;
}
