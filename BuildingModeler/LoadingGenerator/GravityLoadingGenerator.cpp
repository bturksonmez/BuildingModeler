#include "GravityLoadingGenerator.h"
#include "../BuildingModelerAPI.h"

#include <chrono>
#include <cmath>
#include <map>

using namespace loadingGenerator;

GravityLoadingGenerator::GravityLoadingGenerator(double minDeadLoadFactor, double maxDeadLoadFactor, double minLiveLoadFactor, double maxLiveLoadFactor)
{
	m_minDeadLoadFactor = minDeadLoadFactor;
	m_maxDeadLoadFactor = maxDeadLoadFactor;
	m_minLiveLoadFactor = minLiveLoadFactor;
	m_maxLiveLoadFactor = maxLiveLoadFactor;
}

void GravityLoadingGenerator::load(json& buildingInfo)
{
	static long long counter = 0;
	m_seed = std::chrono::system_clock::now().time_since_epoch().count() + counter++;
	m_generator.seed(m_seed);

	applyGravityLoads(buildingInfo);
}

void GravityLoadingGenerator::applyGravityLoads(json& buildingInfo)
{
	std::uniform_real_distribution<double> deadLoadFactorDist(m_minDeadLoadFactor, m_maxDeadLoadFactor);
	std::uniform_real_distribution<double> liveLoadFactorDist(m_minLiveLoadFactor, m_maxLiveLoadFactor);

	double deadLoadFactor = deadLoadFactorDist(m_generator);
	double liveLoadFactor = liveLoadFactorDist(m_generator);

	buildingInfo["loading"]["deadLoad"]["deadLoadFactor"] = deadLoadFactor;
	buildingInfo["loading"]["liveLoad"]["liveLoadFactor"] = liveLoadFactor;

	api::updateDeadAndLiveLoads();

	api::addStaticLoadCombination("gravity");
	api::addLoadCaseToStaticLoadCombination("gravity", "dead", deadLoadFactor);
	api::addLoadCaseToStaticLoadCombination("gravity", "live", liveLoadFactor);
	api::setStaticLoadCombinationActive("gravity", true);
}