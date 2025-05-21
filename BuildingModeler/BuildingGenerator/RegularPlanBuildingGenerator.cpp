#include "RegularPlanBuildingGenerator.h"
#include "../BuildingModelerAPI.h"
#include "../LoadingGenerator/GravityLoadingGenerator.h"
#include "../LoadingGenerator/ELFLoadingGenerator.h"
#include "../LoadingGenerator/ModalAnalysisGenerator.h"

#include <cstdlib> 
#include <chrono>
#include <cmath>
#include <map>
#include <fstream>

using namespace buildingGenerator;

long long RegularPlanBuildingGenerator::counter = 0;

RegularPlanBuildingGenerator::RegularPlanBuildingGenerator(const Parameters& parameters, std::unique_ptr<loadingGenerator::ILoadingGenerator> loading) :
	m_parameters(parameters), m_loading(std::move(loading))
{
	void validateInput();
}

json RegularPlanBuildingGenerator::generateAndAnalyze(std::optional<long long> seed, int processID)
{
	if (seed == std::nullopt) {
		m_seed = std::chrono::system_clock::now().time_since_epoch().count() + counter++;
		m_generator.seed(m_seed);
	}
	else {
		m_seed = std::chrono::system_clock::now().time_since_epoch().count() + seed.value() + counter++;
		m_generator.seed(m_seed);
	}

	api::clear();

	api::setProcessID(processID);

	json buildingInfo;

	// Generate building plan
	generateBuildingPlan(buildingInfo);

	// Generate floors
	std::uniform_int_distribution<int> numberOfStoreysDist(m_parameters.geometricParameters.minNumberOfStoreys, m_parameters.geometricParameters.maxNumberOfStoreys);
	std::uniform_real_distribution<double> storeyHeightDist(m_parameters.geometricParameters.minStoreyHeight, m_parameters.geometricParameters.maxStoreyHeight);
	int ns = numberOfStoreysDist(m_generator);
	double H2 = storeyHeightDist(m_generator);
	std::uniform_real_distribution<double> firstStoreyHeightDist(H2, m_parameters.geometricParameters.maxFirstStoreyHeight);
	double H1 = firstStoreyHeightDist(m_generator);
	buildingInfo["numberOfStoreys"] = ns;
	buildingInfo["firstStoreyHeight"] = H1;
	buildingInfo["storeyHeight"] = H2;
	generateFloors(ns, H1, H2);

	// Retrieve geometric properties
	int numOfBaysX = buildingInfo["numberOfBaysX"];
	int numOfBaysY = buildingInfo["numberOfBaysY"];
	std::vector<double> bayWidthsX = buildingInfo["bayWidthsX"];
	std::vector<double> bayWidthsY = buildingInfo["bayWidthsY"];
	double planArea = buildingInfo["planArea"];

	// Generate materials
	std::uniform_real_distribution<double> youngsModulusDist(m_parameters.minConcreteYoungsModulus, m_parameters.maxConcreteYoungsModulus);
	std::uniform_real_distribution<double> shearWallCrackedSectionModifierDist(m_parameters.shearWallParameters.minShearWallCrackedSectionModifier, m_parameters.shearWallParameters.maxShearWallCrackedSectionModifier);
	double E = youngsModulusDist(m_generator);
	double crackedModShearWall = shearWallCrackedSectionModifierDist(m_generator);
	buildingInfo["youngsModulus"] = E;
	buildingInfo["shearWall"]["crackedSectionModifier"] = crackedModShearWall;
	generateMaterials(E, crackedModShearWall);

	//Create shear walls
	std::unordered_map<int, std::pair<int, int>> shearWallJointMap;
	std::uniform_real_distribution<double> shearWallThicknessDist(m_parameters.shearWallParameters.minShearWallThickness, m_parameters.shearWallParameters.maxShearWallThickness);
	double tShearWall = shearWallThicknessDist(m_generator);
	buildingInfo["shearWall"]["thickness"] = tShearWall;
	double shearWallRatioX = 0.0;
	double shearWallRatioY = 0.0;
	std::vector<std::vector<std::vector<double>>> shearWallArrangement;
	shearWallArrangement = getShearWallArrangement(numOfBaysX, numOfBaysY, bayWidthsX, bayWidthsY, tShearWall, shearWallRatioX, shearWallRatioY);

	buildingInfo["shearWall"]["shearWallXDir"]["ratio"] = shearWallRatioX;
	buildingInfo["shearWall"]["shearWallXDir"]["area"] = shearWallRatioX * planArea;
	for (int i = 0; i <= m_parameters.geometricParameters.maxNumberOfBays; ++i) {
		buildingInfo["shearWall"]["shearWallXDir"][std::to_string(i)] = shearWallArrangement[0][i];
	}
	buildingInfo["shearWall"]["shearWallYDir"]["ratio"] = shearWallRatioY;
	buildingInfo["shearWall"]["shearWallYDir"]["area"] = shearWallRatioY * planArea;
	for (int i = 0; i <= m_parameters.geometricParameters.maxNumberOfBays; ++i) {
		buildingInfo["shearWall"]["shearWallYDir"][std::to_string(i)] = shearWallArrangement[1][i];
	}

	// Generate joints
	generateJoints(buildingInfo, shearWallArrangement, shearWallJointMap);

	api::giveJointHint(shearWallJointMap);

	// Generate shear walls
	generateShearWalls(shearWallArrangement, shearWallJointMap, tShearWall, buildingInfo);
	
	// Generate columns
	std::vector<std::vector<int>> modifiedShearWallArrangement(m_parameters.geometricParameters.maxNumberOfBays + 1, std::vector<int>(m_parameters.geometricParameters.maxNumberOfBays + 1, -1));
	for (int i = 0; i <= numOfBaysY; ++i) {

		for (int j = 0; j <= numOfBaysX; ++j) {

			modifiedShearWallArrangement[i][j] = 0;
		}
	}
	for (int i = 0; i <= numOfBaysX; ++i) {

		for (int j = 0; j <= numOfBaysY; ++j) {

			if (shearWallArrangement[0][j][i] > 0.001) {

				modifiedShearWallArrangement[j][i]++;
			}
		}
	}
	for (int i = 0; i <= numOfBaysY; ++i) {

		for (int j = 0; j <= numOfBaysX; ++j) {

			if (shearWallArrangement[1][j][i] > 0.001) {

				modifiedShearWallArrangement[i][j]++;
			}
		}
	}
	for (int i = 0; i <= m_parameters.geometricParameters.maxNumberOfBays; ++i) {
		buildingInfo["shearWall"]["modifiedArrangement"][std::to_string(i)] = modifiedShearWallArrangement[i];
		buildingInfo["column"]["columnArrangement"][std::to_string(i)] = std::vector<int>(m_parameters.geometricParameters.maxNumberOfBays + 1, -1);
	}
	std::uniform_real_distribution<double> maxAspectRatioDist(1.0, m_parameters.columnParameters.maxAspectRatioForColumns);
	std::uniform_real_distribution<double> maxAreaRatioInnerToOuterDist(1.0, m_parameters.columnParameters.maxAreaRatioInnerToOuterColumns);
	std::uniform_real_distribution<double> equivalentSquareColumnWidthDist(m_parameters.columnParameters.minEquivalentSquareColumnWidth, m_parameters.columnParameters.maxEquivalentSquareColumnWidth);
	std::uniform_real_distribution<double> columnCrackedSectionModifierDist(m_parameters.columnParameters.minColumnCrackedSectionModifier, m_parameters.columnParameters.maxColumnCrackedSectionModifier);
	double aspectRatio = maxAspectRatioDist(m_generator);
	double areaRatioInnerToOuter = maxAreaRatioInnerToOuterDist(m_generator);
	double squareWidth = equivalentSquareColumnWidthDist(m_generator);
	double crackedModColumn = columnCrackedSectionModifierDist(m_generator);
	double widthOuterS = squareWidth / std::sqrt(aspectRatio);
	double widthOuterL = squareWidth * std::sqrt(aspectRatio);
	double widthInnerS = widthOuterS * std::sqrt(areaRatioInnerToOuter);
	double widthInnerL = widthOuterL * std::sqrt(areaRatioInnerToOuter);
	buildingInfo["column"]["outerColumns"]["width"] = widthOuterS;
	buildingInfo["column"]["outerColumns"]["depth"] = widthOuterL;
	buildingInfo["column"]["innerColumns"]["width"] = widthInnerS;
	buildingInfo["column"]["innerColumns"]["depth"] = widthInnerL;
	buildingInfo["column"]["crackedMod"] = crackedModColumn;
	generateColumns(modifiedShearWallArrangement, widthOuterS, widthOuterL, widthInnerS, widthInnerL, crackedModColumn, buildingInfo);
	
	// Generate beams
	std::uniform_real_distribution<double> beamWidthDist(m_parameters.beamParameters.minBeamWidth, m_parameters.beamParameters.maxBeamWidth);
	std::uniform_real_distribution<double> equivalentBeamDepthDist(std::max(m_parameters.beamParameters.minBeamDepth, squareWidth), m_parameters.beamParameters.maxBeamDepth);
	std::uniform_real_distribution<double> beamCrackedSectionModifierDist(m_parameters.beamParameters.minBeamCrackedSectionModifier, m_parameters.beamParameters.maxBeamCrackedSectionModifier);
	double width = beamWidthDist(m_generator);
	double equivalentDepth = equivalentBeamDepthDist(m_generator);
	double crackedModBeam = beamCrackedSectionModifierDist(m_generator);
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
	if (maxLength >= minLength - 1e-10 && maxLength <= minLength + 1e-10) {
		minDepth = equivalentDepth;
		maxDepth = equivalentDepth;
	}
	else if ((equivalentDepth - m_parameters.beamParameters.minBeamDepth) / difDepth > thresholdVal) {
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
	buildingInfo["beam"]["crackedMod"] = crackedModBeam;
	generateBeams(width, minDepth, maxDepth, minLength, maxLength, crackedModBeam, shearWallJointMap, buildingInfo);
	
	// Mesh area elements
	meshAreaElements();

	// Apply model preferences and generate slabv masses
	std::uniform_real_distribution<double> liveLoadDist(m_parameters.modelingPreferences.minLiveLoadPerArea, m_parameters.modelingPreferences.maxLiveLoadPerArea);
	std::uniform_real_distribution<double> liveLoadMassContributionDist(m_parameters.modelingPreferences.minLiveLoadMassContribution, m_parameters.modelingPreferences.maxLiveLoadMassContribution);
	double liveLoad = liveLoadDist(m_generator);
	double liveLoadMassContributionFactor = liveLoadMassContributionDist(m_generator);
	buildingInfo["loading"]["liveLoad"]["liveLoadPerArea"] = liveLoad;
	buildingInfo["loading"]["liveLoad"]["liveLoadMassParticipationFactor"] = liveLoadMassContributionFactor;
	std::uniform_real_distribution<double> slabThicknessDist(m_parameters.slabParameters.minSlabThickness, m_parameters.slabParameters.maxSlabThickness);
	double tSlab = slabThicknessDist(m_generator);
	buildingInfo["slab"]["thickness"] = tSlab;
	generateSlabMasses(tSlab, shearWallJointMap, buildingInfo);
	applyModelingPreferences(buildingInfo);

	// Load the building
	if (dynamic_cast<loadingGenerator::GravityLoadingGenerator*>(m_loading.get())) {
		int coreX = buildingInfo["shearWall"]["coreLocationX"];
		int coreY = buildingInfo["shearWall"]["coreLocationY"];
		auto totalLiveLoad = (double)ns * liveLoad * (planArea - bayWidthsX[coreX] * bayWidthsY[coreY]);
		buildingInfo["loading"]["totalLiveLoad"] = totalLiveLoad;
		buildingInfo["loading"]["totalDeadLoad"] = api::getBuildingWeight();
		m_loading->load(buildingInfo);

		// Analyze the building
		auto analysisSuccess = analyze();
	
		// Fetch the results
		if (!analysisSuccess["gravity"]) {
			return json{};
		}
		fetchResultsForGravityAnalysis(buildingInfo);
	}
	else if (dynamic_cast<loadingGenerator::ELFLoadingGenerator*>(m_loading.get())) {
		m_loading->load(buildingInfo);
		
		// Analyze the building
		auto analysisSuccess = analyze();
	
		// Fetch the results
		if (!analysisSuccess["earthquake"]) {
			return json{};
		}
		fetchResultsForEarthquakeAnalysis(buildingInfo);
	}
	else if (dynamic_cast<loadingGenerator::ModalAnalysisGenerator*>(m_loading.get())) {
		m_loading->load(buildingInfo);
	
		// Analyze the building
		auto analysisSuccess = analyze();
	
		// Fetch the results
		if (!analysisSuccess["modal"]) {
			return json{};
		}
		fetchResultsForModalAnalysis(buildingInfo);
	}
	
	return buildingInfo;
}

bool RegularPlanBuildingGenerator::createModelFromJsonAndAnalyze(json& buildingInfo)
{
	m_seed = std::chrono::system_clock::now().time_since_epoch().count() + counter++;
	m_generator.seed(m_seed);

	for (int i = 0; i <= m_parameters.geometricParameters.maxNumberOfBays; ++i) {
		buildingInfo["verticalMemberPlan"][std::to_string(i)] = std::vector<int>(m_parameters.geometricParameters.maxNumberOfBays + 1, 0);
	}
	for (int i = 0; i <= m_parameters.geometricParameters.maxNumberOfBays; ++i) {
		buildingInfo["verticalMemberPlanSecondary"][std::to_string(i)] = std::vector<int>(m_parameters.geometricParameters.maxNumberOfBays + 1, 0);
	}

	// Generate floors
	int ns = buildingInfo["numberOfStoreys"];
	double H1 = buildingInfo["firstStoreyHeight"];
	double H2 = buildingInfo["storeyHeight"];
	generateFloors(ns, H1, H2);

	// Retrieve geometric properties
	int numOfBaysX = buildingInfo["numberOfBaysX"];
	int numOfBaysY = buildingInfo["numberOfBaysY"];
	std::vector<double> bayWidthsX = buildingInfo["bayWidthsX"];
	std::vector<double> bayWidthsY = buildingInfo["bayWidthsY"];
	double planArea = buildingInfo["planArea"];

	// Generate joints
	//generateJoints(buildingInfo);

	// Generate materials
	double E = buildingInfo["youngsModulus"];
	double crackedModShearWall = buildingInfo["shearWall"]["crackedSectionModifier"];
	generateMaterials(E, crackedModShearWall);

	//Generate shear walls
	double tShearWall = buildingInfo["shearWall"]["thickness"];
	std::vector<std::vector<std::vector<double>>> shearWallArrangement(2, std::vector<std::vector<double>>(m_parameters.geometricParameters.maxNumberOfBays + 1));
	for (int i = 0; i <= m_parameters.geometricParameters.maxNumberOfBays; ++i) {
		buildingInfo["shearWall"]["shearWallXDir"][std::to_string(i)].get_to(shearWallArrangement[0][i]);
	}
	for (int i = 0; i <= m_parameters.geometricParameters.maxNumberOfBays; ++i) {
		buildingInfo["shearWall"]["shearWallYDir"][std::to_string(i)].get_to(shearWallArrangement[1][i]);
	}
	//generateShearWalls(shearWallArrangement, tShearWall, buildingInfo);

	// Generate slabs
	double tSlab = buildingInfo["slab"]["thickness"];
	//generateSlabMasses(tSlab, buildingInfo);

	// Generate columns
	std::vector<std::vector<int>> modifiedShearWallArrangement(m_parameters.geometricParameters.maxNumberOfBays + 1);
	for (int i = 0; i <= m_parameters.geometricParameters.maxNumberOfBays; ++i) {
		buildingInfo["shearWall"]["modifiedArrangement"][std::to_string(i)].get_to(modifiedShearWallArrangement[i]);
	}
	double widthOuterS = buildingInfo["column"]["outerColumns"]["width"];
	double widthOuterL = buildingInfo["column"]["outerColumns"]["depth"];
	double widthInnerS = buildingInfo["column"]["innerColumns"]["width"];
	double widthInnerL = buildingInfo["column"]["innerColumns"]["depth"];
	double crackedModColumn = buildingInfo["column"]["crackedMod"];
	generateColumns(modifiedShearWallArrangement, widthOuterS, widthOuterL, widthInnerS, widthInnerL, crackedModColumn, buildingInfo);

	// Generate beams
	double width = buildingInfo["beam"]["width"];
	double equivalentDepth = buildingInfo["beam"]["equivalentDepth"];
	double minDepth = buildingInfo["beam"]["minDepth"];
	double maxDepth = buildingInfo["beam"]["maxDepth"];
	double minLength = buildingInfo["beam"]["minLength"];
	double maxLength = buildingInfo["beam"]["maxLength"];
	double crackedModBeam = buildingInfo["beam"]["crackedMod"];
	//generateBeams(width, minDepth, maxDepth, minLength, maxLength, crackedModBeam, buildingInfo);

	// Mesh area elements
	meshAreaElements();

	// Apply model preferences
	double liveLoad = buildingInfo["loading"]["liveLoad"]["liveLoadPerArea"];
	double liveLoadMassContributionFactor = buildingInfo["loading"]["liveLoad"]["liveLoadMassParticipationFactor"];
	applyModelingPreferences(buildingInfo);

	// Load the building
	if (dynamic_cast<loadingGenerator::GravityLoadingGenerator*>(m_loading.get())) {
		auto totalLiveLoad = (double)ns * liveLoad * planArea;
		buildingInfo["loading"]["totalLiveLoad"] = totalLiveLoad;
		buildingInfo["loading"]["totalDeadLoad"] = api::getBuildingWeight();
		double deadLoadFactor = 1.0; //buildingInfo["loading"]["deadLoad"]["deadLoadFactor"];
		double liveLoadFactor = 1.0; //buildingInfo["loading"]["liveLoad"]["liveLoadFactor"];
		api::updateDeadAndLiveLoads();
		api::addStaticLoadCombination("gravity");
		api::addLoadCaseToStaticLoadCombination("gravity", "dead", deadLoadFactor);
		api::addLoadCaseToStaticLoadCombination("gravity", "live", liveLoadFactor);
		api::setStaticLoadCombinationActive("gravity", true);

		// Analyze the building
		auto analysisSuccess = analyze();

		// Fetch the results
		if (!analysisSuccess["gravity"]) {
			return false;
		}

		fetchResultsForGravityAnalysis(buildingInfo);
	}
	else if (dynamic_cast<loadingGenerator::ELFLoadingGenerator*>(m_loading.get())) {
		int ns = buildingInfo["numberOfStoreys"];
		double buildingWeight = 0;
		for (int i = 1; i <= ns; ++i) {
			buildingWeight += api::getDiaphragmMass(i).value().x * 9.81;
		}

		double sA = buildingInfo["loading"]["earthquakeLoad"]["spectralAcceleration"];
		auto baseShear = sA * buildingWeight;
		buildingInfo["loading"]["earthquakeLoad"]["baseShear"] = baseShear;
		buildingInfo["loading"]["earthquakeLoad"]["totalMass"] = buildingWeight / 9.81;

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

	// Analyze the building
	auto analysisSuccess = analyze();

	// Fetch the results
		if (!analysisSuccess["earthquake"]) {
		return false;
	}
		fetchResultsForEarthquakeAnalysis(buildingInfo);
	}

	// Write model to json file
	std::ofstream file("data/building_info6R.json");
	file << buildingInfo.dump(4);  // The argument 4 specifies indentation for pretty-printing
	file.close();

	return true;
}

void RegularPlanBuildingGenerator::validateInput()
{
	// To do: validate parameters input here
}

void RegularPlanBuildingGenerator::generateBuildingPlan(json& buildingInfo)
{
	std::uniform_int_distribution<int> numberOfBayDist(m_parameters.geometricParameters.minNumberOfBays, m_parameters.geometricParameters.maxNumberOfBays);

	int minAddition = 0;
	int maxAddition = int((m_parameters.geometricParameters.maxBayWidth - m_parameters.geometricParameters.minBayWidth + 0.001) / m_parameters.geometricParameters.planSensitivity);
	std::uniform_int_distribution<int> bayWidthDist(minAddition, maxAddition);

	std::vector<double> bayWidthsX(m_parameters.geometricParameters.maxNumberOfBays);
	std::vector<double> bayWidthsY(m_parameters.geometricParameters.maxNumberOfBays);

	int numOfBaysX = numberOfBayDist(m_generator);
	int numOfBaysY = numberOfBayDist(m_generator);

	int diff = (numOfBaysX % 2 == 0 ? 1 : 2);
	for (int i = 0; i < numOfBaysX; ++i) {
		auto bayWidth = m_parameters.geometricParameters.minBayWidth + (double)bayWidthDist(m_generator) * m_parameters.geometricParameters.planSensitivity;

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
		auto bayWidth = m_parameters.geometricParameters.minBayWidth + (double)bayWidthDist(m_generator) * m_parameters.geometricParameters.planSensitivity;

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

void RegularPlanBuildingGenerator::generateFloors(int numberOfStories, double firstStoreyHeight, double storeyHeight)
{
	api::addFloor(0, 0.0);
	auto currentHeight = firstStoreyHeight;

	for (int i = 1; i <= numberOfStories; ++i) {
		api::addFloor(i, currentHeight);
		currentHeight += storeyHeight;
	}
}

void RegularPlanBuildingGenerator::generateJoints(json& buildingInfo, const std::vector<std::vector<std::vector<double>>>& shearWallArrangement, std::unordered_map<int, std::pair<int, int>>& shearWallJointMap)
{
	int ns = buildingInfo["numberOfStoreys"];
	double H1 = buildingInfo["firstStoreyHeight"];
	double H2 = buildingInfo["storeyHeight"];
	int numOfBaysX = buildingInfo["numberOfBaysX"];
	int numOfBaysY = buildingInfo["numberOfBaysY"];
	std::vector<double> bayWidthsX = buildingInfo["bayWidthsX"];
	std::vector<double> bayWidthsY = buildingInfo["bayWidthsY"];

	auto shearWallLength {
		[&] (int i, int j, bool isXDir) -> std::optional<double> {
			std::optional<double> ret;

			if (isXDir) {

				if (shearWallArrangement[0][i][j] > 0.001) {
					ret = shearWallArrangement[0][i][j];
				}
			}
			else {

				if (shearWallArrangement[1][i][j] > 0.001) {
					ret = shearWallArrangement[1][i][j];
				}
			}

			return ret;
		}
	};

	int jointTag = 1;
	double coordZ = 0.0;
	for (int i = 0; i <= ns; ++i) {

		double coordY = 0.0;
		for (int j = 0; j <= numOfBaysY; ++j) {

			double coordX = 0.0;
			for (int k = 0; k <= numOfBaysX; ++k) {

				bool isJointRegular = true;

				if (j == 0 || j == numOfBaysY) {

					auto swLength = shearWallLength(j, k, true);

					if (swLength != std::nullopt) {
						api::addJoint(20000 + jointTag, { coordX - swLength.value() / 2.0, coordY, coordZ });
						api::addJoint(30000 + jointTag, { coordX + swLength.value() / 2.0, coordY, coordZ });

						shearWallJointMap[jointTag] = std::make_pair(20000 + jointTag, 30000 + jointTag);

						isJointRegular = false;
					}
				}

				if (k == 0 || k == numOfBaysX) {

					auto swLength = shearWallLength(k, j, false);

					if (swLength != std::nullopt) {
						api::addJoint(20000 + jointTag, { coordX, coordY - swLength.value() / 2.0, coordZ });
						api::addJoint(30000 + jointTag, { coordX, coordY + swLength.value() / 2.0, coordZ });

						shearWallJointMap[jointTag] = std::make_pair(20000 + jointTag, 30000 + jointTag);

						isJointRegular = false;
					}
				}

				if (isJointRegular) {
					api::addJoint(jointTag, { coordX, coordY, coordZ });
					api::setFloorNo(jointTag, i);

					if (i == 0) {
						api::setConstraintVector(jointTag, { 1, 1, 1, 1, 1, 1 });
					}
				}
				else {
					api::setFloorNo(20000 + jointTag, i);
					api::setFloorNo(30000 + jointTag, i);

					if (i == 0) {
						api::setConstraintVector(20000 + jointTag, { 1, 1, 1, 1, 1, 1 });
						api::setConstraintVector(30000 + jointTag, { 1, 1, 1, 1, 1, 1 });
					}
					
					api::addJoint(jointTag, { coordX, coordY, coordZ });
					api::setFloorNo(jointTag, i);
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

void RegularPlanBuildingGenerator::generateMaterials(double E, double shearWallCrackedSectionModifier)
{
	double Ecracked = shearWallCrackedSectionModifier * E;

	double G = E / (1.0 + 0.2) / 2.0;
	double Gcracked = Ecracked / (1.0 + 0.2) / 2.0;

	api::addElasticMaterial(1, E, G, 2.4);
	api::addElasticMaterial(2, Ecracked, Gcracked, 2.4); // for shear walls only
}

void RegularPlanBuildingGenerator::generateShearWalls(const std::vector<std::vector<std::vector<double>>>& shearWallArrangement,std::unordered_map<int, std::pair<int, int>>& shearWallJointMap, double thickness, json& buildingInfo)
{
	int ns = buildingInfo["numberOfStoreys"];
	int numOfBaysX = buildingInfo["numberOfBaysX"];
	int numOfBaysY = buildingInfo["numberOfBaysY"];

	api::addElasticSection2D(301, 2, thickness);

	double primaryIx = 0.0;
	double secondaryIx = 0.0;
	int numOfJointsPerFloor = (numOfBaysX + 1) * (numOfBaysY + 1);
	for (int i = 0; i < ns; ++i) {

		int elementTag = 30100 + 100 * i + 1;
		for (int j = 0; j < shearWallArrangement[0].size(); ++j) {

			for (int k = 0; k < shearWallArrangement[0][j].size(); ++k) {

				if (shearWallArrangement[0][j][k] > 0.001) {

					int bottomJointTag = i * numOfJointsPerFloor + 1 + (numOfBaysX + 1) * j + k;
					int topJointTag = i * numOfJointsPerFloor + 1 + (numOfBaysX + 1) * j + k + numOfJointsPerFloor;
					int jointITag = shearWallJointMap[bottomJointTag].first;
					int jointJTag = shearWallJointMap[bottomJointTag].second;
					int jointKTag = shearWallJointMap[topJointTag].second;
					int jointLTag = shearWallJointMap[topJointTag].first;

					api::addShearWall(elementTag, { jointITag, jointJTag, jointKTag, jointLTag }, 301, physicalModel::AreaElementFormulation::LINEAR);

					if (0 == i) {
						buildingInfo["verticalMemberPlan"][std::to_string(j)][k] = elementTag;

						auto coordI = api::getCoordinates(jointITag);
						auto coordJ = api::getCoordinates(jointJTag);
						auto length = (coordJ - coordI).norm2();
						auto I = (1.0 / 12.0) * thickness * std::pow(length, 3);
						primaryIx += I;
					}

					++elementTag;
				}
			}
		}

		for (int j = 0; j < shearWallArrangement[1].size(); ++j) {

			for (int k = 0; k < shearWallArrangement[1][j].size(); ++k) {

				if (shearWallArrangement[1][j][k] > 0.001) {

					int bottomJointTag = i * numOfJointsPerFloor + 1 + (numOfBaysX + 1) * k + j;
					int topJointTag = i * numOfJointsPerFloor + 1 + (numOfBaysX + 1) * k + j + numOfJointsPerFloor;
					int jointITag = shearWallJointMap[bottomJointTag].second;
					int jointJTag = shearWallJointMap[bottomJointTag].first;
					int jointKTag = shearWallJointMap[topJointTag].first;
					int jointLTag = shearWallJointMap[topJointTag].second;

					api::addShearWall(elementTag, { jointITag, jointJTag, jointKTag, jointLTag }, 301, physicalModel::AreaElementFormulation::LINEAR);

					if (0 == i) {
						buildingInfo["verticalMemberPlan"][std::to_string(k)][j] = elementTag;

						auto coordI = api::getCoordinates(jointITag);
						auto coordJ = api::getCoordinates(jointJTag);
						auto length = (coordJ - coordI).norm2();
						auto I = (1.0 / 12.0) * length * std::pow(thickness, 3);
						secondaryIx += I;
					}

					++elementTag;
				}
			}
		}
	}

	buildingInfo["shearWall"]["shearWallXDir"]["momentOfInertia"] = primaryIx;
	buildingInfo["shearWall"]["shearWallYDir"]["momentOfInertia"] = secondaryIx;
}

void RegularPlanBuildingGenerator::generateSlabMasses(double thickness, std::unordered_map<int, std::pair<int, int>>& shearWallJointMap, json& buildingInfo)
{
	int ns = buildingInfo["numberOfStoreys"];
	int numOfBaysX = buildingInfo["numberOfBaysX"];
	int numOfBaysY = buildingInfo["numberOfBaysY"];
	std::vector<double> bayWidthsX = buildingInfo["bayWidthsX"];
	std::vector<double> bayWidthsY = buildingInfo["bayWidthsY"];
	double liveLoad = buildingInfo["loading"]["liveLoad"]["liveLoadPerArea"];
	double liveLoadMassContributionFactor = buildingInfo["loading"]["liveLoad"]["liveLoadMassParticipationFactor"];

	auto assignMass {
		[&] (int jointTag, double mass) -> void {
			
			utility::Vector3 transMass(mass, mass, 0.0);
			api::addTranslationalMass(jointTag, transMass);
		}
	};

	int numOfJointsPerFloor = (numOfBaysX + 1) * (numOfBaysY + 1);
	for (int i = 0; i < ns; ++i) {

		for (int j = 0; j < numOfBaysY; ++j) {

			for (int k = 0; k < numOfBaysX; ++k) {

				int initJointTag = (i + 1) * numOfJointsPerFloor + 1;
				int jointITag = initJointTag + (numOfBaysX + 1) * j + k;
				int jointJTag = initJointTag + (numOfBaysX + 1) * j + k + 1;
				int jointKTag = initJointTag + (numOfBaysX + 1) * (j + 1) + k + 1;
				int jointLTag = initJointTag + (numOfBaysX + 1) * (j + 1) + k;

				auto slabArea = bayWidthsX[k] * bayWidthsY[j];
				auto slabMass = (2.4 * slabArea * thickness + liveLoadMassContributionFactor * liveLoad * slabArea / 9.81) / 4.0;

				assignMass(jointITag, slabMass);
				assignMass(jointJTag, slabMass);
				assignMass(jointKTag, slabMass);
				assignMass(jointLTag, slabMass);
			}
		}
	}
}

void RegularPlanBuildingGenerator::generateColumns(const std::vector<std::vector<int>>& modifiedShearWallArrangement, double outerColumnWidth, double outerColumnDepth, double innerColumnWidth, double innerColumnDepth, double columnCrackedSectionModifier, json& buildingInfo)
{
	int ns = buildingInfo["numberOfStoreys"];
	int numOfBaysX = buildingInfo["numberOfBaysX"];
	int numOfBaysY = buildingInfo["numberOfBaysY"];

	std::vector<std::vector<int>> columnArrangement(m_parameters.geometricParameters.maxNumberOfBays + 1);
	for (int i = 0; i <= m_parameters.geometricParameters.maxNumberOfBays; ++i) {
		buildingInfo["column"]["columnArrangement"][std::to_string(i)].get_to(columnArrangement[i]);
	}

	std::uniform_int_distribution<int> columnOrientationDist(0, 1);
	
	// To do: array yap bunlari
	api::addElasticSection1D(101, 1, new physicalModel::Rectangle(outerColumnDepth, outerColumnWidth)); // outer column strong in x direction
	api::addElasticSection1D(102, 1, new physicalModel::Rectangle(outerColumnWidth, outerColumnDepth)); // outer column weak in x direction
	api::addElasticSection1D(111, 1, new physicalModel::Rectangle(innerColumnDepth, innerColumnWidth)); // inner column strong in x direction
	api::addElasticSection1D(112, 1, new physicalModel::Rectangle(innerColumnWidth, innerColumnDepth)); // inner column weak in x direction

	double totalIx = 0.0;
	double totalA = 0.0;

	int numOfJointsPerFloor = (numOfBaysX + 1) * (numOfBaysY + 1);
	for (int i = 0; i < ns; ++i) {

		int elementTag = 10100 + 100 * i + 1;
		for (int j = 0; j <= numOfBaysY; ++j) {

			for (int k = 0; k <= numOfBaysX; ++k) {

				if (0 == modifiedShearWallArrangement[j][k]) {

					int sectionTag;

					if (columnArrangement[j][k] == -1) {

						if (j > numOfBaysY / 2) {
							columnArrangement[j][k] = columnArrangement[numOfBaysY - j][k];
						}
						else if (k > numOfBaysX / 2) {
							columnArrangement[j][k] = columnArrangement[j][numOfBaysX - k];
						}
						else {
							columnArrangement[j][k] = columnOrientationDist(m_generator);
						}
						buildingInfo["column"]["columnArrangement"][std::to_string(j)][k] = columnArrangement[j][k];
					}

					if (j == 0 || k == 0 || j == numOfBaysY || k == numOfBaysX) {

						sectionTag = columnArrangement[j][k] ? 101 : 102;
					}
					else {

						sectionTag = columnArrangement[j][k] ? 111 : 112;
					}

					int jointITag = i * numOfJointsPerFloor + 1 + (numOfBaysX + 1) * j + k;
					int jointJTag = i * numOfJointsPerFloor + 1 + (numOfBaysX + 1) * j + k + numOfJointsPerFloor;
					api::addColumn(elementTag, { jointITag, jointJTag }, sectionTag, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
					api::setSectionModifiers(elementTag, 0, 1.0, 1.0, columnCrackedSectionModifier, 1.0);

					if (m_parameters.meshInfo.meshColumn) {

						int numOfSegments = api::getLength(elementTag) / m_parameters.meshInfo.meshSensitivity + 1;
						std::vector<double> segmentRatios(numOfSegments, 1.0 / (double)numOfSegments);
						api::setSegmentRatios(elementTag, segmentRatios);
					}

					if (0 == i) {

						buildingInfo["verticalMemberPlan"][std::to_string(j)][k] = elementTag;

						totalA += api::getArea(elementTag);
						totalIx += api::getMomentOfInertiaZZ(elementTag);
					}

					++elementTag;
				}
			}
		}
	}

	buildingInfo["column"]["area"] = totalA;
	buildingInfo["column"]["momentOfInertia"] = totalIx;
}

void RegularPlanBuildingGenerator::generateBeams(double width, double minDepth, double maxDepth, double minLength, double maxLength, double beamCrackedSectionModifier, std::unordered_map<int, std::pair<int, int>>& shearWallJointMap, json& buildingInfo)
{
	int ns = buildingInfo["numberOfStoreys"];
	int numOfBaysX = buildingInfo["numberOfBaysX"];
	int numOfBaysY = buildingInfo["numberOfBaysY"];
	std::vector<double> bayWidthsX = buildingInfo["bayWidthsX"];
	std::vector<double> bayWidthsY = buildingInfo["bayWidthsY"];

	auto getDepth{
		[=](double length) {

			if (maxLength >= minLength - 1e-10 && maxLength <= minLength + 1e-10) {
				return minDepth;
			}
			else if (length < minLength) {
				return minDepth;
			}
			else if (length > maxLength) {
				return maxDepth;
			}
			return minDepth + (maxDepth - minDepth) * (length - minLength) / (maxLength - minLength);
		}
	};

	double totalBeamIndex = 0.0;

	std::vector<std::vector<int>> sectionTagsX(numOfBaysY + 1, std::vector<int>(numOfBaysX, 0.0));
	int numOfJointsPerFloor = (numOfBaysX + 1) * (numOfBaysY + 1);
	int sectionTag = 201;
	int currentSectionTag;
	for (int i = 0; i < ns; ++i) {

		int elementTag = 20100 + 100 * i + 1;
		for (int j = 0; j <= numOfBaysY; ++j) {

			for (int k = 0; k < numOfBaysX; ++k) {

				int jointITag = (i + 1) * numOfJointsPerFloor + 1 + (numOfBaysX + 1) * j + k;
				int jointJTag = (i + 1) * numOfJointsPerFloor + 1 + (numOfBaysX + 1) * j + k + 1;

				if (shearWallJointMap.count(jointITag) && k != 0) {
					jointITag = shearWallJointMap[jointITag].second;
				}

				if (shearWallJointMap.count(jointJTag) && k != numOfBaysX - 1) {
					jointJTag = shearWallJointMap[jointJTag].first;
				}

				auto coordI = api::getCoordinates(jointITag);
				auto coordJ = api::getCoordinates(jointJTag);
				double length = (coordJ - coordI).norm2();
				double depth = getDepth(length);
				
				if (sectionTagsX[j][k] == 0) {

					api::addElasticSection1D(sectionTag, 1, new physicalModel::Rectangle(width, depth));
					sectionTagsX[j][k] = sectionTag;
					currentSectionTag = sectionTag++;
				}
				else {
					currentSectionTag = sectionTagsX[j][k];
				}

				api::addBeam(elementTag, { jointITag, jointJTag }, currentSectionTag, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
				api::setSectionModifiers(elementTag, 0, 1.0, beamCrackedSectionModifier, 1.0, 1.0);

				if (0 == i) {

					totalBeamIndex += api::getMomentOfInertiaYY(elementTag) / std::pow(length, 3.0);
				}

				++elementTag;
			}
		}
	}

	buildingInfo["beam"]["totalBeamIndexX"] = totalBeamIndex;

	std::vector<std::vector<int>> sectionTagsY(numOfBaysY, std::vector<int>(numOfBaysX + 1, 0.0));
	sectionTag = 401;
	for (int i = 0; i < ns; ++i) {

		int elementTag = 25100 + 100 * i + 1;
		for (int j = 0; j < numOfBaysY; ++j) {

			for (int k = 0; k <= numOfBaysX; ++k) {

				int jointITag = (i + 1) * numOfJointsPerFloor + 1 + (numOfBaysX + 1) * j + k;
				int jointJTag = (i + 1) * numOfJointsPerFloor + 1 + (numOfBaysX + 1) * (j + 1) + k;

				if (shearWallJointMap.count(jointITag) && j != 0) {
					jointITag = shearWallJointMap[jointITag].second;
				}

				if (shearWallJointMap.count(jointJTag) && j != numOfBaysY - 1) {
					jointJTag = shearWallJointMap[jointJTag].first;
				}

				auto coordI = api::getCoordinates(jointITag);
				auto coordJ = api::getCoordinates(jointJTag);
				double length = (coordJ - coordI).norm2();
				double depth = getDepth(length);

				if (sectionTagsY[j][k] == 0) {

					api::addElasticSection1D(sectionTag, 1, new physicalModel::Rectangle(width, depth));
					sectionTagsY[j][k] = sectionTag;
					currentSectionTag = sectionTag++;
				}
				else {
					currentSectionTag = sectionTagsY[j][k];
				}

				api::addBeam(elementTag, { jointITag, jointJTag }, currentSectionTag, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
				api::setSectionModifiers(elementTag, 0, 1.0, beamCrackedSectionModifier, 1.0, 1.0);

				if (0 == i) {
				    
				    totalBeamIndex += api::getMomentOfInertiaYY(elementTag) / std::pow(bayWidthsY[j], 3.0);
				}
    
				++elementTag;
			}
		}
	}

	buildingInfo["beam"]["totalBeamIndex"] = totalBeamIndex;
}

void RegularPlanBuildingGenerator::meshAreaElements()
{
	api::updateAreaElementProperties();

	auto shearWallTags = api::getShearWallElementTags();
	for (int i = 0; i < shearWallTags.size(); ++i) {

		auto jointTags = api::getAreaElementJointTags(shearWallTags[i]);
		auto coordI = api::getCoordinates(jointTags[0]);
		auto coordJ = api::getCoordinates(jointTags[1]);
		auto coordK = api::getCoordinates(jointTags[2]);

		int n1 = (coordJ - coordI).norm2() / m_parameters.meshInfo.meshSensitivity;
		n1 = n1 % 2 == 0 ? n1 : n1 + 1;
		int n2 = (coordK - coordJ).norm2() / m_parameters.meshInfo.meshSensitivity; 
		n2 = n2 % 2 == 0 ? n2 : n2 + 1;

		api::meshAreaElement(shearWallTags[i], n1, n2);
	}

	if (m_parameters.meshInfo.meshSlabBeam) {
		auto slabTags = api::getSlabElementTags();
		for (int i = 0; i < slabTags.size(); ++i) {

			api::meshAreaElement(slabTags[i]);
		}
	}
}

void RegularPlanBuildingGenerator::applyModelingPreferences(json& buildingInfo)
{
	int ns = buildingInfo["numberOfStoreys"];

	api::includeMassFromMembers(m_parameters.modelingPreferences.includeMassFromMembers);
	api::updateMassSourceFromMembers();

	if (m_parameters.modelingPreferences.makeFloorsRigid) {

		int masterNodeTag = 10010;
		for (int i = 1; i <= ns; ++i) {

			api::makeRigid(i, masterNodeTag);

			if (m_parameters.modelingPreferences.includeMassFromMembers) {

				api::confineFloorMassOnDiaphragmNode(i, true);

				if (1 == i) {
					auto massCenterCoord = api::getMassCenter(i);

					if (std::nullopt != massCenterCoord) {

						buildingInfo["massCenter"]["coordX"] = massCenterCoord.value().x;
						buildingInfo["massCenter"]["coordY"] = massCenterCoord.value().y;
					}
					
				}
			}
			
			masterNodeTag += 10;
		}
	}

	api::disableSlabElements(m_parameters.modelingPreferences.disableSlabElements);
	api::includePDeltaEffects(m_parameters.modelingPreferences.includePDeltaEffects);

	if (m_parameters.modelingPreferences.disableSlabElements) {

		api::applyGravityLoadThroughLineElements(true);
	}
	else {

		api::applyGravityLoadThroughLineElements(m_parameters.modelingPreferences.gravityThroughLineElements);
	}

	api::includeDeadLoadFromMembers(m_parameters.modelingPreferences.includeDeadLoadFromMembers);
}

std::unordered_map<std::string, bool> RegularPlanBuildingGenerator::analyze()

{
	buildingModeler::BuildingModelerAPI::createAnalyticalModel();
	buildingModeler::BuildingModelerAPI::createModelAndLoadingFiles();
	return buildingModeler::BuildingModelerAPI::analyze();
}

void RegularPlanBuildingGenerator::fetchResultsForGravityAnalysis(json& buildingInfo)
{
	int ns = buildingInfo["numberOfStoreys"];

	fetchAxialLoadDistribution("gravity", buildingInfo);
	fetchBeamDisplacementDistribution("gravity", ns, buildingInfo);
}

void RegularPlanBuildingGenerator::fetchResultsForEarthquakeAnalysis(json& buildingInfo)
{
	fetchAxialLoadDistribution("earthquake", buildingInfo);
	fetchBaseShearInXDirDistribution("earthquake", buildingInfo);
	fetchMomentInXDirDistribution("earthquake", buildingInfo);
	fetchDriftRatioDistribution("earthquake", buildingInfo);
}

void RegularPlanBuildingGenerator::fetchResultsForModalAnalysis(json& buildingInfo)
{
	fetchPeriods("modal", buildingInfo);
	fetchFundamentalPeriodInGivenDirection("modal", buildingInfo);
	fetchMassParticipationRatioInXDir("modal", buildingInfo);
}

void RegularPlanBuildingGenerator::fetchAxialLoadDistribution(std::string analysisName, json& buildingInfo)
{
	int numOfBaysX = buildingInfo["numberOfBaysX"];
	int numOfBaysY = buildingInfo["numberOfBaysY"];

	std::vector<std::vector<int>> shearWallArrangement(m_parameters.geometricParameters.maxNumberOfBays + 1);
	for (int i = 0; i <= m_parameters.geometricParameters.maxNumberOfBays; ++i) {
		buildingInfo["shearWall"]["modifiedArrangement"][std::to_string(i)].get_to(shearWallArrangement[i]);
	}

	for (int i = 0; i <= m_parameters.geometricParameters.maxNumberOfBays; ++i) {
		buildingInfo["output"][analysisName]["axialLoadDistribution"][std::to_string(i)] = std::vector<double>(m_parameters.geometricParameters.maxNumberOfBays + 1, 0);
	}

	double totalAxialLoad = 0.0;
	for (int i = 0; i <= numOfBaysY; ++i) {

		std::vector<int> verticalMembersOnBay = buildingInfo["verticalMemberPlan"][std::to_string(i)];

		for (int j = 0; j <= numOfBaysX; ++j) {

			int elementTag = verticalMembersOnBay[j];


			if (0 == elementTag) {
				continue;
			}

			double axialLoad;
			if (1 == shearWallArrangement[i][j]) {
				axialLoad = api::getShearWallForceZ(elementTag, analysisName);
				buildingInfo["output"][analysisName]["axialLoadDistribution"][std::to_string(i)][j] = axialLoad;
			}
			else {
				axialLoad = api::getLineElementForceZ(elementTag, analysisName);
				buildingInfo["output"][analysisName]["axialLoadDistribution"][std::to_string(i)][j] = axialLoad;
			}

			totalAxialLoad += axialLoad;
		}
	}

	buildingInfo["output"][analysisName]["totalAxialLoad"] = totalAxialLoad;
}

void RegularPlanBuildingGenerator::fetchBaseShearInXDirDistribution(std::string analysisName, json& buildingInfo)
{
	int numOfBaysX = buildingInfo["numberOfBaysX"];
	int numOfBaysY = buildingInfo["numberOfBaysY"];

	std::vector<std::vector<int>> shearWallArrangement(m_parameters.geometricParameters.maxNumberOfBays + 1);
	for (int i = 0; i <= m_parameters.geometricParameters.maxNumberOfBays; ++i) {
		buildingInfo["shearWall"]["modifiedArrangement"][std::to_string(i)].get_to(shearWallArrangement[i]);
	}

	for (int i = 0; i <= m_parameters.geometricParameters.maxNumberOfBays; ++i) {
		buildingInfo["output"][analysisName]["baseShearDistribution"][std::to_string(i)] = std::vector<double>(m_parameters.geometricParameters.maxNumberOfBays + 1, 0);
	}

	double totalBaseShear = 0.0;
	double baseShearFromWallsInYDir = 0.0;
	for (int i = 0; i <= numOfBaysY; ++i) {

		std::vector<int> verticalMembersOnBay = buildingInfo["verticalMemberPlan"][std::to_string(i)];

		for (int j = 0; j <= numOfBaysX; ++j) {

			int elementTag = verticalMembersOnBay[j];


			if (0 == elementTag) {
				continue;
			}

			double shear;
			if (1 == shearWallArrangement[i][j]) {
				shear = api::getShearWallForceX(elementTag, analysisName);
				buildingInfo["output"][analysisName]["baseShearDistribution"][std::to_string(i)][j] = shear;

				if (j == 0 || j == numOfBaysX) {
					baseShearFromWallsInYDir += shear;
				}
			}
			else {
				shear = api::getLineElementForceX(elementTag, analysisName);
				buildingInfo["output"][analysisName]["baseShearDistribution"][std::to_string(i)][j] = shear;
			}

			totalBaseShear += shear;
		}
	}

	buildingInfo["output"][analysisName]["totalBaseShear"] = totalBaseShear;
	buildingInfo["output"][analysisName]["baseShearFromWallsInYDir"] = baseShearFromWallsInYDir;
}

void RegularPlanBuildingGenerator::fetchMomentInXDirDistribution(std::string analysisName, json& buildingInfo)
{
	int numOfBaysX = buildingInfo["numberOfBaysX"];
	int numOfBaysY = buildingInfo["numberOfBaysY"];

	std::vector<std::vector<int>> shearWallArrangement(m_parameters.geometricParameters.maxNumberOfBays + 1);
	for (int i = 0; i <= m_parameters.geometricParameters.maxNumberOfBays; ++i) {
		buildingInfo["shearWall"]["modifiedArrangement"][std::to_string(i)].get_to(shearWallArrangement[i]);
	}

	for (int i = 0; i <= m_parameters.geometricParameters.maxNumberOfBays; ++i) {
		buildingInfo["output"][analysisName]["momentDistribution"][std::to_string(i)] = std::vector<double>(m_parameters.geometricParameters.maxNumberOfBays + 1, 0);
	}
	
	double totalMoment = 0.0;
	double momentFromWallsInYDir = 0.0;
	for (int i = 0; i <= numOfBaysY; ++i) {

		std::vector<int> verticalMembersOnBay = buildingInfo["verticalMemberPlan"][std::to_string(i)];

		for (int j = 0; j <= numOfBaysX; ++j) {

			int elementTag = verticalMembersOnBay[j];


			if (0 == elementTag) {
				continue;
			}

			double moment;
			if (1 == shearWallArrangement[i][j]) {
				moment = api::getShearWallMomentYY(elementTag, analysisName);
				buildingInfo["output"][analysisName]["momentDistribution"][std::to_string(i)][j] = moment;

				if (j == 0 || j == numOfBaysX) {
					momentFromWallsInYDir += moment;
				}
			}
			else {
				moment = api::getLineElementMomentYY(elementTag, analysisName);
				buildingInfo["output"][analysisName]["momentDistribution"][std::to_string(i)][j] = moment;
			}

			totalMoment += moment;
		}
	}

	buildingInfo["output"][analysisName]["totalMoment"] = totalMoment;
	buildingInfo["output"][analysisName]["momentFromWallsInYDir"] = momentFromWallsInYDir;
}

void RegularPlanBuildingGenerator::fetchBeamDisplacementDistribution(std::string analysisName, int floorNumber, json& buildingInfo)
{
	auto beamTags = api::getBeamTags(floorNumber);

	double minDelta = std::numeric_limits<double>::max();
	double maxDelta = std::numeric_limits<double>::min();
	double totalDelta = 0;

	for (auto it = beamTags.begin(); it != beamTags.end(); it++) {

		auto length = api::getLength(*it);
		int numOfSegments = length / m_parameters.meshInfo.meshSensitivity;
		numOfSegments = numOfSegments % 2 == 0 ? numOfSegments : numOfSegments + 1;
		double disp = api::getLineElementDisplacement(*it, numOfSegments / 2, analysisName, 3);
		double delta = std::abs(disp / length);

		minDelta = std::min(delta, minDelta);
		maxDelta = std::max(delta, maxDelta);
		totalDelta += delta;
	}

	auto aveDelta = totalDelta / (double)beamTags.size();

	buildingInfo["output"][analysisName]["minBeamDelta"][std::to_string(floorNumber)] = minDelta;
	buildingInfo["output"][analysisName]["maxBeamDelta"][std::to_string(floorNumber)] = maxDelta;
	buildingInfo["output"][analysisName]["averageBeamDelta"][std::to_string(floorNumber)] = aveDelta;
}

void RegularPlanBuildingGenerator::fetchDriftRatioDistribution(std::string analysisName, json& buildingInfo)
{
	int ns = buildingInfo["numberOfStoreys"];
	int numOfBaysX = buildingInfo["numberOfBaysX"];
	int numOfBaysY = buildingInfo["numberOfBaysY"];

	buildingInfo["output"][analysisName]["driftRatioBuilding"] = api::getBuildingDR(analysisName, 1);

	int bottomRightJoint = (numOfBaysX + 1) * (numOfBaysY + 1) + numOfBaysX + 1;
	int topRightJoint = 2 * (numOfBaysX + 1) * (numOfBaysY + 1);
	int masterJoint = api::getMasterJointTag(1);

	buildingInfo["output"][analysisName]["bottomRight"]["xDisp"] = api::getDisplacements(bottomRightJoint, analysisName)[0];
	buildingInfo["output"][analysisName]["bottomRight"]["yDisp"] = api::getDisplacements(bottomRightJoint, analysisName)[1];
	buildingInfo["output"][analysisName]["bottomRight"]["rot"] = api::getDisplacements(bottomRightJoint, analysisName)[5];
	buildingInfo["output"][analysisName]["topRight"]["xDisp"] = api::getDisplacements(topRightJoint, analysisName)[0];
	buildingInfo["output"][analysisName]["topRight"]["yDisp"] = api::getDisplacements(topRightJoint, analysisName)[1];
	buildingInfo["output"][analysisName]["topRight"]["rot"] = api::getDisplacements(topRightJoint, analysisName)[5];
	buildingInfo["output"][analysisName]["masterJoint"]["xDisp"] = api::getDisplacements(masterJoint, analysisName)[0];
	buildingInfo["output"][analysisName]["masterJoint"]["yDisp"] = api::getDisplacements(masterJoint, analysisName)[1];
	buildingInfo["output"][analysisName]["masterJoint"]["rot"] = api::getDisplacements(masterJoint, analysisName)[5];
}

void RegularPlanBuildingGenerator::fetchPeriods(std::string analysisName, json& buildingInfo)
{
	int ns = buildingInfo["numberOfStoreys"];
	int numOfModes = std::min(2 * ns, 12);

	auto periods = api::getPeriods("modal");
	
	for (int i = 1; i <= numOfModes; ++i) {
		buildingInfo["output"][analysisName]["periods"][std::to_string(i)] = periods[i - 1];
	}
}

void RegularPlanBuildingGenerator::fetchFundamentalPeriodInGivenDirection(std::string analysisName, json& buildingInfo)
{
	buildingInfo["output"][analysisName]["fundamentalPeriodX"] = api::getFundamentalPeriod("modal", 1);
}

void RegularPlanBuildingGenerator::fetchMassParticipationRatioInXDir(std::string analysisName, json& buildingInfo)
{
	int ns = buildingInfo["numberOfStoreys"];
	int numOfModes = std::min(2 * ns, 12);

	// Defining l vector
	std::vector<double> lX(3 * ns, 0.0);

	for (int i = 0; i < (3 * ns); i++)
	{
		if ((i + 1) % 3 == 1)
			lX[i] = 1.0;
	}

	// Defining mass vector
	double totalMass = 0.0;
	std::vector<double> massVec(3 * ns, 0.0);
	for (int i = 1; i <= ns; ++i) {
        auto mass = api::getDiaphragmMass(i);

		if (mass == std::nullopt) {
			return;
		}
		else {
			massVec[(i - 1) * 3] = mass.value().x;
			massVec[(i - 1) * 3 + 1] = mass.value().y;
			massVec[(i - 1) * 3 + 2] = mass.value().z;

			totalMass += mass.value().x;
		}
	}

	// Calculating L vector
	std::vector<double> LX(numOfModes, 0.0);
	std::vector<double> LY(numOfModes, 0.0);

	auto modeShapeX = api::getModeShapeX("modal");
	auto modeShapeY = api::getModeShapeY("modal");
	auto modeShapeXY = api::getModeShapeXY("modal");

	std::vector<std::vector<double>> modeShapes (numOfModes, std::vector<double>(3 * ns));
	for (int i = 0; i < numOfModes; i++)
	{
		for (int j = 0; j < ns; j++)
		{
			modeShapes[i][3 * j] = modeShapeX[i][j];
			modeShapes[i][3 * j + 1] = modeShapeY[i][j];
			modeShapes[i][3 * j + 2] = modeShapeXY[i][j];
		}
	}

	for (int i = 0; i < numOfModes; i++)
	{
		double vecSumX = 0;

		for (int j = 0; j < (3 * ns); j++)
		{
			vecSumX += modeShapes[i][j] * massVec[j] * lX[j];
		}

		LX[i] = vecSumX;
	}

	// Calculating modal mass vector
	std::vector<double> modalMassVec(numOfModes, 0.0);

	for (int i = 0; i < numOfModes; i++)
	{
		double vecSum = 0;

		for (int j = 0; j < (3 * ns); j++)
			vecSum += modeShapes[i][j] * massVec[j] * modeShapes[i][j];


		modalMassVec[i] = vecSum;
	}

	// Calculating modal participation factor
	std::vector<double> modalParticipationX(numOfModes, 0.0);

	for (int i = 0; i < numOfModes; i++)
	{
		modalParticipationX[i] = LX[i] / modalMassVec[i];
	}

	// Calculating mass participation ratios
	std::vector<double> massParticipationX(numOfModes, 0.0);

	for (int i = 0; i < numOfModes; i++)
	{
		massParticipationX[i] = pow(LX[i], 2) / modalMassVec[i] / totalMass;
	}

	for (int i = 1; i <= numOfModes; ++i) {
		buildingInfo["output"][analysisName]["massParticipationX"][std::to_string(i)] = massParticipationX[i - 1];
	}
}

std::vector<std::vector<std::vector<double>>> RegularPlanBuildingGenerator::getShearWallArrangement(int numOfBaysLongDir, int numOfBaysPerpDir, std::vector<double> bayWidthsLongDir, std::vector<double> bayWidthsPerpDir, double thickness, double& shearWallRatioX, double& shearWallRatioY)
{
	std::vector<std::vector<std::vector<double>>> shearWallArrangement(2);
	shearWallArrangement[0] = std::vector<std::vector<double>>(bayWidthsPerpDir.size() + 1, std::vector<double>(bayWidthsLongDir.size() + 1, -1));
	for (int i = 0; i <= numOfBaysPerpDir; ++i) {
		for (int j = 0; j <= numOfBaysLongDir; ++j) {
			shearWallArrangement[0][i][j] = 0;
		}
	}
	shearWallArrangement[1] = std::vector<std::vector<double>>(bayWidthsLongDir.size() + 1, std::vector<double>(bayWidthsPerpDir.size() + 1, -1));
	for (int i = 0; i <= numOfBaysLongDir; ++i) {
		for (int j = 0; j <= numOfBaysPerpDir; ++j) {
			shearWallArrangement[1][i][j] = 0;
		}
	}

	if (!m_parameters.shearWallParameters.includeShearWalls) {
		return shearWallArrangement;
	}

	double lengthLong = 0.0;
	for (int i = 0; i < numOfBaysLongDir; ++i) {
		lengthLong += bayWidthsLongDir[i];
	}

	double lengthPerp = 0.0;
	for (int i = 0; i < numOfBaysPerpDir; ++i) {
		lengthPerp += bayWidthsPerpDir[i];
	}

	double buildingArea = lengthLong * lengthPerp;

	// Perimeter shear wall arrangement
	int chosenConfigurationXDir;
	int chosenConfigurationYDir;
	auto perimeterArrangementsX = getPerimeterShearWallLocations(numOfBaysLongDir, bayWidthsLongDir, thickness, buildingArea);
	auto perimeterArrangementsY = getPerimeterShearWallLocations(numOfBaysPerpDir, bayWidthsPerpDir, thickness, buildingArea);
	std::uniform_int_distribution<int> perimeterArrangementsXDist(0, perimeterArrangementsX.size());
	std::uniform_int_distribution<int> perimeterArrangementsYDist(0, perimeterArrangementsY.size());
	chosenConfigurationXDir = perimeterArrangementsXDist(m_generator);
	chosenConfigurationYDir = perimeterArrangementsYDist(m_generator);

	// Final shear wall arrangement
	if (chosenConfigurationXDir > 0) {
		shearWallArrangement[0][0] = perimeterArrangementsX[chosenConfigurationXDir - 1];
		shearWallArrangement[0][numOfBaysPerpDir] = perimeterArrangementsX[chosenConfigurationXDir - 1];

		for (const auto arrangementX : perimeterArrangementsX[chosenConfigurationXDir - 1]) {
			if (arrangementX >= 0.001) {
				shearWallRatioX += 2 * thickness * arrangementX / buildingArea;
			}
		}
	}

	if (chosenConfigurationYDir > 0) {
		shearWallArrangement[1][0] = perimeterArrangementsY[chosenConfigurationYDir - 1];
		shearWallArrangement[1][numOfBaysLongDir] = perimeterArrangementsY[chosenConfigurationYDir - 1];

		for (const auto arrangementY : perimeterArrangementsY[chosenConfigurationYDir - 1]) {
			if (arrangementY >= 0.001) {
				shearWallRatioY += 2 * thickness * arrangementY / buildingArea;
			}
		}
	}

	return shearWallArrangement;
}

std::vector<std::vector<double>> RegularPlanBuildingGenerator::getPerimeterShearWallLocations(int numOfBays, std::vector<double> bayWidths, double thickness, double buildingArea)
{
	std::vector<std::vector<double>> perimeterLocations;
	std::vector<std::vector<int>> perimeterLocationPool;

	switch (numOfBays) {
		case 2:
			perimeterLocationPool.push_back({1});
			break;
		case 3:
			perimeterLocationPool.push_back({1, 2});
			break;
		case 4:
			perimeterLocationPool.push_back({2});
			perimeterLocationPool.push_back({1, 3});
			break;
		case 5:
			perimeterLocationPool.push_back({2, 3});
			perimeterLocationPool.push_back({1, 4});
			break;
		case 6:
			perimeterLocationPool.push_back({3});
			perimeterLocationPool.push_back({2, 4});
			perimeterLocationPool.push_back({1, 5});
			perimeterLocationPool.push_back({1, 3, 5});
			break;
		case 7:
			perimeterLocationPool.push_back({3, 4});
			perimeterLocationPool.push_back({2, 5});
			perimeterLocationPool.push_back({1, 6});
			break;
		case 8:
			perimeterLocationPool.push_back({4});
			perimeterLocationPool.push_back({3, 5});
			perimeterLocationPool.push_back({2, 6});
			perimeterLocationPool.push_back({1, 7});
			perimeterLocationPool.push_back({2, 4, 6});
			perimeterLocationPool.push_back({1, 4, 7});
			break;
	}

	double maxAllowableShearWallArea = m_parameters.shearWallParameters.maxShearWallRatio * buildingArea;

	for (int i = 0; i < perimeterLocationPool.size(); ++i) {

		double maxLength = maxAllowableShearWallArea / thickness / 2.0 / (double)perimeterLocationPool[i].size();

		if (maxLength > 1.5999) {
			std::vector<double> locations;
			for (int j = 0; j <= bayWidths.size(); ++j) {
				
				if (j <= numOfBays) {
					locations.push_back(0);
				}
				else {
					locations.push_back(-1);
				}
			}

			std::optional<double> midLength;

			if (perimeterLocationPool[i].size() & 1) {
				maxLength = std::min(maxLength, 0.7 * bayWidths[numOfBays / 2]);
				std::uniform_real_distribution<double> midShearWallLength(std::min(1.6001, maxLength), std::max(1.6001, maxLength));

				midLength = 0.2 * std::round(midShearWallLength(m_generator) / 0.2);
				maxLength = (maxAllowableShearWallArea / thickness / 2.0 - midLength.value()) / 2.0;
			}

			if (perimeterLocationPool[i].size() > 1) {
				
				if (midLength != std::nullopt) {
					locations[perimeterLocationPool[i][1]] = midLength.value();
				}

				int index = perimeterLocationPool[i][0];
				maxLength = std::min(maxLength, 0.7 * std::min(bayWidths[index - 1], bayWidths[index]));

				std::uniform_real_distribution<double> edgeShearWallLength(std::min(1.6001, maxLength), std::max(1.6001, maxLength));
				auto edgeLength = 0.2 * std::round(edgeShearWallLength(m_generator) / 0.2);
				
				locations[perimeterLocationPool[i].front()] = edgeLength;
				locations[perimeterLocationPool[i].back()] = edgeLength;
			}
			else {
				locations[perimeterLocationPool[i][0]] = midLength.value();
			}

			perimeterLocations.push_back(locations);
		}

	}

	return perimeterLocations;
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