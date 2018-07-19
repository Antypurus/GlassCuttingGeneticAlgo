#include "RandomNumberGenerator.h"

namespace iart
{
	namespace utils
	{
		RandomNumberGenerator::RandomNumberGenerator() = default;

		RandomNumberGenerator::RandomNumberGenerator(const unsigned long long lowerBound, const unsigned long long upperBound)
		{
			initMerseneTwister();
			this->m_int_distribution_ = INT_DISTRIBUTION(lowerBound, upperBound);
		}

		RandomNumberGenerator::RandomNumberGenerator(const double lowerBound, const double upperBound)
		{
			initMerseneTwister();
			this->m_real_distribution_ = REAL_DISTRIBUTION(lowerBound, upperBound);
		}

		unsigned long long RandomNumberGenerator::operator()(int limit)
		{
			return this->m_int_distribution_(this->m_twister_);
		}

		double RandomNumberGenerator::operator()(double limit) 
		{
			return this->m_real_distribution_(this->m_twister_);
		}

		inline void RandomNumberGenerator::initMerseneTwister()
		{
			static std::random_device generator;
			static std::mt19937_64 mersenne_twister(generator());
			this->m_twister_ = mersenne_twister;
		}
	}
}
