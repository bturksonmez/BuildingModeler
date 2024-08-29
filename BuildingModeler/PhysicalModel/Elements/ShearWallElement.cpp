#include "ShearWallElement.h"
#include "../Building.h"
#include "../../OpenSeesWrapper/OpenseesModel.h"

using namespace physicalModel;

ShearWallElement::ShearWallElement(int elementTag, std::vector<int> jointTags, std::shared_ptr<Section> section, AreaElementFormulation areaElementFormulation)
	: AreaElement(elementTag, jointTags, section, areaElementFormulation)
{
	m_areaElementType = AreaElementType::SHEARWALL;
}

double ShearWallElement::calculateAxialForceGlobalZ(std::string analysisTag, size_t timeStep, bool atBottom)
{
	auto analysis = opensees::OpenseesModel::getInstance().getAnalysis(analysisTag);
	auto output = analysis->getOutput();
	auto staticOutput = std::dynamic_pointer_cast<opensees::StaticOutput>(output);

	auto forces = staticOutput->getElementForce();
	auto elements = atBottom ? getBottomAnalyticalElements() : getTopAnalyticalElements();

	auto axialForce = forces[elements[0]][timeStep][atBottom ? 2 : 20];

	for (auto element : elements) {
		axialForce += forces[element][timeStep][atBottom ? 8 : 14];
	}

	return axialForce;
}

double ShearWallElement::calculateShearForceGlobalX(std::string analysisTag, size_t timeStep, bool atBottom)
{
	auto analysis = opensees::OpenseesModel::getInstance().getAnalysis(analysisTag);
	auto output = analysis->getOutput();
	auto staticOutput = std::dynamic_pointer_cast<opensees::StaticOutput>(output);

	auto forces = staticOutput->getElementForce();
	auto elements = atBottom ? getBottomAnalyticalElements() : getTopAnalyticalElements();

	auto shearForce = forces[elements[0]][timeStep][atBottom ? 0 : 18];

	for (auto element : elements) {
		shearForce += forces[element][timeStep][atBottom ? 6 : 12];
	}

	return shearForce;
}

double ShearWallElement::calculateShearForceGlobalY(std::string analysisTag, size_t timeStep, bool atBottom)
{
	auto analysis = opensees::OpenseesModel::getInstance().getAnalysis(analysisTag);
	auto output = analysis->getOutput();
	auto staticOutput = std::dynamic_pointer_cast<opensees::StaticOutput>(output);

	auto forces = staticOutput->getElementForce();
	auto elements = atBottom ? getBottomAnalyticalElements() : getTopAnalyticalElements();

	auto shearForce = forces[elements[0]][timeStep][atBottom ? 1 : 19];

	for (auto element : elements) {
		shearForce += forces[element][timeStep][atBottom ? 7 : 13];
	}

	return shearForce;
}

double ShearWallElement::calculateMomentGlobalXX(std::string analysisTag, size_t timeStep, bool atBottom)
{
	auto analysis = opensees::OpenseesModel::getInstance().getAnalysis(analysisTag);
	auto output = analysis->getOutput();
	auto staticOutput = std::dynamic_pointer_cast<opensees::StaticOutput>(output);

	auto forces = staticOutput->getElementForce();
	auto elements = atBottom ? getBottomAnalyticalElements() : getTopAnalyticalElements();

	auto moment = forces[elements[0]][timeStep][atBottom ? 3 : 21];

	for (auto element : elements) {
		moment += forces[element][timeStep][atBottom ? 9 : 15];
	}

	return moment;
}

double ShearWallElement::calculateMomentGlobalYY(std::string analysisTag, size_t timeStep, bool atBottom)
{
	auto analysis = opensees::OpenseesModel::getInstance().getAnalysis(analysisTag);
	auto output = analysis->getOutput();
	auto staticOutput = std::dynamic_pointer_cast<opensees::StaticOutput>(output);

	auto forces = staticOutput->getElementForce();
	auto elements = atBottom ? getBottomAnalyticalElements() : getTopAnalyticalElements();

	auto moment = forces[elements[0]][timeStep][atBottom ? 4 : 22];

	for (auto element : elements) {
		moment += forces[element][timeStep][atBottom ? 10 : 16];
	}

	return moment;
}

double ShearWallElement::calculateMomentGlobalZZ(std::string analysisTag, size_t timeStep, bool atBottom)
{
	auto analysis = opensees::OpenseesModel::getInstance().getAnalysis(analysisTag);
	auto output = analysis->getOutput();
	auto staticOutput = std::dynamic_pointer_cast<opensees::StaticOutput>(output);

	auto forces = staticOutput->getElementForce();
	auto elements = atBottom ? getBottomAnalyticalElements() : getTopAnalyticalElements();

	auto moment = forces[elements[0]][timeStep][atBottom ? 5 : 23];

	for (auto element : elements) {
		moment += forces[element][timeStep][atBottom ? 11 : 17];
	}

	return moment;
}

double ShearWallElement::calculateDR(std::string analysisTag, size_t dof, size_t timeStep)
{
	auto analysis = opensees::OpenseesModel::getInstance().getAnalysis(analysisTag);
	auto output = analysis->getOutput();

	auto displacements = output->getNodeDisplacement();
	auto dispI = (displacements[m_jointTags[0]][timeStep][dof] + displacements[m_jointTags[1]][timeStep][dof]) / 2.0;
	auto dispJ = (displacements[m_jointTags[2]][timeStep][dof] + displacements[m_jointTags[3]][timeStep][dof]) / 2.0;

	auto bottomZ = physicalModel::Building::getInstance().getJoint(m_jointTags[0])->getCoords().z;
	auto topZ = physicalModel::Building::getInstance().getJoint(m_jointTags[3])->getCoords().z;
	auto length = topZ - bottomZ;

	auto DR = std::abs(dispI - dispJ) / length;

	return DR;
}

double ShearWallElement::calculateChordRotation(std::string analysisTag, size_t dofRot, size_t timeStep, bool fromBottom)
{
	auto analysis = opensees::OpenseesModel::getInstance().getAnalysis(analysisTag);
	auto output = analysis->getOutput();

	size_t dofTrans = dofRot == 4 ? 0 : 1;

	auto displacements = output->getNodeDisplacement();

	auto dispI = (displacements[m_jointTags[0]][timeStep][dofTrans] + displacements[m_jointTags[1]][timeStep][dofTrans]) / 2.0;
	auto dispJ = (displacements[m_jointTags[2]][timeStep][dofTrans] + displacements[m_jointTags[3]][timeStep][dofTrans]) / 2.0;
	auto delta = dispJ - dispI;
	delta = dofRot == 4 ? delta : -delta;

	return 1;
}

const std::vector<int> ShearWallElement::getBottomAnalyticalElements() const
{
	std::vector<int> bottomElements;

	int n1 = 1;
	if (m_meshable) {

		if (m_surroundingLineElementTags[0] != -1) {
			n1 = physicalModel::Building::getInstance().getLineElement(m_surroundingLineElementTags[0])->getSegmentLengths().size();
		}
		else {
			n1 = m_n1;
		}
	}

	for (int i = 0; i < n1; ++i) {
		bottomElements.push_back(m_analyticalElementTags[i]);
	}
	
	return bottomElements;
}

const std::vector<int> ShearWallElement::getTopAnalyticalElements() const
{
	std::vector<int> topElements;

	int n1 = 1;
	if (m_meshable) {

		if (m_surroundingLineElementTags[0] != -1) {
			n1 = physicalModel::Building::getInstance().getLineElement(m_surroundingLineElementTags[0])->getSegmentLengths().size();
		}
		else {
			n1 = m_n1;
		}
	}

	for (int i = m_analyticalElementTags.size() - n1; i < m_analyticalElementTags.size(); ++i) {
		topElements.push_back(m_analyticalElementTags[i]);
	}

	return topElements;
}