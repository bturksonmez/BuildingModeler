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

json RegularPlanBuildingGenerator::generateAndAnalyze()
{
	m_seed = std::chrono::system_clock::now().time_since_epoch().count() + counter++;
	m_generator.seed(m_seed);

	api::clear();

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

	// Generate joints
	generateJoints(buildingInfo);

	// Generate materials
	std::uniform_real_distribution<double> youngsModulusDist(m_parameters.minConcreteYoungsModulus, m_parameters.maxConcreteYoungsModulus);
	std::uniform_real_distribution<double> shearWallCrackedSectionModifierDist(m_parameters.shearWallParameters.minShearWallCrackedSectionModifier, m_parameters.shearWallParameters.maxShearWallCrackedSectionModifier);
	double E = youngsModulusDist(m_generator);
	double crackedModShearWall = shearWallCrackedSectionModifierDist(m_generator);
	buildingInfo["youngsModulus"] = E;
	buildingInfo["shearWall"]["crackedSectionModifier"] = crackedModShearWall;
	generateMaterials(E, crackedModShearWall);

	//Generate shear walls
	std::uniform_real_distribution<double> shearWallThicknessDist(m_parameters.shearWallParameters.minShearWallThickness, m_parameters.shearWallParameters.maxShearWallThickness);
	double tShearWall = shearWallThicknessDist(m_generator);
	buildingInfo["shearWall"]["thickness"] = tShearWall;
	double shearWallRatioX;
	double shearWallRatioY;
	std::vector<std::vector<std::vector<int>>> shearWallArrangement;
	if (m_parameters.shearWallParameters.includeShearWalls) {
		std::pair<int, int> coreLocation {-1, -1};
		shearWallArrangement = getShearWallArrangement(numOfBaysX, numOfBaysY, bayWidthsX, bayWidthsY, tShearWall, shearWallRatioX, shearWallRatioY, coreLocation);
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

		buildingInfo["shearWall"]["coreLocationX"] = coreLocation.first;
		buildingInfo["shearWall"]["coreLocationY"] = coreLocation.second;
	}
	generateShearWalls(shearWallArrangement, tShearWall, buildingInfo);
	
	// Generate slabs
	std::uniform_real_distribution<double> slabThicknessDist(m_parameters.slabParameters.minSlabThickness, m_parameters.slabParameters.maxSlabThickness);
	double tSlab = slabThicknessDist(m_generator);
	buildingInfo["slab"]["thickness"] = tSlab;
	generateSlabs(tSlab, buildingInfo);
	
	// Generate columns
	std::vector<std::vector<int>> modifiedShearWallArrangement(m_parameters.geometricParameters.maxNumberOfBays + 1, std::vector<int>(m_parameters.geometricParameters.maxNumberOfBays + 1, -1));
	for (int i = 0; i <= numOfBaysY; ++i) {

		for (int j = 0; j <= numOfBaysX; ++j) {

			modifiedShearWallArrangement[i][j] = 0;
		}
	}
	for (int i = 0; i < numOfBaysX; ++i) {

		for (int j = 0; j <= numOfBaysY; ++j) {

			if (1 == shearWallArrangement[0][j][i]) {

				modifiedShearWallArrangement[j][i]++;
				modifiedShearWallArrangement[j][i + 1]++;
			}
		}
	}
	for (int i = 0; i < numOfBaysY; ++i) {

		for (int j = 0; j <= numOfBaysX; ++j) {

			if (1 == shearWallArrangement[1][j][i]) {

				modifiedShearWallArrangement[i][j]++;
				modifiedShearWallArrangement[i + 1][j]++;
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
	generateBeams(width, minDepth, maxDepth, minLength, maxLength, crackedModBeam, buildingInfo);
	
	// Mesh area elements
	meshAreaElements();
	
	// Apply model preferences
	std::uniform_real_distribution<double> liveLoadDist(m_parameters.modelingPreferences.minLiveLoadPerArea, m_parameters.modelingPreferences.maxLiveLoadPerArea);
	std::uniform_real_distribution<double> liveLoadMassContributionDist(m_parameters.modelingPreferences.minLiveLoadMassContribution, m_parameters.modelingPreferences.maxLiveLoadMassContribution);
	double liveLoad = liveLoadDist(m_generator);
	double liveLoadMassContributionFactor = liveLoadMassContributionDist(m_generator);
	buildingInfo["loading"]["liveLoad"]["liveLoadPerArea"] = liveLoad;
	buildingInfo["loading"]["liveLoad"]["liveLoadMassParticipationFactor"] = liveLoadMassContributionFactor;
	applyModelingPreferences(liveLoad, liveLoadMassContributionFactor, buildingInfo);
	
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
	generateJoints(buildingInfo);

	// Generate materials
	double E = buildingInfo["youngsModulus"];
	double crackedModShearWall = buildingInfo["shearWall"]["crackedSectionModifier"];
	generateMaterials(E, crackedModShearWall);

	//Generate shear walls
	double tShearWall = buildingInfo["shearWall"]["thickness"];
	std::vector<std::vector<std::vector<int>>> shearWallArrangement(2, std::vector<std::vector<int>>(m_parameters.geometricParameters.maxNumberOfBays + 1));
	for (int i = 0; i <= m_parameters.geometricParameters.maxNumberOfBays; ++i) {
		buildingInfo["shearWall"]["shearWallXDir"][std::to_string(i)].get_to(shearWallArrangement[0][i]);
	}
	for (int i = 0; i <= m_parameters.geometricParameters.maxNumberOfBays; ++i) {
		buildingInfo["shearWall"]["shearWallYDir"][std::to_string(i)].get_to(shearWallArrangement[1][i]);
	}
	generateShearWalls(shearWallArrangement, tShearWall, buildingInfo);

	// Generate slabs
	double tSlab = buildingInfo["slab"]["thickness"];
	generateSlabs(tSlab, buildingInfo);

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
	generateBeams(width, minDepth, maxDepth, minLength, maxLength, crackedModBeam, buildingInfo);

	// Mesh area elements
	meshAreaElements();

	// Apply model preferences
	double liveLoad = buildingInfo["loading"]["liveLoad"]["liveLoadPerArea"];
	double liveLoadMassContributionFactor = buildingInfo["loading"]["liveLoad"]["liveLoadMassParticipationFactor"];
	applyModelingPreferences(liveLoad, liveLoadMassContributionFactor, buildingInfo);

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

	for (int i = 0; i <= m_parameters.geometricParameters.maxNumberOfBays; ++i) {
		buildingInfo["verticalMemberPlanSecondary"][std::to_string(i)] = std::vector<int>(m_parameters.geometricParameters.maxNumberOfBays + 1, 0);
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

void RegularPlanBuildingGenerator::generateMaterials(double E, double shearWallCrackedSectionModifier)
{
	double Ecracked = shearWallCrackedSectionModifier * E;

	double G = E / (1.0 + 0.2) / 2.0;
	double Gcracked = Ecracked / (1.0 + 0.2) / 2.0;

	api::addElasticMaterial(1, E, G, 2.4);
	api::addElasticMaterial(2, Ecracked, Gcracked, 2.4); // for shear walls only
}

void RegularPlanBuildingGenerator::generateShearWalls(const std::vector<std::vector<std::vector<int>>>& shearWallArrangement, double thickness, json& buildingInfo)
{
	int ns = buildingInfo["numberOfStoreys"];
	int numOfBaysX = buildingInfo["numberOfBaysX"];
	int numOfBaysY = buildingInfo["numberOfBaysY"];

	api::addElasticSection2D(301, 2, thickness);

	double totalIx = 0.0;
	int numOfJointsPerFloor = (numOfBaysX + 1) * (numOfBaysY + 1);
	for (int i = 0; i < ns; ++i) {

		int elementTag = 30100 + 100 * i + 1;
		for (int j = 0; j < shearWallArrangement[0].size(); ++j) {

			for (int k = 0; k < shearWallArrangement[0][j].size(); ++k) {

				if (1 == shearWallArrangement[0][j][k]) {

					int initJointTag = i * numOfJointsPerFloor + 1;
					int jointITag = initJointTag + (numOfBaysX + 1) * j + k;
					int jointJTag = initJointTag + (numOfBaysX + 1) * j + k + 1;
					int jointKTag = initJointTag + (numOfBaysX + 1) * j + k + 1 + numOfJointsPerFloor;
					int jointLTag = initJointTag + (numOfBaysX + 1) * j + k + numOfJointsPerFloor;

					api::addShearWall(elementTag, { jointITag, jointJTag, jointKTag, jointLTag }, 301, physicalModel::AreaElementFormulation::LINEAR);

					if (0 == i) {
						buildingInfo["verticalMemberPlan"][std::to_string(j)][k] = elementTag;
						buildingInfo["verticalMemberPlan"][std::to_string(j)][k + 1] = elementTag;

						auto coordI = api::getCoordinates(jointITag);
						auto coordJ = api::getCoordinates(jointJTag);
						auto length = (coordJ - coordI).norm2();
						auto I = (1.0 / 12.0) * thickness * std::pow(length, 3);
						totalIx += I;
					}

					++elementTag;
				}
			}
		}

		for (int j = 0; j < shearWallArrangement[1].size(); ++j) {

			for (int k = 0; k < shearWallArrangement[1][j].size(); ++k) {

				if (1 == shearWallArrangement[1][j][k]) {

					int initJointTag = i * numOfJointsPerFloor + 1;
					int jointITag = initJointTag + (numOfBaysX + 1) * (k + 1) + j;
					int jointJTag = initJointTag + (numOfBaysX + 1) * k + j;
					int jointKTag = initJointTag + (numOfBaysX + 1) * k + j + numOfJointsPerFloor;
					int jointLTag = initJointTag + (numOfBaysX + 1) * (k + 1) + j + numOfJointsPerFloor;

					api::addShearWall(elementTag, { jointITag, jointJTag, jointKTag, jointLTag }, 301, physicalModel::AreaElementFormulation::LINEAR);

					if (0 == i) {
						buildingInfo["verticalMemberPlanSecondary"][std::to_string(k)][j] = elementTag;
						buildingInfo["verticalMemberPlanSecondary"][std::to_string(k + 1)][j] = elementTag;
					}

					++elementTag;
				}
			}
		}
	}

	buildingInfo["shearWall"]["shearWallXDir"]["momentOfInertia"] = totalIx;
}

void RegularPlanBuildingGenerator::generateSlabs(double thickness, json& buildingInfo)
{
	int ns = buildingInfo["numberOfStoreys"];
	int numOfBaysX = buildingInfo["numberOfBaysX"];
	int numOfBaysY = buildingInfo["numberOfBaysY"];
	int coreLocationX = buildingInfo["shearWall"]["coreLocationX"];
	int coreLocationY = buildingInfo["shearWall"]["coreLocationY"];

	api::addElasticSection2D(401, 1, thickness);

	int numOfJointsPerFloor = (numOfBaysX + 1) * (numOfBaysY + 1);
	for (int i = 0; i < ns; ++i) {

		int elementTag = 40100 + 100 * i + 1;
		for (int j = 0; j < numOfBaysY; ++j) {

			for (int k = 0; k < numOfBaysX; ++k) {

				if (k == coreLocationX && j == coreLocationY) {
					continue;
				}

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
					bool isStrong;

					if (columnArrangement[j][k] == -1) {
						columnArrangement[j][k] = columnOrientationDist(m_generator);
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

void RegularPlanBuildingGenerator::generateBeams(double width, double minDepth, double maxDepth, double minLength, double maxLength, double beamCrackedSectionModifier, json& buildingInfo)
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

			std::vector<int> shearWallArrangementX = buildingInfo["shearWall"]["shearWallXDir"][std::to_string(j)];

			for (int k = 0; k < numOfBaysX; ++k) {

				int jointITag = (i + 1) * numOfJointsPerFloor + 1 + (numOfBaysX + 1) * j + k;
				int jointJTag = (i + 1) * numOfJointsPerFloor + 1 + (numOfBaysX + 1) * j + k + 1;

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

				if (0 == shearWallArrangementX[k]) {

					api::addBeam(elementTag, { jointITag, jointJTag }, currentSectionTag, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
					api::setSectionModifiers(elementTag, 0, 1.0, beamCrackedSectionModifier, 1.0, 1.0);

					if (m_parameters.meshInfo.meshSlabBeam) {

						int numOfSegments = bayWidthsX[k] / m_parameters.meshInfo.meshSensitivity;
						numOfSegments = numOfSegments % 2 == 0 ? numOfSegments : numOfSegments + 1;
						std::vector<double> segmentRatios(numOfSegments, 1.0 / (double)numOfSegments);
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

			double depth = getDepth(bayWidthsY[j]);

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

				std::vector<int> shearWallArrangementY = buildingInfo["shearWall"]["shearWallYDir"][std::to_string(k)];

				int jointITag = (i + 1) * numOfJointsPerFloor + 1 + (numOfBaysX + 1) * j + k;
				int jointJTag = (i + 1) * numOfJointsPerFloor + 1 + (numOfBaysX + 1) * (j + 1) + k;

				if (0 == shearWallArrangementY[j])
				{
					api::addBeam(elementTag, { jointITag, jointJTag }, currentSectionTag, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
				    api::setSectionModifiers(elementTag, 0, 1.0, beamCrackedSectionModifier, 1.0, 1.0);
    
				    if (m_parameters.meshInfo.meshSlabBeam) {
				    
				    	int numOfSegments = bayWidthsY[j] / m_parameters.meshInfo.meshSensitivity;
				    	numOfSegments = numOfSegments % 2 == 0 ? numOfSegments : numOfSegments + 1;
				    	std::vector<double> segmentRatios(numOfSegments, 1.0 / (double)numOfSegments);
				    	api::setSegmentRatios(elementTag, segmentRatios);
				    }
    
				    if (0 == i) {
				    
				    	totalBeamIndex += api::getMomentOfInertiaYY(elementTag) / std::pow(bayWidthsY[j], 3.0);
				    }
    
				    ++elementTag;
				}
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

void RegularPlanBuildingGenerator::applyModelingPreferences(double liveLoad, double liveLoadMassContributionFactor, json& buildingInfo)
{
	int ns = buildingInfo["numberOfStoreys"];

	for (int i = 1; i <= ns; ++i) {

		api::setLiveLoadForFloor(i, liveLoad);
		api::setLiveLoadMassContributionForFloor(i, liveLoadMassContributionFactor);
	}

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
	fetchFundamentalPeriodInGivenDirection("modal", buildingInfo);
}

void RegularPlanBuildingGenerator::fetchAxialLoadDistribution(std::string analysisName, json& buildingInfo)
{
	int numOfBaysX = buildingInfo["numberOfBaysX"];
	int numOfBaysY = buildingInfo["numberOfBaysY"];

	std::vector<std::vector<int>> shearWallArrangementX(m_parameters.geometricParameters.maxNumberOfBays + 1);
	for (int i = 0; i <= m_parameters.geometricParameters.maxNumberOfBays; ++i) {
		buildingInfo["shearWall"]["shearWallXDir"][std::to_string(i)].get_to(shearWallArrangementX[i]);
	}

	std::vector<std::vector<int>> shearWallArrangementY(m_parameters.geometricParameters.maxNumberOfBays + 1);
	for (int i = 0; i <= m_parameters.geometricParameters.maxNumberOfBays; ++i) {
		buildingInfo["shearWall"]["shearWallYDir"][std::to_string(i)].get_to(shearWallArrangementY[i]);
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
			if (j != numOfBaysX && 1 == shearWallArrangementX[i][j]) {

				axialLoad = api::getShearWallForceZ(elementTag, analysisName);
				buildingInfo["output"][analysisName]["axialLoadDistribution"][std::to_string(i)][j] = axialLoad / 2.0;
				buildingInfo["output"][analysisName]["axialLoadDistribution"][std::to_string(i)][j + 1] = axialLoad / 2.0;
				++j;
			}
			else {
				axialLoad = api::getLineElementForceZ(elementTag, analysisName);
				buildingInfo["output"][analysisName]["axialLoadDistribution"][std::to_string(i)][j] = axialLoad;
			}

			totalAxialLoad += axialLoad;
		}
	}

	std::vector<std::vector<int>> verticalSecondaryMembers(m_parameters.geometricParameters.maxNumberOfBays + 1);
	for (int i = 0; i <= m_parameters.geometricParameters.maxNumberOfBays; ++i) {
		buildingInfo["verticalMemberPlanSecondary"][std::to_string(i)].get_to(verticalSecondaryMembers[i]);
	}

	for (int i = 0; i <= m_parameters.geometricParameters.maxNumberOfBays; ++i) {

		for (int j = 0; j < m_parameters.geometricParameters.maxNumberOfBays; ++j) {

			int elementTag = verticalSecondaryMembers[j][i];

			if (0 == elementTag) {
				continue;
			}

			double axialLoad = 0;
			if (1 == shearWallArrangementY[i][j]) {

				axialLoad = api::getShearWallForceZ(elementTag, analysisName);
				double tempLoad1 = buildingInfo["output"][analysisName]["axialLoadDistribution"][std::to_string(j)][i];
				double tempLoad2 = buildingInfo["output"][analysisName]["axialLoadDistribution"][std::to_string(j+1)][i];
				buildingInfo["output"][analysisName]["axialLoadDistribution"][std::to_string(j)][i] = tempLoad1 + axialLoad / 2.0;
				buildingInfo["output"][analysisName]["axialLoadDistribution"][std::to_string(j+1)][i] = tempLoad2 + axialLoad / 2.0;
				++j;
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

	std::vector<std::vector<int>> shearWallArrangementX(m_parameters.geometricParameters.maxNumberOfBays + 1);
	for (int i = 0; i <= m_parameters.geometricParameters.maxNumberOfBays; ++i) {
		buildingInfo["shearWall"]["shearWallXDir"][std::to_string(i)].get_to(shearWallArrangementX[i]);
	}

	std::vector<std::vector<int>> shearWallArrangementY(m_parameters.geometricParameters.maxNumberOfBays + 1);
	for (int i = 0; i <= m_parameters.geometricParameters.maxNumberOfBays; ++i) {
		buildingInfo["shearWall"]["shearWallYDir"][std::to_string(i)].get_to(shearWallArrangementY[i]);
	}

	for (int i = 0; i <= m_parameters.geometricParameters.maxNumberOfBays; ++i) {
		buildingInfo["output"][analysisName]["baseShearDistribution"][std::to_string(i)] = std::vector<double>(m_parameters.geometricParameters.maxNumberOfBays + 1, 0);
	}

	double totalBaseShear = 0.0;
	for (int i = 0; i <= numOfBaysY; ++i) {

		std::vector<int> verticalMembersOnBay = buildingInfo["verticalMemberPlan"][std::to_string(i)];

		for (int j = 0; j <= numOfBaysX; ++j) {

			int elementTag = verticalMembersOnBay[j];


			if (0 == elementTag) {
				continue;
			}

			double shear;
			if (j != numOfBaysX && 1 == shearWallArrangementX[i][j]) {

				shear = api::getShearWallForceX(elementTag, analysisName);
				buildingInfo["output"][analysisName]["baseShearDistribution"][std::to_string(i)][j] = shear / 2.0;
				buildingInfo["output"][analysisName]["baseShearDistribution"][std::to_string(i)][j + 1] = shear / 2.0;
				++j;
			}
			else {
				shear = api::getLineElementForceX(elementTag, analysisName);
				buildingInfo["output"][analysisName]["baseShearDistribution"][std::to_string(i)][j] = shear;
			}

			totalBaseShear += shear;
		}
	}

	double baseShearFromWallsInYDir = 0.0;
	std::vector<std::vector<int>> verticalSecondaryMembers(m_parameters.geometricParameters.maxNumberOfBays + 1);
	for (int i = 0; i <= m_parameters.geometricParameters.maxNumberOfBays; ++i) {
		buildingInfo["verticalMemberPlanSecondary"][std::to_string(i)].get_to(verticalSecondaryMembers[i]);
	}

	for (int i = 0; i <= m_parameters.geometricParameters.maxNumberOfBays; ++i) {

		for (int j = 0; j < m_parameters.geometricParameters.maxNumberOfBays; ++j) {

			int elementTag = verticalSecondaryMembers[j][i];

			if (0 == elementTag) {
				continue;
			}

			double shear = 0;
			if (1 == shearWallArrangementY[i][j]) {

				shear = api::getShearWallForceX(elementTag, analysisName);
				double temp1 = buildingInfo["output"][analysisName]["baseShearDistribution"][std::to_string(j)][i];
				double temp2 = buildingInfo["output"][analysisName]["baseShearDistribution"][std::to_string(j + 1)][i];
				temp1 += shear / 2;
				temp2 += shear / 2;
				buildingInfo["output"][analysisName]["baseShearDistribution"][std::to_string(j)][i] = temp1;
				buildingInfo["output"][analysisName]["baseShearDistribution"][std::to_string(j + 1)][i] = temp2;
				++j;
			}

			totalBaseShear += shear;
			baseShearFromWallsInYDir += shear;
		}
	}

	buildingInfo["output"][analysisName]["totalBaseShear"] = totalBaseShear;
	buildingInfo["output"][analysisName]["baseShearFromWallsInYDir"] = baseShearFromWallsInYDir;
}

void RegularPlanBuildingGenerator::fetchMomentInXDirDistribution(std::string analysisName, json& buildingInfo)
{
	int numOfBaysX = buildingInfo["numberOfBaysX"];
	int numOfBaysY = buildingInfo["numberOfBaysY"];

	std::vector<std::vector<int>> shearWallArrangementX(m_parameters.geometricParameters.maxNumberOfBays + 1);
	for (int i = 0; i <= m_parameters.geometricParameters.maxNumberOfBays; ++i) {
		buildingInfo["shearWall"]["shearWallXDir"][std::to_string(i)].get_to(shearWallArrangementX[i]);
	}

	std::vector<std::vector<int>> shearWallArrangementY(m_parameters.geometricParameters.maxNumberOfBays + 1);
	for (int i = 0; i <= m_parameters.geometricParameters.maxNumberOfBays; ++i) {
		buildingInfo["shearWall"]["shearWallYDir"][std::to_string(i)].get_to(shearWallArrangementY[i]);
	}

	for (int i = 0; i <= m_parameters.geometricParameters.maxNumberOfBays; ++i) {
		buildingInfo["output"][analysisName]["momentDistribution"][std::to_string(i)] = std::vector<double>(m_parameters.geometricParameters.maxNumberOfBays + 1, 0);
	}
	
	double totalMoment = 0.0;
	for (int i = 0; i <= numOfBaysY; ++i) {

		std::vector<int> verticalMembersOnBay = buildingInfo["verticalMemberPlan"][std::to_string(i)];

		for (int j = 0; j <= numOfBaysX; ++j) {

			int elementTag = verticalMembersOnBay[j];


			if (0 == elementTag) {
				continue;
			}

			double moment;
			if (j != numOfBaysX && 1 == shearWallArrangementX[i][j]) {

				moment = api::getShearWallMomentYY(elementTag, analysisName);
				buildingInfo["output"][analysisName]["momentDistribution"][std::to_string(i)][j] = moment / 2.0;
				buildingInfo["output"][analysisName]["momentDistribution"][std::to_string(i)][j + 1] = moment / 2.0;
				++j;
			}
			else {
				moment = api::getLineElementMomentYY(elementTag, analysisName);
				buildingInfo["output"][analysisName]["momentDistribution"][std::to_string(i)][j] = moment;
			}

			totalMoment += moment;
		}
	}

	double momentFromWallsInYDir = 0.0;
	std::vector<std::vector<int>> verticalSecondaryMembers(m_parameters.geometricParameters.maxNumberOfBays + 1);
	for (int i = 0; i <= m_parameters.geometricParameters.maxNumberOfBays; ++i) {
		buildingInfo["verticalMemberPlanSecondary"][std::to_string(i)].get_to(verticalSecondaryMembers[i]);
	}

	for (int i = 0; i <= m_parameters.geometricParameters.maxNumberOfBays; ++i) {

		for (int j = 0; j < m_parameters.geometricParameters.maxNumberOfBays; ++j) {

			int elementTag = verticalSecondaryMembers[j][i];

			if (0 == elementTag) {
				continue;
			}

			double moment = 0;
			if (1 == shearWallArrangementY[i][j]) {

				moment = api::getShearWallMomentYY(elementTag, analysisName);
				double temp1 = buildingInfo["output"][analysisName]["momentDistribution"][std::to_string(j)][i];
				double temp2 = buildingInfo["output"][analysisName]["momentDistribution"][std::to_string(j + 1)][i];
				temp1 += moment / 2;
				temp2 += moment / 2;
				buildingInfo["output"][analysisName]["momentDistribution"][std::to_string(j)][i] = temp1;
				buildingInfo["output"][analysisName]["momentDistribution"][std::to_string(j + 1)][i] = temp2;
				++j;
			}

			momentFromWallsInYDir += moment;
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

	std::vector<std::vector<int>> shearWallArrangementX(m_parameters.geometricParameters.maxNumberOfBays + 1);
	for (int i = 0; i <= m_parameters.geometricParameters.maxNumberOfBays; ++i) {
		buildingInfo["shearWall"]["shearWallXDir"][std::to_string(i)].get_to(shearWallArrangementX[i]);
	}

	buildingInfo["output"][analysisName]["driftRatioDistributionMax"] = std::vector<double>(m_parameters.geometricParameters.maxNumberOfStoreys, 0);
	buildingInfo["output"][analysisName]["driftRatioDistributionAve"] = std::vector<double>(m_parameters.geometricParameters.maxNumberOfStoreys, 0);

	for (int i = 1; i <= ns; ++i) {

		double maxDriftRatio = -1;

		auto columnTags = api::getColumnTags(i);
		for (const auto columnTag : columnTags) {

			auto driftRatio = api::getLineElementDR(columnTag, analysisName, 1);
			maxDriftRatio = std::max(driftRatio, maxDriftRatio);
		}

		auto shearWallTags = api::getShearWallTagsInXDir(i);
		for (const auto shearWallTag : shearWallTags) {

			auto driftRatio = api::getShearWallDR(shearWallTag, analysisName, 1);
			maxDriftRatio = std::max(driftRatio, maxDriftRatio);
		}

		double aveDriftRatio = api::getFloorDR(i, analysisName, 1);

		buildingInfo["output"][analysisName]["driftRatioDistributionMax"][i-1] = maxDriftRatio;
		buildingInfo["output"][analysisName]["driftRatioDistributionAve"][i-1] = aveDriftRatio;
	}

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

void RegularPlanBuildingGenerator::fetchFundamentalPeriodInGivenDirection(std::string analysisName, json& buildingInfo)
{
	buildingInfo["output"][analysisName]["fundamentalPeriodX"] = api::getFundamentalPeriod("modal", 1);
}

std::vector<std::vector<std::vector<int>>> RegularPlanBuildingGenerator::getShearWallArrangement(int numOfBaysLongDir, int numOfBaysPerpDir, std::vector<double> bayWidthsLongDir, std::vector<double> bayWidthsPerpDir, double thickness, double& shearWallRatioX, double& shearWallRatioY, std::pair<int, int>& coreLocation)
{
	std::vector<std::vector<std::vector<int>>> shearWallArrangement(2);
	shearWallArrangement[0] = std::vector<std::vector<int>>(bayWidthsPerpDir.size() + 1, std::vector<int>(bayWidthsLongDir.size(), -1));
	for (int i = 0; i <= numOfBaysPerpDir; ++i) {
		for (int j = 0; j < numOfBaysLongDir; ++j) {
			shearWallArrangement[0][i][j] = 0;
		}
	}
	shearWallArrangement[1] = std::vector<std::vector<int>>(bayWidthsLongDir.size() + 1, std::vector<int>(bayWidthsPerpDir.size(), -1));
	for (int i = 0; i <= numOfBaysLongDir; ++i) {
		for (int j = 0; j < numOfBaysPerpDir; ++j) {
			shearWallArrangement[1][i][j] = 0;
		}
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
	
	// Core shear wall arrangement for both directions
	auto coreShearWallArrangement = getCoreShearWallArrangement(numOfBaysLongDir, numOfBaysPerpDir, coreLocation);
	double coreShearWallRatioX = 0.0;
	for (auto wallArangement : coreShearWallArrangement[0]) {
		coreShearWallRatioX += thickness * bayWidthsLongDir[wallArangement.first] / buildingArea;
	}
	double coreShearWallRatioY = 0.0;
	for (auto wallArangement : coreShearWallArrangement[1]) {
		coreShearWallRatioY += thickness * bayWidthsPerpDir[wallArangement.first] / buildingArea;
	}

	// Additional shear wall arrangement
	int chosenConfigurationXDir;
	int chosenConfigurationYDir;
	auto additionalArrangementsX = getAdditionalShearWallLocations(numOfBaysLongDir, bayWidthsLongDir, thickness, coreShearWallRatioX, buildingArea);
	auto additionalArrangementsY = getAdditionalShearWallLocations(numOfBaysPerpDir, bayWidthsPerpDir, thickness, coreShearWallRatioY, buildingArea);
	std::uniform_int_distribution<int> coreShearWallFirstConfigDist(0, additionalArrangementsX.size() + 1);
	chosenConfigurationXDir = coreShearWallFirstConfigDist(m_generator);
	if (additionalArrangementsY.empty()) {
		chosenConfigurationYDir = -1;
	}
	else {
		std::uniform_int_distribution<int> coreShearWallSecondConfigDist(0, additionalArrangementsY.size() - 1);
		chosenConfigurationYDir = coreShearWallSecondConfigDist(m_generator);
	}

	// Final shear wall arrangement
	if (0 == chosenConfigurationXDir) {
		shearWallRatioX = 0;
		shearWallRatioY = 0;
		return shearWallArrangement;
	}
	else {

		for (const auto coreX : coreShearWallArrangement[0]) {
			shearWallArrangement[0][coreX.second][coreX.first] = 1;
		}

		for (const auto coreY : coreShearWallArrangement[1]) {
			shearWallArrangement[1][coreY.second][coreY.first] = 1;
		}
	}
	
	double additionalShearWallRatioX = 0;
	double additionalShearWallRatioY = 0;
	if (chosenConfigurationXDir > 1) {

		auto additionalArrangementX = additionalArrangementsX[chosenConfigurationXDir - 2];
		
		for (const auto additionalX : additionalArrangementX) {
			shearWallArrangement[0][0][additionalX] = 1;
			shearWallArrangement[0][numOfBaysPerpDir][additionalX] = 1;
			additionalShearWallRatioX += 2 * thickness * bayWidthsLongDir[additionalX] / buildingArea;
		}

		if (chosenConfigurationYDir > -1) {

			auto additionalArrangementY = additionalArrangementsY[chosenConfigurationYDir];
			for (const auto additionalY : additionalArrangementY) {
				shearWallArrangement[1][0][additionalY] = 1;
				shearWallArrangement[1][numOfBaysLongDir][additionalY] = 1;
				additionalShearWallRatioY += 2 * thickness * bayWidthsPerpDir[additionalY] / buildingArea;
			}
		}
	}

	shearWallRatioX = coreShearWallRatioX + additionalShearWallRatioX;
	shearWallRatioY = coreShearWallRatioY + additionalShearWallRatioY;

	return shearWallArrangement;
}

std::vector<std::vector<std::pair<int, int>>> RegularPlanBuildingGenerator::getCoreShearWallArrangement(int numOfBaysLongDir, int numOfBaysPerpDir, std::pair<int, int>& coreLocation)
{
	std::vector<std::vector<std::pair<int, int>>> coreShearWallArrangement(2);
	std::uniform_int_distribution<int> coreShearWallConfigDist(1, 8);
	auto chosenConfig = coreShearWallConfigDist(m_generator);

	// Core shear walls
	if (0 == numOfBaysLongDir % 2) {

		if (0 == numOfBaysPerpDir % 2) {

			if (1 == chosenConfig) {
				coreShearWallArrangement[0].push_back(std::make_pair(numOfBaysLongDir / 2 - 1, numOfBaysPerpDir / 2));
				coreShearWallArrangement[1].push_back(std::make_pair(numOfBaysPerpDir / 2 - 1, numOfBaysLongDir / 2 - 1));
				coreShearWallArrangement[1].push_back(std::make_pair(numOfBaysPerpDir / 2 - 1, numOfBaysLongDir / 2));
				coreLocation.first = numOfBaysLongDir / 2 - 1;
				coreLocation.second = numOfBaysPerpDir / 2 - 1;
			}
			else if (2 == chosenConfig) {
				coreShearWallArrangement[0].push_back(std::make_pair(numOfBaysLongDir / 2 - 1, numOfBaysPerpDir / 2 - 1));
				coreShearWallArrangement[0].push_back(std::make_pair(numOfBaysLongDir / 2 - 1, numOfBaysPerpDir / 2));
				coreShearWallArrangement[1].push_back(std::make_pair(numOfBaysPerpDir / 2 - 1, numOfBaysLongDir / 2));
				coreLocation.first = numOfBaysLongDir / 2 - 1;
				coreLocation.second = numOfBaysPerpDir / 2 - 1;
			}
			else if (3 == chosenConfig) {
				coreShearWallArrangement[0].push_back(std::make_pair(numOfBaysLongDir / 2, numOfBaysPerpDir / 2));
				coreShearWallArrangement[1].push_back(std::make_pair(numOfBaysPerpDir / 2 - 1, numOfBaysLongDir / 2));
				coreShearWallArrangement[1].push_back(std::make_pair(numOfBaysPerpDir / 2 - 1, numOfBaysLongDir / 2 + 1));
				coreLocation.first = numOfBaysLongDir / 2;
				coreLocation.second = numOfBaysPerpDir / 2 - 1;
			}
			else if (4 == chosenConfig) {
				coreShearWallArrangement[0].push_back(std::make_pair(numOfBaysLongDir / 2, numOfBaysPerpDir / 2 - 1));
				coreShearWallArrangement[0].push_back(std::make_pair(numOfBaysLongDir / 2, numOfBaysPerpDir / 2));
				coreShearWallArrangement[1].push_back(std::make_pair(numOfBaysPerpDir / 2 - 1, numOfBaysLongDir / 2));
				coreLocation.first = numOfBaysLongDir / 2;
				coreLocation.second = numOfBaysPerpDir / 2 - 1;
			}
			else if (5 == chosenConfig) {
				coreShearWallArrangement[0].push_back(std::make_pair(numOfBaysLongDir / 2 - 1, numOfBaysPerpDir / 2));
				coreShearWallArrangement[1].push_back(std::make_pair(numOfBaysPerpDir / 2, numOfBaysLongDir / 2 - 1));
				coreShearWallArrangement[1].push_back(std::make_pair(numOfBaysPerpDir / 2, numOfBaysLongDir / 2));
				coreLocation.first = numOfBaysLongDir / 2 - 1;
				coreLocation.second = numOfBaysPerpDir / 2;
			}
			else if (6 == chosenConfig) {
				coreShearWallArrangement[0].push_back(std::make_pair(numOfBaysLongDir / 2 - 1, numOfBaysPerpDir / 2));
				coreShearWallArrangement[0].push_back(std::make_pair(numOfBaysLongDir / 2 - 1, numOfBaysPerpDir / 2 + 1));
				coreShearWallArrangement[1].push_back(std::make_pair(numOfBaysPerpDir / 2, numOfBaysLongDir / 2));
				coreLocation.first = numOfBaysLongDir / 2 - 1;
				coreLocation.second = numOfBaysPerpDir / 2;
			}
			else if (7 == chosenConfig) {
				coreShearWallArrangement[0].push_back(std::make_pair(numOfBaysLongDir / 2, numOfBaysPerpDir / 2));
				coreShearWallArrangement[1].push_back(std::make_pair(numOfBaysPerpDir / 2, numOfBaysLongDir / 2));
				coreShearWallArrangement[1].push_back(std::make_pair(numOfBaysPerpDir / 2, numOfBaysLongDir / 2 + 1));
				coreLocation.first = numOfBaysLongDir / 2;
				coreLocation.second = numOfBaysPerpDir / 2;
			}
			else if (8 == chosenConfig) {
				coreShearWallArrangement[0].push_back(std::make_pair(numOfBaysLongDir / 2, numOfBaysPerpDir / 2));
				coreShearWallArrangement[0].push_back(std::make_pair(numOfBaysLongDir / 2, numOfBaysPerpDir / 2 + 1));
				coreShearWallArrangement[1].push_back(std::make_pair(numOfBaysPerpDir / 2, numOfBaysLongDir / 2));
				coreLocation.first = numOfBaysLongDir / 2;
				coreLocation.second = numOfBaysPerpDir / 2;
			}
			else {
				// To do: do some exceptions here
			}
		}
		else {
			if (chosenConfig > 0 && chosenConfig < 5) {
				coreShearWallArrangement[0].push_back(std::make_pair(numOfBaysLongDir / 2 - 1, numOfBaysPerpDir / 2));
				coreShearWallArrangement[0].push_back(std::make_pair(numOfBaysLongDir / 2 - 1, numOfBaysPerpDir / 2 + 1));
				coreShearWallArrangement[1].push_back(std::make_pair(numOfBaysPerpDir / 2, numOfBaysLongDir / 2));
				coreLocation.first = numOfBaysLongDir / 2 - 1;
				coreLocation.second = numOfBaysPerpDir / 2;
			}
			else if (chosenConfig > 4 && chosenConfig < 9) {
				coreShearWallArrangement[0].push_back(std::make_pair(numOfBaysLongDir / 2, numOfBaysPerpDir / 2));
				coreShearWallArrangement[0].push_back(std::make_pair(numOfBaysLongDir / 2, numOfBaysPerpDir / 2 + 1));
				coreShearWallArrangement[1].push_back(std::make_pair(numOfBaysPerpDir / 2, numOfBaysLongDir / 2));
				coreLocation.first = numOfBaysLongDir / 2;
				coreLocation.second = numOfBaysPerpDir / 2;
			}
			else {
				// To do: do some exceptions here
			}
		}
	}
	else {
		if (0 == numOfBaysPerpDir % 2) {
			if (chosenConfig > 0 && chosenConfig < 5) {
				coreShearWallArrangement[0].push_back(std::make_pair(numOfBaysLongDir / 2, numOfBaysPerpDir / 2));
				coreShearWallArrangement[1].push_back(std::make_pair(numOfBaysPerpDir / 2 - 1, numOfBaysLongDir / 2));
				coreShearWallArrangement[1].push_back(std::make_pair(numOfBaysPerpDir / 2 - 1, numOfBaysLongDir / 2 + 1));
				coreLocation.first = numOfBaysLongDir / 2;
				coreLocation.second = numOfBaysPerpDir / 2 - 1;
			}
			else if (chosenConfig > 4 && chosenConfig < 9) { 
				coreShearWallArrangement[0].push_back(std::make_pair(numOfBaysLongDir / 2, numOfBaysPerpDir / 2));
				coreShearWallArrangement[1].push_back(std::make_pair(numOfBaysPerpDir / 2, numOfBaysLongDir / 2));
				coreShearWallArrangement[1].push_back(std::make_pair(numOfBaysPerpDir / 2, numOfBaysLongDir / 2 + 1));
				coreLocation.first = numOfBaysLongDir / 2;
				coreLocation.second = numOfBaysPerpDir / 2;
			}
			else {
				// To do: do some exceptions here
			}
		}
		else {
			coreLocation.first = numOfBaysLongDir / 2;
			coreLocation.second = numOfBaysPerpDir / 2;

			if (chosenConfig > 0 && chosenConfig < 3) {
				coreShearWallArrangement[0].push_back(std::make_pair(numOfBaysLongDir / 2, numOfBaysPerpDir / 2 + 1));
				coreShearWallArrangement[1].push_back(std::make_pair(numOfBaysPerpDir / 2, numOfBaysLongDir / 2));
				coreShearWallArrangement[1].push_back(std::make_pair(numOfBaysPerpDir / 2, numOfBaysLongDir / 2 + 1));
			}
			else if (chosenConfig > 2 && chosenConfig < 5) {
				coreShearWallArrangement[0].push_back(std::make_pair(numOfBaysLongDir / 2, numOfBaysPerpDir / 2));
				coreShearWallArrangement[0].push_back(std::make_pair(numOfBaysLongDir / 2, numOfBaysPerpDir / 2 + 1));
				coreShearWallArrangement[1].push_back(std::make_pair(numOfBaysPerpDir / 2, numOfBaysLongDir / 2 + 1));
			}
			else if (chosenConfig > 4 && chosenConfig < 7) {
				coreShearWallArrangement[0].push_back(std::make_pair(numOfBaysLongDir / 2, numOfBaysPerpDir / 2));
				coreShearWallArrangement[1].push_back(std::make_pair(numOfBaysPerpDir / 2, numOfBaysLongDir / 2));
				coreShearWallArrangement[1].push_back(std::make_pair(numOfBaysPerpDir / 2, numOfBaysLongDir / 2 + 1));
			}
			else if (chosenConfig > 6 && chosenConfig < 9) {
				coreShearWallArrangement[0].push_back(std::make_pair(numOfBaysLongDir / 2, numOfBaysPerpDir / 2));
				coreShearWallArrangement[0].push_back(std::make_pair(numOfBaysLongDir / 2, numOfBaysPerpDir / 2 + 1));
				coreShearWallArrangement[1].push_back(std::make_pair(numOfBaysPerpDir / 2, numOfBaysLongDir / 2));
			}
			else {
				// To do: do some exceptions here
			}
		}
	}

	return coreShearWallArrangement;
}

std::vector<std::vector<int>> RegularPlanBuildingGenerator::getAdditionalShearWallLocations(int numOfBays, std::vector<double> bayWidths, double thickness, double shearWallRatio, double buildingArea)
{
	std::vector<std::vector<int>> additionalLocations;
	std::vector<std::vector<int>> possibleLocations;

	if (3 == numOfBays) {
		possibleLocations.push_back({1});
	}
	else if (4 == numOfBays) {
		possibleLocations.push_back({1});
		possibleLocations.push_back({2});
	}
	else if (5 == numOfBays) {
		possibleLocations.push_back({2});
		possibleLocations.push_back({1, 3});
	}
	else if (6 == numOfBays) {
		possibleLocations.push_back({2});
		possibleLocations.push_back({3});
		possibleLocations.push_back({1, 4});
	}

	for (auto it = possibleLocations.begin(); it != possibleLocations.end(); it++) {

		auto totalShearWallRatio = shearWallRatio;

		for (const auto bayNumber : *it) {

			totalShearWallRatio += thickness * bayWidths[bayNumber] / buildingArea;
		}

		if (totalShearWallRatio < m_parameters.shearWallParameters.maxShearWallRatio + 1e-10) {
			additionalLocations.push_back(*it);
		}
	}

	return additionalLocations;
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