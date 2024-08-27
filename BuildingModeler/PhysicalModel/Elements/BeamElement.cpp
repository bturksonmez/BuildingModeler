#include "BeamElement.h"
#include "../Building.h"
#include "../../OpenSeesWrapper/OpenseesModel.h"

using namespace physicalModel;

BeamElement::BeamElement(int elementTag, std::vector<int> jointTags, std::shared_ptr<Section> section, LineElementFormulation lineElementFormulation)
	: LineElement(elementTag, jointTags, section, lineElementFormulation)
{
	m_lineElementType = LineElementType::BEAM;
}

double BeamElement::calculateChordRotation(std::string analysisTag, size_t dofRot, bool fromIJoint, size_t timeStep)
{
	auto analysis = opensees::OpenseesModel::getInstance().getAnalysis(analysisTag);
	auto output = analysis->getOutput();

	auto displacements = output->getNodeDisplacement();
	auto chordRotation = displacements[fromIJoint ? m_jointTags[0] : m_jointTags[1]][timeStep][dofRot];

	return chordRotation;
}