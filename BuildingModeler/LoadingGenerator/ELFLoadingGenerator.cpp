#include "ELFLoadingGenerator.h"
#include "../BuildingModelerAPI.h"

#include <chrono>
#include <cmath>
#include <map>

using namespace loadingGenerator;

ELFLoadingGenerator::ELFLoadingGenerator(double minSpectralAcceleration, double mmaxSpectralAcceleration)
{
	m_minSpectralAcceleration = minSpectralAcceleration;
	m_maxSpectralAcceleration = mmaxSpectralAcceleration;
}

void ELFLoadingGenerator::load(json& buildingInfo)
{
	static long long counter = 0;
	m_seed = std::chrono::system_clock::now().time_since_epoch().count() + counter++;
	m_generator.seed(m_seed);

	applyEarthquakeLoads(buildingInfo);
}

void ELFLoadingGenerator::applyEarthquakeLoads(json& buildingInfo)
{
	std::uniform_real_distribution<double> spectralAccelerationDist(m_minSpectralAcceleration, m_maxSpectralAcceleration);

    auto sA = spectralAccelerationDist(m_generator);
    auto baseShear = sA * api::getBuildingWeight();
    buildingInfo["loading"]["earthquakeLoad"]["baseShear"] = baseShear;

    int ns = buildingInfo["numberOfStoreys"];

    double totalMoment = 0;
    for (int i = 1; i <= ns; ++i) {
        totalMoment += api::getDiaphragmMass(i).value().x * api::getFloorHeight(i);
    }

    api::addStaticLoadCase("earthquake", physicalModel::StaticLoadCaseType::EARTHQUAKE);
    for (int i = 1; i <= ns; ++i) {
        auto eqLoad = baseShear * api::getDiaphragmMass(i).value().x * api::getFloorHeight(i) / totalMoment;
        api::addPointLoad("earthquake", api::getMasterJointTag(i), eqLoad, 0.0, 0.0, 0.0, 0.0, 0.0);
    }

	api::setLoadCaseActive("earthquake", true);
}