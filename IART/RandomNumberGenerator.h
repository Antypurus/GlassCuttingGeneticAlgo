#pragma once
#include <random>

namespace iart
{
	namespace utils
	{
		class RandomNumberGenerator
		{
		public:
			typedef std::uniform_int_distribution<unsigned long long> INT_DISTRIBUTION;
			typedef std::uniform_real_distribution<double> REAL_DISTRIBUTION;
		private:
			std::mt19937_64 m_twister_;
			INT_DISTRIBUTION m_int_distribution_;
			REAL_DISTRIBUTION m_real_distribution_;
		public:
			RandomNumberGenerator();
			RandomNumberGenerator(const unsigned long long lowerBound, const unsigned long long upperBound);
			RandomNumberGenerator(const double lowerBound, const double upperBound);
			unsigned long long operator()(int limit = 0);
			double operator()(double limit = 0.0);
		private:
			inline void initMerseneTwister();
		};
	}
}
