#include "Roulete.h"
#include "Gene.h"
#include <iostream>

namespace iart
{
	namespace utils
	{
		Roulete::Roulete(std::vector<GA::Gene>& genes, double totalPopulationFitness)
		{
			this->m_genes_[0] = &GA::Gene();
			for(auto&&gene:genes)
			{
				const double fraction = gene.getGeneFitness() / totalPopulationFitness;
				this->addGene(gene, fraction);
			}
		}

		GA::Gene Roulete::getGene(double d)
		{
			auto ret = this->m_genes_.upper_bound(d);
			if(ret!=this->m_genes_.end())
			{
				return *ret->second;
			}
			return GA::Gene();
		}

		void Roulete::addGene(GA::Gene& gene, double fitnessfraction)
		{
			this->lastLimit += fitnessfraction;
			this->m_genes_[this->lastLimit] = &gene;
		}
	}
}
