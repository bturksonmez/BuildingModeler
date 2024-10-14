#pragma once

#include <random>
#include <vector>

#include "ILoadingGenerator.h"

namespace loadingGenerator
{
	class GravityLoadingGenerator : public ILoadingGenerator
	{
	private:
		double m_minDeadLoadFactor;
		double m_maxDeadLoadFactor;
		double m_minLiveLoadFactor;
		double m_maxLiveLoadFactor;
		long long m_seed;
		std::default_random_engine m_generator;

		void applyGravityLoads(json& buildingInfo);

	public:
		GravityLoadingGenerator(double minDeadLoadFactor, double maxDeadLoadFactor, double minLiveLoadFactor, double maxLiveLoadFactor);
		GravityLoadingGenerator() = delete;
		~GravityLoadingGenerator() = default;

		void load(json& buildingInfo) override;
	};
}