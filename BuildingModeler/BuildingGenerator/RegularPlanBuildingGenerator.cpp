#include "RegularPlanBuildingGenerator.h"
#include "../BuildingModelerAPI.h"

#include <chrono>
#include <cmath>
#include <map>
#include <unordered_map>

using namespace buildingGenerator;

RegularPlanBuildingGenerator::RegularPlanBuildingGenerator(const Parameters& parameters) : m_parameters(parameters)
{
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
	generateMaterials(buildingInfo);
	generateShearWalls(buildingInfo);
	generateSlabs(buildingInfo);
	generateColumns(buildingInfo);

	return buildingInfo;
}

void RegularPlanBuildingGenerator::validateInput()
{
	// To do: validate parameters input here
}

void RegularPlanBuildingGenerator::generateBuildingPlan(json& buildingInfo)
{
	std::uniform_int_distribution<int> numberOfBayDist(m_parameters.geometricParameters.minNumberOfBays, m_parameters.geometricParameters.maxNumberOfBays);
	std::uniform_real_distribution<double> bayWidthDist(m_parameters.geometricParameters.minBayWidth, m_parameters.geometricParameters.maxBayWidth);

	std::vector<double> bayWidthsX(m_parameters.geometricParameters.maxNumberOfBays);
	std::vector<double> bayWidthsY(m_parameters.geometricParameters.maxNumberOfBays);

	int numOfBaysX = numberOfBayDist(m_generator);
	int numOfBaysY = numberOfBayDist(m_generator);

	int diff = (numOfBaysX % 2 == 0 ? 1 : 2);
	for (int i = 0; i < numOfBaysX; ++i) {
		auto bayWidth = bayWidthDist(m_generator);

		if (i >= (numOfBaysX + 1) / 2) {
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

		if (i >= (numOfBaysY + 1) / 2) {
			bayWidthsY[i] = bayWidthsY[i - diff];
			diff += 2;
		}
		else {
			bayWidthsY[i] = bayWidth;
		}
	}

	double lengthX = 0.0;
	for (int i = 0; i < numOfBaysX; ++i) {
		lengthX += bayWidthsX[i];
	}

	double lengthY = 0.0;
	for (int i = 0; i < numOfBaysY; ++i) {
		lengthY += bayWidthsY[i];
	}

	buildingInfo["numberOfBaysX"] = numOfBaysX;
	buildingInfo["numberOfBaysY"] = numOfBaysY;
	buildingInfo["bayWidthsX"] = bayWidthsX;
	buildingInfo["bayWidthsY"] = bayWidthsY;
	buildingInfo["planArea"] = lengthX * lengthY;

	for (int i = 0; i <= m_parameters.geometricParameters.maxNumberOfBays; ++i) {
		buildingInfo["verticalMemberPlan"][std::to_string(i)] = std::vector<int>(m_parameters.geometricParameters.maxNumberOfBays + 1, 0);
	}
}

void RegularPlanBuildingGenerator::generateFloors(json& buildingInfo)
{
	std::uniform_int_distribution<int> numberOfStoreysDist(m_parameters.geometricParameters.minNumberOfStoreys, m_parameters.geometricParameters.maxNumberOfStoreys);
	std::uniform_real_distribution<double> storeyHeightDist(m_parameters.geometricParameters.minStoreyHeight, m_parameters.geometricParameters.maxStoreyHeight);

	int ns = numberOfStoreysDist(m_generator);
	double H2 = storeyHeightDist(m_generator);

	std::uniform_real_distribution<double> firstStoreyHeightDist(H2, m_parameters.geometricParameters.maxFirstStoreyHeight);
	double H1 = firstStoreyHeightDist(m_generator);
	
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

				if (k < numOfBaysX) {
					coordX += bayWidthsX[k];
				}
				
				jointTag++;
			}

			if (j < numOfBaysY) {
				coordY += bayWidthsY[j];
			}
		}

		if (i == 0) {
			coordZ += H1;
		}
		else {
			coordZ += H2;
		}
	}
}

void RegularPlanBuildingGenerator::generateMaterials(json& buildingInfo)
{
	std::uniform_real_distribution<double> youngsModulusDist(m_parameters.minConcreteYoungsModulus, m_parameters.maxConcreteYoungsModulus);
	std::uniform_real_distribution<double> shearWallCrackedSectionModifierDist(m_parameters.shearWallParameters.minShearWallCrackedSectionModifier, m_parameters.shearWallParameters.maxShearWallCrackedSectionModifier);

	double E = youngsModulusDist(m_generator);
	double crackedMod = shearWallCrackedSectionModifierDist(m_generator);
	double Ecracked = crackedMod * E;

	double G = E / (1.0 + 0.2) / 2.0;
	double Gcracked = E / (1.0 + 0.2) / 2.0;

	api::addElasticMaterial(1, E, G, 2.4);
	api::addElasticMaterial(2, Ecracked, Gcracked, 2.4); // for shear walls only

	buildingInfo["youngsModulus"] = E;
	buildingInfo["shearWall"]["crackedSectionModifier"] = crackedMod;
}

void RegularPlanBuildingGenerator::generateShearWalls(json& buildingInfo)
{
	int ns = buildingInfo["numberOfStoreys"];
	int numOfBaysX = buildingInfo["numberOfBaysX"];
	int numOfBaysY = buildingInfo["numberOfBaysY"];
	std::vector<double> bayWidthsX = buildingInfo["bayWidthsX"];
	std::vector<double> bayWidthsY = buildingInfo["bayWidthsY"];

	std::uniform_real_distribution<double> shearWallThicknessDist(m_parameters.shearWallParameters.minShearWallThickness, m_parameters.shearWallParameters.maxShearWallThickness);
	double t = shearWallThicknessDist(m_generator);
	buildingInfo["shearWall"]["thickness"] = t;

	double planArea = buildingInfo["planArea"];
	
	double shearWallRatioX;
	std::vector<std::vector<int>> shearWallArrangementXDir;
	if (m_parameters.shearWallParameters.includeShearWallInXDir) {
		shearWallArrangementXDir = getShearWallArrangementInLongitudinalDir(numOfBaysX, numOfBaysY, bayWidthsX, bayWidthsY, t, shearWallRatioX);
		buildingInfo["shearWall"]["shearWallXDir"]["ratio"] = shearWallRatioX;
		buildingInfo["shearWall"]["shearWallXDir"]["area"] = shearWallRatioX * planArea;
		buildingInfo["shearWall"]["arrangementX"] = shearWallArrangementXDir[0];
		buildingInfo["shearWall"]["arrangementY"] = shearWallArrangementXDir[1];
	}

	api::addElasticSection2D(301, 2, t);

	double totalIx = 0.0;
	int numOfJointsPerFloor = (numOfBaysX + 1) * (numOfBaysY + 1);
	for (int i = 0; i < ns; ++i) {

		int elementTag = 30100 + 100 * i + 1;
		for (int j = 0; j < shearWallArrangementXDir[1].size(); ++j) {

			for (int k = 0; k < shearWallArrangementXDir[0].size(); ++k) {

				if (1 == shearWallArrangementXDir[0][k] && 1 == shearWallArrangementXDir[1][j]) {

					int initJointTag = i * numOfJointsPerFloor + 1;
					int jointITag = initJointTag + (numOfBaysX + 1) * j + k;
					int jointJTag = initJointTag + (numOfBaysX + 1) * j + k + 1;
					int jointKTag = initJointTag + (numOfBaysX + 1) * j + k + 1 + numOfJointsPerFloor;
					int jointLTag = initJointTag + (numOfBaysX + 1) * j + k + numOfJointsPerFloor;

					api::addShearWall(elementTag, { jointITag, jointJTag, jointKTag, jointLTag }, 301, physicalModel::AreaElementFormulation::LINEAR);

					if (0 == i) {
						buildingInfo["verticalMemberPlan"][std::to_string(j)][k] = elementTag;
						buildingInfo["verticalMemberPlan"][std::to_string(j)][k+1] = elementTag;

						auto coordI = api::getCoordinates(jointITag);
						auto coordJ = api::getCoordinates(jointJTag);
						auto length = (coordJ - coordI).norm2();
						auto I = (1.0 / 12.0) * t * std::pow(length, 3);
						totalIx += I;
					}

					++elementTag;
				}
			}
		}
	}

	buildingInfo["shearWall"]["shearWallXDir"]["momentOfInertia"] = totalIx;
}

void RegularPlanBuildingGenerator::generateSlabs(json& buildingInfo)
{
	int ns = buildingInfo["numberOfStoreys"];
	int numOfBaysX = buildingInfo["numberOfBaysX"];
	int numOfBaysY = buildingInfo["numberOfBaysY"];

	std::uniform_real_distribution<double> slabThicknessDist(m_parameters.slabParameters.minSlabThickness, m_parameters.slabParameters.maxSlabThickness);
	double t = slabThicknessDist(m_generator);
	buildingInfo["slab"]["thickness"] = t;

	api::addElasticSection2D(401, 1, t);

	int numOfJointsPerFloor = (numOfBaysX + 1) * (numOfBaysY + 1);
	for (int i = 0; i < ns; ++i) {

		int elementTag = 40100 + 100 * i + 1;
		for (int j = 0; j < numOfBaysY; ++j) {

			for (int k = 0; k < numOfBaysX; ++k) {

				int initJointTag = (i + 1) * numOfJointsPerFloor + 1;
				int jointITag = initJointTag + (numOfBaysX + 1) * j + k;
				int jointJTag = initJointTag + (numOfBaysX + 1) * j + k + 1;
				int jointKTag = initJointTag + (numOfBaysX + 1) * (j + 1) + k + 1;
				int jointLTag = initJointTag + (numOfBaysX + 1) * (j + 1) + k;

				api::addSlab(elementTag, { jointITag, jointJTag, jointKTag, jointLTag }, 401, physicalModel::AreaElementFormulation::LINEAR);

				++elementTag;
			}
		}
	}
}

void RegularPlanBuildingGenerator::generateColumns(json& buildingInfo)
{
	int ns = buildingInfo["numberOfStoreys"];
	int numOfBaysX = buildingInfo["numberOfBaysX"];
	int numOfBaysY = buildingInfo["numberOfBaysY"];
	std::vector<int> shearWallArrangementX = buildingInfo["shearWall"]["arrangementX"];
	std::vector<int> shearWallArrangementY = buildingInfo["shearWall"]["arrangementY"];

	std::vector<int> modifiedShearWallArrangementX;
	modifiedShearWallArrangementX.push_back(shearWallArrangementX[0] ? 1 : 0);
	for (int i = 1; i < numOfBaysX; ++i) {

		if (shearWallArrangementX[i] || shearWallArrangementX[i - 1]) {
			modifiedShearWallArrangementX.push_back(1);
		}
		else {
			modifiedShearWallArrangementX.push_back(0);
		}
	}
	modifiedShearWallArrangementX.push_back(shearWallArrangementX[numOfBaysX - 1] ? 1 : 0);

	std::uniform_int_distribution<int> columnOrientationDist(0, 1);
	std::uniform_real_distribution<double> maxAspectRatioDist(1.0, m_parameters.columnParameters.maxAspectRatioForColumns);
	std::uniform_real_distribution<double> maxAreaRatioInnerToOuterDist(1.0, m_parameters.columnParameters.maxAreaRatioInnerToOuterColumns);
	std::uniform_real_distribution<double> equivalentSquareColumnWidthDist(m_parameters.columnParameters.minEquivalentSquareColumnWidth, m_parameters.columnParameters.maxEquivalentSquareColumnWidth);
	std::uniform_real_distribution<double> columnCrackedSectionModifierDist(m_parameters.columnParameters.minColumnCrackedSectionModifier, m_parameters.columnParameters.maxColumnCrackedSectionModifier);
	
	double aspectRatio = maxAspectRatioDist(m_generator);
	double areaRatioInnerToOuter = maxAreaRatioInnerToOuterDist(m_generator);
	double squareWidth = equivalentSquareColumnWidthDist(m_generator);
	double crackedMod = columnCrackedSectionModifierDist(m_generator);

	double widthOuterS = squareWidth / std::sqrt(aspectRatio);
	double widthOuterL = squareWidth * std::sqrt(aspectRatio);
	double widthInnerS = widthOuterS * std::sqrt(areaRatioInnerToOuter);
	double widthInnerL = widthOuterL * std::sqrt(areaRatioInnerToOuter);
	buildingInfo["column"]["outerColumns"]["width"] = widthOuterS;
	buildingInfo["column"]["outerColumns"]["depth"] = widthOuterL;
	buildingInfo["column"]["innerColumns"]["width"] = widthInnerS;
	buildingInfo["column"]["innerColumns"]["depth"] = widthInnerL;
	buildingInfo["column"]["crackedMod"] = crackedMod;

	// To do: array yap bunlari
	api::addElasticSection1D(101, 1, new physicalModel::Rectangle(widthOuterL, widthOuterS)); // outer column strong in x direction
	api::addElasticSection1D(102, 1, new physicalModel::Rectangle(widthOuterS, widthOuterL)); // outer column weak in x direction
	api::addElasticSection1D(111, 1, new physicalModel::Rectangle(widthInnerL, widthInnerS)); // inner column strong in x direction
	api::addElasticSection1D(112, 1, new physicalModel::Rectangle(widthInnerS, widthInnerL)); // inner column weak in x direction

	double totalIx = 0.0;
	double totalA = 0.0;

	int numOfJointsPerFloor = (numOfBaysX + 1) * (numOfBaysY + 1);
	for (int i = 0; i < ns; ++i) {

		int elementTag = 10100 + 100 * i + 1;
		for (int j = 0; j <= numOfBaysY / 2; ++j) {

			for (int k = 0; k <= numOfBaysX; ++k) {

				if (1 != shearWallArrangementY[j] || 1 != modifiedShearWallArrangementX[k]) {

					int sectionTag;

					if (j == 0 || k == 0 || k == numOfBaysX) {

						sectionTag = columnOrientationDist(m_generator) ? 101 : 102;
					}
					else {

						sectionTag = columnOrientationDist(m_generator) ? 111 : 112;
					}

					if (2 * j == numOfBaysY) {

						int jointITag = i * numOfJointsPerFloor + 1 + (numOfBaysX + 1) * j + k;
						int jointJTag = i * numOfJointsPerFloor + 1 + (numOfBaysX + 1) * j + k + numOfJointsPerFloor;
						api::addColumn(elementTag, { jointITag, jointJTag }, sectionTag, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
						api::setSectionModifiers(elementTag, 0, 1.0, 1.0, crackedMod, 1.0);

						if (m_parameters.meshInfo.meshColumn) {

							int numOfSegments = api::getLength(elementTag) / m_parameters.meshInfo.meshSensitivity + 1;
							std::vector<double> segmentRatios(numOfSegments, api::getLength(elementTag) / (double)numOfSegments);
							api::setSegmentRatios(elementTag, segmentRatios);
						}

						if (0 == i) {

							buildingInfo["verticalMemberPlan"][std::to_string(j)][k] = elementTag;

							totalA += api::getArea(elementTag);
							totalIx += api::getMomentOfInertiaZZ(elementTag);
						}

						++elementTag;
					}
					else {

						int jointITag1 = i * numOfJointsPerFloor + 1 + (numOfBaysX + 1) * j + k;
						int jointJTag1 = i * numOfJointsPerFloor + 1 + (numOfBaysX + 1) * j + k + numOfJointsPerFloor;
						int elementTag1 = elementTag++;
						api::addColumn(elementTag1, { jointITag1, jointJTag1 }, sectionTag, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
						
						int jointITag2 = i * numOfJointsPerFloor + 1 + (numOfBaysX + 1) * (numOfBaysY - j) + k;
						int jointJTag2 = i * numOfJointsPerFloor + 1 + (numOfBaysX + 1) * (numOfBaysY - j) + k + numOfJointsPerFloor;
						int elementTag2 = elementTag++;
						api::addColumn(elementTag2, { jointITag2, jointJTag2 }, sectionTag, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);

						if (0 == i) {

							buildingInfo["verticalMemberPlan"][std::to_string(j)][k] = elementTag1;
							buildingInfo["verticalMemberPlan"][std::to_string(numOfBaysY - j)][k] = elementTag2;

							totalA += api::getArea(elementTag1);
							totalA += api::getArea(elementTag2);

							totalIx += api::getMomentOfInertiaZZ(elementTag1);
							totalIx += api::getMomentOfInertiaZZ(elementTag2);
						}
					}	
				}
			}
		}
	}

	buildingInfo["column"]["area"] = totalA;
	buildingInfo["column"]["momentOfInertia"] = totalIx;
}

void RegularPlanBuildingGenerator::generateBeams(json& buildingInfo)
{
	int ns = buildingInfo["numberOfStoreys"];
	int numOfBaysX = buildingInfo["numberOfBaysX"];
	int numOfBaysY = buildingInfo["numberOfBaysY"];
	std::vector<double> bayWidthsX = buildingInfo["bayWidthsX"];
	std::vector<double> bayWidthsY = buildingInfo["bayWidthsY"];
	std::vector<int> shearWallArrangementX = buildingInfo["shearWall"]["arrangementX"];
	std::vector<int> shearWallArrangementY = buildingInfo["shearWall"]["arrangementY"];

	std::uniform_real_distribution<double> beamWidthDist(m_parameters.beamParameters.minBeamWidth, m_parameters.beamParameters.maxBeamWidth);
	std::uniform_real_distribution<double> equivalentBeamDepthDist(m_parameters.beamParameters.minBeamDepth, m_parameters.beamParameters.maxBeamDepth);
	std::uniform_real_distribution<double> beamCrackedSectionModifierDist(m_parameters.beamParameters.minBeamCrackedSectionModifier, m_parameters.beamParameters.maxBeamCrackedSectionModifier);

	double width = beamWidthDist(m_generator);
	double equivalentDepth = equivalentBeamDepthDist(m_generator);
	double crackedMod = beamCrackedSectionModifierDist(m_generator);
	double difDepth = m_parameters.beamParameters.maxBeamDepth - m_parameters.beamParameters.minBeamDepth;

	double minLength = std::numeric_limits<double>::max();
	double maxLength = std::numeric_limits<double>::min();
	double totalLength = 0;

	for (int i = 0; i < numOfBaysX; ++i) {
		minLength = std::min(minLength, bayWidthsX[i]);
		maxLength = std::max(maxLength, bayWidthsX[i]);
		totalLength += bayWidthsX[i];
	}

	for (int i = 0; i < numOfBaysY; ++i) {
		minLength = std::min(minLength, bayWidthsY[i]);
		maxLength = std::max(maxLength, bayWidthsY[i]);
		totalLength += bayWidthsY[i];
	}
	double aveLength = totalLength / (double)(numOfBaysX + numOfBaysY);
	double thresholdVal = (aveLength - minLength) / (maxLength - minLength);

	double minDepth = m_parameters.beamParameters.minBeamDepth;
	double maxDepth = m_parameters.beamParameters.maxBeamDepth;
	if ((equivalentDepth - m_parameters.beamParameters.minBeamDepth) / thresholdVal) {
		minDepth = (equivalentDepth - thresholdVal * maxDepth) / (1 - thresholdVal);
	}
	else {
		maxDepth = (equivalentDepth - minDepth * (1 - thresholdVal)) / thresholdVal;
	}

	buildingInfo["beam"]["width"] = width;
	buildingInfo["beam"]["equivalentDepth"] = equivalentDepth;
	buildingInfo["beam"]["minDepth"] = minDepth;
	buildingInfo["beam"]["maxDepth"] = maxDepth;
	buildingInfo["beam"]["minLength"] = minLength;
	buildingInfo["beam"]["maxLength"] = maxLength;
	buildingInfo["beam"]["crackedMod"] = crackedMod;

	auto getDepth{
		[=](double length) {
			return minDepth + (maxDepth - minDepth) * (length - minLength) / (maxLength - minLength);
		}
	};

	double totalBeamIndex = 0.0;

	std::vector<int> sectionTagsX((numOfBaysX + 1) / 2, 0);
	int numOfJointsPerFloor = (numOfBaysX + 1) * (numOfBaysY + 1);
	int sectionTag = 201;
	int currentSectionTag;
	for (int i = 0; i < ns; ++i) {

		int elementTag = 20100 + 100 * i + 1;
		for (int j = 0; j <= numOfBaysY; ++j) {

			for (int k = 0; k < numOfBaysX; ++k) {

				int jointITag = i * numOfJointsPerFloor + 1 + (numOfBaysX + 1) * j + k;
				int jointJTag = i * numOfJointsPerFloor + 1 + (numOfBaysX + 1) * j + k + 1;

				double depth = getDepth(bayWidthsX[k]);
				
				if (k > (numOfBaysX - 1) / 2) {

					currentSectionTag = sectionTagsX[numOfBaysX - 1 - k];
				}
				else {

					if (sectionTagsX[k] == 0) {

						api::addElasticSection1D(sectionTag, 1, new physicalModel::Rectangle(width, depth));
						sectionTagsX[k] = sectionTag;
						currentSectionTag = sectionTag++;
					}
					else {
						currentSectionTag = sectionTagsX[k];
					}
				}

				if (1 != shearWallArrangementX[k] || 1 != shearWallArrangementY[j]) {

					api::addBeam(elementTag, { jointITag, jointJTag }, currentSectionTag, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
					api::setSectionModifiers(elementTag, 0, 1.0, crackedMod, 1.0, 1.0);

					if (m_parameters.meshInfo.meshSlabBeam) {

						int numOfSegments = bayWidthsX[k] / m_parameters.meshInfo.meshSensitivity;
						numOfSegments = numOfSegments % 2 == 0 ? numOfSegments : numOfSegments + 1;
						std::vector<double> segmentRatios(numOfSegments, bayWidthsX[k] / (double)numOfSegments);
						api::setSegmentRatios(elementTag, segmentRatios);
					}

					if (0 == i) {

						totalBeamIndex += api::getMomentOfInertiaYY(elementTag) / std::pow(bayWidthsX[k], 3.0);
					}

					++elementTag;
				}
			}
		}
	}

	buildingInfo["beam"]["totalBeamIndexX"] = totalBeamIndex;

	std::vector<int> sectionTagsY((numOfBaysY + 1) / 2, 0);
	sectionTag = 251;
	for (int i = 0; i < ns; ++i) {

		int elementTag = 25100 + 100 * i + 1;
		for (int j = 0; j < numOfBaysY; ++j) {

			double depth = getDepth(bayWidthsX[j]);

			if (j > (numOfBaysY - 1) / 2) {

				currentSectionTag = sectionTagsY[numOfBaysY - 1 - j];
			}
			else {

				if (sectionTagsY[j] == 0) {

					api::addElasticSection1D(sectionTag, 1, new physicalModel::Rectangle(width, depth));
					sectionTagsY[j] = sectionTag;
					currentSectionTag = sectionTag++;
				}
				else {
					currentSectionTag = sectionTagsY[j];
				}
			}

			for (int k = 0; k <= numOfBaysX; ++k) {

				int jointITag = i * numOfJointsPerFloor + 1 + (numOfBaysX + 1) * j + k;
				int jointJTag = i * numOfJointsPerFloor + 1 + (numOfBaysX + 1) * (j + 1) + k;

				api::addBeam(elementTag, { jointITag, jointJTag }, currentSectionTag, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
				api::setSectionModifiers(elementTag, 0, 1.0, crackedMod, 1.0, 1.0);

				if (m_parameters.meshInfo.meshSlabBeam) {

					int numOfSegments = bayWidthsY[j] / m_parameters.meshInfo.meshSensitivity;
					numOfSegments = numOfSegments % 2 == 0 ? numOfSegments : numOfSegments + 1;
					std::vector<double> segmentRatios(numOfSegments, bayWidthsY[j] / (double)numOfSegments);
					api::setSegmentRatios(elementTag, segmentRatios);
				}

				if (0 == i) {

					totalBeamIndex += api::getMomentOfInertiaYY(elementTag) / std::pow(bayWidthsY[j], 3.0);
				}

				++elementTag;
			}
		}
	}

	buildingInfo["beam"]["totalBeamIndex"] = totalBeamIndex;
}

std::vector<std::vector<int>> RegularPlanBuildingGenerator::getShearWallArrangementInLongitudinalDir(int numOfBaysLongDir, int numOfBaysPerpDir, std::vector<double> bayWidthsLongDir, std::vector<double> bayWidthsPerpDir, double thickness, double& shearWallRatio)
{
	double lengthLong = 0.0;
	for (int i = 0; i < numOfBaysLongDir; ++i) {
		lengthLong += bayWidthsLongDir[i];
	}

	double lengthPerp = 0.0;
	for (int i = 0; i < numOfBaysPerpDir; ++i) {
		lengthPerp += bayWidthsPerpDir[i];
	}

	double buildingArea = lengthLong * lengthPerp;
	std::map<int, std::vector<std::vector<int>>> arrangements;
	arrangements[0] = {};
	int numOfLocationAlongPerpDir = (numOfBaysPerpDir + 1) / 2;
	std::vector<int> locationVector;
	for (int i = 0; i < numOfLocationAlongPerpDir; ++i) {
		locationVector.push_back(i);
	}
	auto possibleLocations = findSubsetsOfVector(locationVector);

	int count = 1;
	std::vector<double> shearWallRatioVec;
	shearWallRatioVec.push_back(0);
	// Symmetric one bay shear walls
	double sectionArea = bayWidthsLongDir[numOfBaysLongDir / 2] * thickness;
	for (int i = 1; i < possibleLocations.size(); ++i) {

		auto shearWallRatio = (2 * sectionArea * (double)possibleLocations[i].size()) / buildingArea;

		if (shearWallRatio <= m_parameters.shearWallParameters.maxShearWallRatio) {
			arrangements[count].push_back({ numOfBaysLongDir / 2 });
			arrangements[count].push_back(possibleLocations[i]);
			shearWallRatioVec.push_back(shearWallRatio);
			count++;
		}
	}
	
	// Symmetric two bay shear walls
	int firstBay = numOfBaysLongDir % 2 == 0 ? numOfBaysLongDir / 2 - 2 : numOfBaysLongDir / 2 - 1;
	int secondBay = numOfBaysLongDir / 2 + 1;
	sectionArea = 2 * bayWidthsLongDir[firstBay] * thickness;
	for (int i = 1; i < possibleLocations.size(); ++i) {

		auto shearWallRatio = (2 * sectionArea * (double)possibleLocations[i].size()) / buildingArea;

		if (shearWallRatio <= m_parameters.shearWallParameters.maxShearWallRatio) {
			arrangements[count].push_back({ firstBay, secondBay });
			arrangements[count].push_back(possibleLocations[i]);
			shearWallRatioVec.push_back(shearWallRatio);
			count++;
		}
	}

	std::uniform_int_distribution<int> numberOfStoreysDist(0, count - 1);
	int chosenIndex = numberOfStoreysDist(m_generator);

	auto chosenArrangement = arrangements[chosenIndex];
	shearWallRatio = shearWallRatioVec[chosenIndex];

	std::vector<int> arrangementLong(bayWidthsLongDir.size(), -1);
	for (int i = 0; i < numOfBaysLongDir; ++i) {

		arrangementLong[i] = 0;
	}

	std::vector<int> arrangementPerp(bayWidthsPerpDir.size() + 1, -1);
	for (int i = 0; i < numOfBaysPerpDir + 1; ++i) {

		arrangementPerp[i] = 0;
	}

	if (chosenArrangement.empty()) {
		return { arrangementLong , arrangementPerp };
	}

	for (int i = 0; i < chosenArrangement[0].size(); ++i) {

		arrangementLong[chosenArrangement[0][i]] = 1;
	}


	for (int i = 0; i < chosenArrangement[1].size(); ++i) {

		arrangementPerp[chosenArrangement[1][i]] = 1;
		arrangementPerp[numOfBaysPerpDir - chosenArrangement[1][i]] = 1;
	}

	return { arrangementLong , arrangementPerp };
}

std::vector<std::vector<int>> RegularPlanBuildingGenerator::findSubsetsOfVector(const std::vector<int>& vec) {
	std::vector<std::vector<int>> subsets;
	int n = vec.size();

	for (int i = 0; i < (1 << n); i++) {
		std::vector<int> subset;
		for (int j = 0; j < n; j++) {
			if (i & (1 << j)) {
				subset.push_back(vec[j]);
			}
		}
		subsets.push_back(subset);
	}

	return subsets;
}