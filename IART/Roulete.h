#pragma once
#include <map>
#include <vector>

#include "piece.h"

namespace iart
{
	namespace GA
	{
		class Gene;
	}
	namespace utils
	{
		class Roulete
		{
		private:
			double lastLimit = 0.0;
			std::map<double, GA::Gene*> m_genes_;
		public:
			Roulete(){}
			Roulete(std::vector<GA::Gene>& genes, double totalPopulationFitness);
			GA::Gene getGene(double d);
			void addGene(GA::Gene& gene,double fitnessfraction);
		};
	}
}
