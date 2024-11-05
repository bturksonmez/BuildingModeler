#pragma once

#include <random>
#include <vector>

#include "ILoadingGenerator.h"

namespace loadingGenerator
{
	class ELFLoadingGenerator : public ILoadingGenerator
	{
	private:
		double m_minSpectralAcceleration;
		double m_maxSpectralAcceleration;
		long long m_seed;
		std::default_random_engine m_generator;

		void applyEarthquakeLoads(json& buildingInfo);

	public:
		ELFLoadingGenerator(double minSpectralAcceleration, double maxSpectralAcceleration);
		ELFLoadingGenerator() = delete;
		~ELFLoadingGenerator() = default;

		void load(json& buildingInfo) override;
	};
}