#include "ModalAnalysisGenerator.h"
#include "../BuildingModelerAPI.h"

using namespace loadingGenerator;

ModalAnalysisGenerator::ModalAnalysisGenerator()
{
}

void ModalAnalysisGenerator::load(json& buildingInfo)
{
	int ns = buildingInfo["numberOfStoreys"];
	int numOfModes = std::min(2 * ns, 12);

	api::addModalLoadCase("modal", numOfModes);
	api::setLoadCaseActive("modal", true);
}