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

	auto axialForce = 0.0;

	for (auto element : elements) {
		axialForce += forces[element][timeStep][atBottom ? 2 : 20];
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

	auto shearForce = 0.0;

	for (auto element : elements) {
		shearForce += forces[element][timeStep][atBottom ? 0 : 18];
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

	auto shearForce = 0.0;

	for (auto element : elements) {
		shearForce += forces[element][timeStep][atBottom ? 1 : 19];
		shearForce += forces[element][timeStep][atBottom ? 7 : 13];
	}

	return shearForce;
}

double ShearWallElement::calculateMomentGlobalXX(std::string analysisTag, size_t timeStep, bool atBottom)
{
	if (!onXdirection()) {
		return calculateMomentInPlaneXX(analysisTag, timeStep, atBottom);
	}

	auto analysis = opensees::OpenseesModel::getInstance().getAnalysis(analysisTag);
	auto output = analysis->getOutput();
	auto staticOutput = std::dynamic_pointer_cast<opensees::StaticOutput>(output);

	auto forces = staticOutput->getElementForce();
	auto elements = atBottom ? getBottomAnalyticalElements() : getTopAnalyticalElements();

	auto moment = 0.0;

	for (auto element : elements) {
		moment += forces[element][timeStep][atBottom ? 3 : 21];
		moment += forces[element][timeStep][atBottom ? 9 : 15];
	}

	return moment;
}

double ShearWallElement::calculateMomentGlobalYY(std::string analysisTag, size_t timeStep, bool atBottom)
{
	if (onXdirection()) {
		return calculateMomentInPlaneYY(analysisTag, timeStep, atBottom);
	}

	auto analysis = opensees::OpenseesModel::getInstance().getAnalysis(analysisTag);
	auto output = analysis->getOutput();
	auto staticOutput = std::dynamic_pointer_cast<opensees::StaticOutput>(output);

	auto forces = staticOutput->getElementForce();
	auto elements = atBottom ? getBottomAnalyticalElements() : getTopAnalyticalElements();

	auto moment = 0.0;

	for (auto element : elements) {
		moment += forces[element][timeStep][atBottom ? 4 : 22];
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

	auto moment = 0.0;

	for (auto element : elements) {
		moment += forces[element][timeStep][atBottom ? 5 : 23];
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

	if ((onXdirection() && dofTrans == 1) || (!onXdirection() && dofTrans == 0)) {
		return 0.0;
	}

	auto displacements = output->getNodeDisplacement();

	auto dispI = (displacements[m_jointTags[0]][timeStep][dofTrans] + displacements[m_jointTags[1]][timeStep][dofTrans]) / 2.0;
	auto dispJ = (displacements[m_jointTags[2]][timeStep][dofTrans] + displacements[m_jointTags[3]][timeStep][dofTrans]) / 2.0;
	auto delta = dispJ - dispI;
	delta = dofRot == 4 ? delta : -delta;

	auto bottomZ = physicalModel::Building::getInstance().getJoint(m_jointTags[0])->getCoords().z;
	auto topZ = physicalModel::Building::getInstance().getJoint(m_jointTags[3])->getCoords().z;
	auto length = topZ - bottomZ;

	auto theta = calculateRotation(analysisTag, dofTrans, timeStep, fromBottom);
	auto chordRotation = std::abs(delta / length - theta);

	return chordRotation;
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

bool ShearWallElement::onXdirection() const
{
	// To do: this function assumes that shear wall is either on X or Y direction
	auto jointI = physicalModel::Building::getInstance().getJoint(m_jointTags[0])->getCoords();
	auto jointJ = physicalModel::Building::getInstance().getJoint(m_jointTags[1])->getCoords();

	if (jointI.y > jointJ.y - 1e-7 && jointI.y < jointJ.y + 1e-7) {
		return true;
	}

	return false;
}

double ShearWallElement::calculateMomentInPlaneXX(std::string analysisTag, size_t timeStep, bool atBottom)
{
	auto analysis = opensees::OpenseesModel::getInstance().getAnalysis(analysisTag);
	auto output = analysis->getOutput();
	auto staticOutput = std::dynamic_pointer_cast<opensees::StaticOutput>(output);

	auto forces = staticOutput->getElementForce();
	auto elements = atBottom ? getBottomAnalyticalElements() : getTopAnalyticalElements();

	auto originCoord = (opensees::OpenseesModel::getInstance().getNode(opensees::OpenseesModel::getInstance().getQuadrilateralElement(elements[0])->getNodeTags()[atBottom ? 0 : 3])->getCoords().y +
					  opensees::OpenseesModel::getInstance().getNode(opensees::OpenseesModel::getInstance().getQuadrilateralElement(elements[elements.size() - 1])->getNodeTags()[atBottom ? 1 : 2])->getCoords().y) / 2.0;
	auto moment = 0.0;

	for (auto element : elements) {
		auto currCoord1 = opensees::OpenseesModel::getInstance().getNode(opensees::OpenseesModel::getInstance().getQuadrilateralElement(element)->getNodeTags()[atBottom ? 0 : 3])->getCoords().y;
		auto currCoord2 = opensees::OpenseesModel::getInstance().getNode(opensees::OpenseesModel::getInstance().getQuadrilateralElement(element)->getNodeTags()[atBottom ? 1 : 2])->getCoords().y;
		moment += forces[element][timeStep][atBottom ? 2 : 20] * (currCoord1 - originCoord);
		moment += forces[element][timeStep][atBottom ? 8 : 14] * (currCoord2 - originCoord);
	}

	return moment;
}

double ShearWallElement::calculateMomentInPlaneYY(std::string analysisTag, size_t timeStep, bool atBottom)
{
	auto analysis = opensees::OpenseesModel::getInstance().getAnalysis(analysisTag);
	auto output = analysis->getOutput();
	auto staticOutput = std::dynamic_pointer_cast<opensees::StaticOutput>(output);

	auto forces = staticOutput->getElementForce();
	auto elements = atBottom ? getBottomAnalyticalElements() : getTopAnalyticalElements();

	auto originCoord = (opensees::OpenseesModel::getInstance().getNode(opensees::OpenseesModel::getInstance().getQuadrilateralElement(elements[0])->getNodeTags()[atBottom ? 0 : 3])->getCoords().x +
		opensees::OpenseesModel::getInstance().getNode(opensees::OpenseesModel::getInstance().getQuadrilateralElement(elements[elements.size() - 1])->getNodeTags()[atBottom ? 1 : 2])->getCoords().x) / 2.0;
	auto moment = 0.0;

	for (auto element : elements) {
		auto currCoord1 = opensees::OpenseesModel::getInstance().getNode(opensees::OpenseesModel::getInstance().getQuadrilateralElement(element)->getNodeTags()[atBottom ? 0 : 3])->getCoords().x;
		auto currCoord2 = opensees::OpenseesModel::getInstance().getNode(opensees::OpenseesModel::getInstance().getQuadrilateralElement(element)->getNodeTags()[atBottom ? 1 : 2])->getCoords().x;
		moment += forces[element][timeStep][atBottom ? 2 : 20] * (currCoord1 - originCoord);
		moment += forces[element][timeStep][atBottom ? 8 : 14] * (currCoord2 - originCoord);
	}

	return -moment;
}

double ShearWallElement::calculateRotation(std::string analysisTag, size_t dofTrans, size_t timeStep, bool fromBottom)
{
	auto start = dofTrans == 0 ? physicalModel::Building::getInstance().getJoint(m_jointTags[0])->getCoords().x : physicalModel::Building::getInstance().getJoint(m_jointTags[0])->getCoords().y;
	auto end = dofTrans == 0 ? physicalModel::Building::getInstance().getJoint(m_jointTags[1])->getCoords().x : physicalModel::Building::getInstance().getJoint(m_jointTags[1])->getCoords().y;
	auto length = std::abs(end - start);

	auto analysis = opensees::OpenseesModel::getInstance().getAnalysis(analysisTag);
	auto output = analysis->getOutput();
	auto displacements = output->getNodeDisplacement();

	auto dispI = fromBottom ? displacements[m_jointTags[0]][timeStep][2] : displacements[m_jointTags[3]][timeStep][2];
	auto dispJ = fromBottom ? displacements[m_jointTags[1]][timeStep][2] : displacements[m_jointTags[2]][timeStep][2];
	auto theta = (dispJ - dispI) / length;

	return -theta;
}