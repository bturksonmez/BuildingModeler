#include <iostream>
#include <fstream>
#include "BuildingGenerator/RegularPlanBuildingGenerator.h"
#include "Utilities/VectorUtilities.h"
#include "BuildingModelerAPI.h"

using namespace std;
using namespace buildingGenerator;

int main()
{
	Parameters params;
	params.geometricParameters.minNumberOfBays = 6;
	params.geometricParameters.maxNumberOfBays = 6;
	params.geometricParameters.minBayWidth = 5.0;
	params.geometricParameters.maxBayWidth = 6.0;
	params.geometricParameters.minNumberOfStoreys = 10;
	params.geometricParameters.maxNumberOfStoreys = 10;
	params.geometricParameters.minFirstStoreyHeight = 2.8;
	params.geometricParameters.maxFirstStoreyHeight = 4.5;
	params.geometricParameters.minStoreyHeight = 2.8;
	params.geometricParameters.maxStoreyHeight = 3.5;
	params.columnParameters.maxAspectRatioForColumns = 1.5;
	params.columnParameters.maxAreaRatioInnerToOuterColumns = 2.0;
	params.columnParameters.minEquivalentSquareColumnWidth = 0.35;
	params.columnParameters.maxEquivalentSquareColumnWidth = 0.60;
	params.columnParameters.minColumnCrackedSectionModifier = 0.5;
	params.columnParameters.maxColumnCrackedSectionModifier = 0.7;
	params.beamParameters.minBeamWidth = 0.25;
	params.beamParameters.maxBeamWidth = 0.35;
	params.beamParameters.minBeamDepth = 0.45;
	params.beamParameters.maxBeamDepth = 0.7;
	params.beamParameters.minBeamCrackedSectionModifier = 0.25;
	params.beamParameters.maxBeamCrackedSectionModifier = 0.4;
	params.shearWallParameters.includeShearWallInXDir = true;
	params.shearWallParameters.includeShearWallInYDir = false;
	params.shearWallParameters.maxShearWallRatio = 0.03;
	params.shearWallParameters.minShearWallThickness = 0.25;
	params.shearWallParameters.maxShearWallThickness = 0.3;
	params.shearWallParameters.minShearWallCrackedSectionModifier = 0.5;
	params.shearWallParameters.maxShearWallCrackedSectionModifier = 0.7;
	params.slabParameters.minSlabThickness = 0.15;
	params.slabParameters.maxSlabThickness = 0.20;
	params.minConcreteYoungsModulus = 30000000;
	params.maxConcreteYoungsModulus = 40000000;
	params.meshInfo.meshSensitivity = 1.5;
	params.meshInfo.meshColumn = false;
	params.meshInfo.meshSlabBeam = true;
	params.modelingPreferences.includeMassFromMembers = true;
	params.modelingPreferences.gravityThroughLineElements = false;
	params.modelingPreferences.minLiveLoadMassContribution = 0.3;
	params.modelingPreferences.maxLiveLoadMassContribution = 1.0;
	params.gravityLoading.minDeadLoadFactor = 0.9;
	params.gravityLoading.maxDeadLoadFactor = 1.4;
	params.gravityLoading.minLiveLoadPerArea = 2.0;
	params.gravityLoading.maxLiveLoadPerArea = 3.0;
	params.gravityLoading.minLiveLoadFactor = 0.3;
	params.gravityLoading.maxLiveLoadFactor = 1.6;

	auto generator = IBuildingGenerator::create<RegularPlanBuildingGenerator>(params);
	json buildingInfo = generator->generate();
	std::ofstream file("building_info.json");
	file << buildingInfo.dump(4);  // The argument 4 specifies indentation for pretty-printing
	file.close();

	buildingModeler::BuildingModelerAPI::createAnalyticalModel();
	buildingModeler::BuildingModelerAPI::createModelAndLoadingFiles();
	buildingModeler::BuildingModelerAPI::analyze();

	


	//for (int i = 0; i < 1000; ++i) {
	//
	//	auto generator = IBuildingGenerator::create<RegularPlanBuildingGenerator>(params);
	//	json buildingInfo = generator->generate();
	//	buildingModeler::BuildingModelerAPI::createAnalyticalModel();
	//	buildingModeler::BuildingModelerAPI::createModelAndLoadingFiles();
	//
	//	//std::ofstream file("building_info" + std::to_string(i) + ".json");
	//	//file << buildingInfo.dump(4);  // The argument 4 specifies indentation for pretty-printing
	//	//file.close();
	//}

	return 0;
}

