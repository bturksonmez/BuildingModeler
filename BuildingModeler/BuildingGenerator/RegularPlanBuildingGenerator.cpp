#include "RegularPlanBuildingGenerator.h"
#include "../BuildingModelerAPI.h"

#include <chrono>

using namespace buildingGenerator;

RegularPlanBuildingGenerator::RegularPlanBuildingGenerator(const Parameters& parameters, bool symmetricInXDir, bool symmetricInYDir) :
	m_parameters(parameters), m_symXDir(symmetricInXDir), m_symYDir(symmetricInYDir)
{
	m_symBothDir = m_symXDir && m_symYDir;

	void validateInput();
}

json RegularPlanBuildingGenerator::generate()
{
	static long long counter = 0;
	m_seed = std::chrono::system_clock::now().time_since_epoch().count() + counter++;
	m_generator.seed(m_seed);

	api::clear();

	json buildingInfo;
	generateBuildingPlan(buildingInfo);
	generateFloors(buildingInfo);
	generateJoints(buildingInfo);

	return buildingInfo;
}

void RegularPlanBuildingGenerator::validateInput()
{
	// To do: validate parameters input here
}

void RegularPlanBuildingGenerator::generateBuildingPlan(json& buildingInfo)
{
	std::uniform_int_distribution<int> numberOfBayDist(m_parameters.minNumberOfBays, m_parameters.maxNumberOfBays);
	std::uniform_real_distribution<double> bayWidthDist(m_parameters.minBayWidth, m_parameters.maxBayWidth);

	std::vector<double> bayWidthsX(m_parameters.maxNumberOfBays);
	std::vector<double> bayWidthsY(m_parameters.maxNumberOfBays);

	int numOfBaysX = numberOfBayDist(m_generator);
	int numOfBaysY = numberOfBayDist(m_generator);

	int diff = (numOfBaysX % 2 == 0 ? 1 : 2);
	for (int i = 0; i < numOfBaysX; ++i) {
		auto bayWidth = bayWidthDist(m_generator);

		if (i >= (numOfBaysX + 1) / 2 && m_symXDir) {
			bayWidthsX[i] = bayWidthsX[i - diff];
			diff += 2;
		}
		else {
			bayWidthsX[i] = bayWidth;
		}
	}

	diff = (numOfBaysY % 2 == 0 ? 1 : 2);
	for (int i = 0; i < numOfBaysY; ++i) {
		auto bayWidth = bayWidthDist(m_generator);

		if (i >= (numOfBaysY + 1) / 2 && m_symYDir) {
			bayWidthsY[i] = bayWidthsY[i - diff];
			diff += 2;
		}
		else {
			bayWidthsY[i] = bayWidth;
		}
	}

	buildingInfo["numberOfBaysX"] = numOfBaysX;
	buildingInfo["numberOfBaysY"] = numOfBaysY;
	buildingInfo["bayWidthsX"] = bayWidthsX;
	buildingInfo["bayWidthsY"] = bayWidthsY;
}

void RegularPlanBuildingGenerator::generateFloors(json& buildingInfo)
{
	std::uniform_int_distribution<int> numberOfStoreysDist(m_parameters.minNumberOfStoreys, m_parameters.maxNumberOfStoreys);
	std::uniform_real_distribution<double> firstStoreyHeightDist(m_parameters.minFirstStoreyHeight, m_parameters.maxFirstStoreyHeight);
	std::uniform_real_distribution<double> storeyHeightDist(m_parameters.minStoreyHeight, m_parameters.maxStoreyHeight);
	
	int ns = numberOfStoreysDist(m_generator);
	double H1 = firstStoreyHeightDist(m_generator);
	double H2 = storeyHeightDist(m_generator);

	buildingInfo["numberOfStoreys"] = ns;
	buildingInfo["firstStoreyHeight"] = H1;
	buildingInfo["storeyHeight"] = H2;

	api::addFloor(0, 0.0);
	auto currentHeight = H1;

	for (int i = 1; i <= ns; ++i) {
		api::addFloor(i, currentHeight);
		currentHeight += H2;
	}
}

void RegularPlanBuildingGenerator::generateJoints(json& buildingInfo)
{
	int ns = buildingInfo["numberOfStoreys"];
	double H1 = buildingInfo["firstStoreyHeight"];
	double H2 = buildingInfo["storeyHeight"];
	int numOfBaysX = buildingInfo["numberOfBaysX"];
	int numOfBaysY = buildingInfo["numberOfBaysY"];
	std::vector<double> bayWidthsX = buildingInfo["bayWidthsX"];
	std::vector<double> bayWidthsY = buildingInfo["bayWidthsY"];

	int jointTag = 1;
	double coordZ = 0.0;
	for (int i = 0; i <= ns; ++i) {

		double coordY = 0.0;
		for (int j = 0; j <= numOfBaysY; ++j) {

			double coordX = 0.0;
			for (int k = 0; k <= numOfBaysX; ++k) {

				api::addJoint(jointTag, { coordX, coordY, coordZ });
				api::setFloorNo(jointTag, i);

				if (i == 0) {
					api::setConstraintVector(jointTag, { 1, 1, 1, 1, 1, 1 });
				}

				coordX += bayWidthsX[k];
				jointTag++;
			}

			coordY += bayWidthsY[j];
		}

		if (i == 0) {
			coordZ += H1;
		}
		else {
			coordZ += H2;
		}
	}
}