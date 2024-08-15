#include "LineElement.h"
#include "../Building.h"
#include "../../OpenSeesWrapper/OpenseesModel.h"

using namespace physicalModel;

LineElement::LineElement(int elementTag, std::vector<int> jointTags, std::shared_ptr<Section> section, LineElementFormulation lineElementFormulation)
	: m_elementTag(elementTag), m_jointTags(jointTags), m_lineElementFormulation(lineElementFormulation)
{
	m_length = calculateLength();
	m_segmentLengths.push_back(m_length);
	m_segmentRelativeLengths.push_back(1.0);
	m_sections.push_back(section);
	m_sectionModifiers.push_back(std::make_shared<SectionModifiers>());
}

double LineElement::calculateLength()
{
	utility::Vector3 pointI = physicalModel::Building::getInstance().getJoint(m_jointTags[0])->getCoords();
	utility::Vector3 pointJ = physicalModel::Building::getInstance().getJoint(m_jointTags[1])->getCoords();
	utility::Vector3 dirVec = pointJ - pointI;

	return dirVec.norm2();
}

void LineElement::setSegmentRelativeLengths(std::vector<double> segmentRelativeLengths)
{
	m_segmentRelativeLengths = segmentRelativeLengths;

	m_segmentLengths.resize(segmentRelativeLengths.size());
	for (size_t i = 0; i < m_segmentLengths.size(); ++i) {
		m_segmentLengths[i] = m_length * m_segmentRelativeLengths[i];
	}

	m_sections.resize(segmentRelativeLengths.size());
	for (size_t i = 1; i < m_sections.size(); ++i) {
		m_sections[i] = m_sections[0];
	}

	m_sectionModifiers.resize(segmentRelativeLengths.size());
	for (size_t i = 1; i < m_sectionModifiers.size(); ++i) {
		m_sectionModifiers[i] = m_sectionModifiers[0];
	}
}

void LineElement::setSection(int segmentNo, std::shared_ptr<Section> section)
{
	m_sections[segmentNo] = section;
}

void LineElement::setSectionModifiers(int segmentNo, std::shared_ptr<SectionModifiers> sectionModifier)
{
	m_sectionModifiers[segmentNo] = sectionModifier;
}

void LineElement::addAnalyticalNodeTag(int analyticalNodeTag)
{
	m_analyticalNodeTags.push_back(analyticalNodeTag);
	m_analyticalNodeCoords.push_back(opensees::OpenseesModel::getInstance().getNode(analyticalNodeTag)->getCoords());
}

void LineElement::addAnalyticalElementTag(int analyticalElementTag)
{
	m_analyticalElementTags.push_back(analyticalElementTag);
}

int LineElement::getElementTag() const
{
	return m_elementTag;
}

int LineElement::getIJointTag() const
{
	return m_jointTags[0];
}

int LineElement::getJJointTag() const
{
	return m_jointTags[1];
}

double LineElement::getMass() const
{
	auto elementMass = 0.0;

	for (int i = 0; i < m_segmentLengths.size(); ++i) {

		auto segmentMass = m_sections[i]->getMaterial()->getRho() * m_segmentLengths[i] * m_sections[i]->getA().value();
		elementMass += segmentMass;
	}

	return elementMass;
}

double LineElement::getLength() const
{
	return m_length;
}

const std::vector<double>& LineElement::getSegmentLengths() const
{
	return m_segmentLengths;
}

const std::vector<double>& LineElement::getSegmentRelativeLengths() const
{
	return m_segmentRelativeLengths;
}

const std::shared_ptr<Section> LineElement::getSection(int segmentNo) const
{
	return m_sections[segmentNo];
}

const std::shared_ptr<SectionModifiers> LineElement::getSectionModifiers(int segmentNo) const
{
	return m_sectionModifiers[segmentNo];
}

LineElementType LineElement::getLineElementType() const
{
	return m_lineElementType;
}

LineElementFormulation LineElement::getLineElementFormulation() const
{
	return m_lineElementFormulation;
}

double LineElement::getWeight() const
{
	double weight = 0.0;

	for (int i = 0; i < m_segmentLengths.size(); ++i) {
		weight += (9.81 * m_sections[i]->getMaterial()->getRho() * m_sections[i]->getA().value() * m_segmentLengths[i]);
	}

	return weight;
}

std::vector<int> LineElement::getAnalyticalNodeTags() const
{
	return m_analyticalNodeTags;
}

const std::vector<utility::Vector3>& LineElement::getAnalyticalNodeCoords() const
{
	return m_analyticalNodeCoords;
}

const std::vector<int>& LineElement::getAnalyticalElementTags() const
{
	return m_analyticalElementTags;
}

double LineElement::calculateForceX(std::string analysisTag, bool atIJoint, size_t timeStep)
{
	auto analysis = opensees::OpenseesModel::getInstance().getAnalysis(analysisTag);
	auto output = analysis->getOutput();
	auto staticOutput = std::dynamic_pointer_cast<opensees::StaticOutput>(output);

	auto forces = staticOutput->getElementForce();
	return forces[m_elementTag][0][atIJoint ? 0 : 6];
}

double LineElement::calculateForceY(std::string analysisTag, bool atIJoint, size_t timeStep)
{
	auto analysis = opensees::OpenseesModel::getInstance().getAnalysis(analysisTag);
	auto output = analysis->getOutput();
	auto staticOutput = std::dynamic_pointer_cast<opensees::StaticOutput>(output);

	auto forces = staticOutput->getElementForce();
	return forces[m_elementTag][0][atIJoint ? 1 : 7];
}

double LineElement::calculateForceZ(std::string analysisTag, bool atIJoint, size_t timeStep)
{
	auto analysis = opensees::OpenseesModel::getInstance().getAnalysis(analysisTag);
	auto output = analysis->getOutput();
	auto staticOutput = std::dynamic_pointer_cast<opensees::StaticOutput>(output);

	auto forces = staticOutput->getElementForce();
	return forces[m_elementTag][0][atIJoint ? 2 : 8];
}

double LineElement::calculateMomentXX(std::string analysisTag, bool atIJoint, size_t timeStep)
{
	auto analysis = opensees::OpenseesModel::getInstance().getAnalysis(analysisTag);
	auto output = analysis->getOutput();
	auto staticOutput = std::dynamic_pointer_cast<opensees::StaticOutput>(output);

	auto forces = staticOutput->getElementForce();
	return forces[m_elementTag][0][atIJoint ? 3 : 9];
}

double LineElement::calculateMomentYY(std::string analysisTag, bool atIJoint, size_t timeStep)
{
	auto analysis = opensees::OpenseesModel::getInstance().getAnalysis(analysisTag);
	auto output = analysis->getOutput();
	auto staticOutput = std::dynamic_pointer_cast<opensees::StaticOutput>(output);

	auto forces = staticOutput->getElementForce();
	return forces[m_elementTag][0][atIJoint ? 4 : 10];
}

double LineElement::calculateMomentZZ(std::string analysisTag, bool atIJoint, size_t timeStep)
{
	auto analysis = opensees::OpenseesModel::getInstance().getAnalysis(analysisTag);
	auto output = analysis->getOutput();
	auto staticOutput = std::dynamic_pointer_cast<opensees::StaticOutput>(output);

	auto forces = staticOutput->getElementForce();
	return forces[m_elementTag][0][atIJoint ? 5 : 11];
}

double LineElement::calculateDR(std::string analysisTag, bool fromIJoint, size_t timeStep)
{
	auto analysis = opensees::OpenseesModel::getInstance().getAnalysis(analysisTag);
	auto output = analysis->getOutput();

	auto displacements = output->getNodeDisplacement();
	auto dispI = displacements[m_jointTags[0]][0][0];
	auto dispJ = displacements[m_jointTags[1]][0][0];

	utility::Vector3 pointI = physicalModel::Building::getInstance().getJoint(m_jointTags[0])->getCoords();
	utility::Vector3 pointJ = physicalModel::Building::getInstance().getJoint(m_jointTags[1])->getCoords();

	auto DR = (dispI - dispJ) / std::abs(pointI.z - pointJ.z);

	return fromIJoint ? DR : -1 * DR;
}

double LineElement::calculateChordRotation(std::string analysisTag, bool fromIJoint, size_t timeStep)
{
	return 1;
}

double LineElement::calculateDisplacement(std::string analysisTag, size_t segmentNode, size_t dof, size_t timeStep)
{
	return 1;
}
