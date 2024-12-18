#include "ModalAnalysisGenerator.h"
#include "../BuildingModelerAPI.h"

using namespace loadingGenerator;

ModalAnalysisGenerator::ModalAnalysisGenerator()
{
}

void ModalAnalysisGenerator::load(json& buildingInfo)
{
	int ns = buildingInfo["numberOfStoreys"];
	int numOfModes = ns / 4 + 3;

	api::addModalLoadCase("modal", numOfModes);
	api::setLoadCaseActive("modal", true);
}