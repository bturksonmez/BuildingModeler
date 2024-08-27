#include "ColumnElement.h"
#include "../Building.h"
#include "../../OpenSeesWrapper/OpenseesModel.h"

using namespace physicalModel;

ColumnElement::ColumnElement(int elementTag, std::vector<int> jointTags, std::shared_ptr<Section> section, LineElementFormulation lineElementFormulation)
	: LineElement(elementTag, jointTags, section, lineElementFormulation)
{
	m_lineElementType = LineElementType::COLUMN;
}

double ColumnElement::calculateChordRotation(std::string analysisTag, size_t dofRot, bool fromIJoint, size_t timeStep)
{
	auto analysis = opensees::OpenseesModel::getInstance().getAnalysis(analysisTag);
	auto output = analysis->getOutput();

	size_t dofTrans = dofRot == 4 ? 0 : 1;

	auto displacements = output->getNodeDisplacement();

	auto delta = displacements[m_jointTags[1]][timeStep][dofTrans] - displacements[m_jointTags[0]][timeStep][dofTrans];
	delta = dofRot == 4 ? delta : -delta;
	auto theta = fromIJoint ? displacements[m_jointTags[0]][timeStep][dofRot] : displacements[m_jointTags[0]][timeStep][dofRot];

	return delta / getLength() - theta;
}