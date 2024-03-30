#include <cmath>

#include "AreaElement.h"
#include "../Building.h"
#include "../../Utilities/VectorUtilities.h"
#include "../../OpenSeesWrapper/OpenseesModel.h"

using namespace physicalModel;

AreaElement::AreaElement(int elementTag, std::vector<int> jointTags, std::shared_ptr<Section> section, AreaElementFormulation areaElementFormulation)
	: m_elementTag(elementTag), m_jointTags(jointTags), m_section(section), m_areaElementFormulation(areaElementFormulation)
{
	m_meshable = false;
	m_surroundingLineElementTags.resize(4, -1);
	m_area = calculateArea();

	if (auto tempSection = std::dynamic_pointer_cast<ElasticSection2D>(section)) {
		m_thickness = tempSection->getThickness();
	}
}

double AreaElement::calculateArea()
{
	utility::Vector3 pointI = physicalModel::Building::getInstance().getJoint(m_jointTags[0])->getCoords();
	utility::Vector3 pointJ = physicalModel::Building::getInstance().getJoint(m_jointTags[1])->getCoords();
	utility::Vector3 pointK = physicalModel::Building::getInstance().getJoint(m_jointTags[2])->getCoords();
	utility::Vector3 pointL = physicalModel::Building::getInstance().getJoint(m_jointTags[3])->getCoords();

	auto angleA = utility::VectorUtilities::getAngleBtw(pointJ - pointI, pointL - pointI);
	auto angleC = utility::VectorUtilities::getAngleBtw(pointJ - pointK, pointL - pointK);
	
	auto theta = angleA + angleC;
	auto a = (pointJ - pointI).norm2();
	auto b = (pointK - pointJ).norm2();
	auto c = (pointL - pointK).norm2();
	auto d = (pointI - pointL).norm2();
	auto s = (a + b + c + d) / 2.0;

	return std::sqrt((s - a) * (s - b) * (s - c) * (s - d) - a * b * c * d * std::pow(std::cos(theta / 2.0), 2));
}

void AreaElement::addSurroundingLineElement(int index, int surroundingLineElementTag)
{
	m_surroundingLineElementTags[index] = surroundingLineElementTag;
}

void AreaElement::mesh(bool meshable, int n1, int n2)
{
	m_meshable = meshable;
	m_n1 = n1;
	m_n2 = n2;
}

void AreaElement::setMeshable(bool meshable)
{
	m_meshable = meshable;
}

void AreaElement::addAnalyticalNodeTags(std::vector<int> analyticalNodeTags)
{
	m_analyticalNodeTags.push_back(analyticalNodeTags);

	std::vector<utility::Vector3> analyticalNodeCoords;
	for (const auto& tag : analyticalNodeTags) {
		analyticalNodeCoords.push_back(opensees::OpenseesModel::getInstance().getNode(tag)->getCoords());
	}
	m_analyticalNodeCoords.push_back(analyticalNodeCoords);
}

void AreaElement::addAnalyticalElementTag(int analyticalElementTag)
{
	m_analyticalElementTags.push_back(analyticalElementTag);
}

int AreaElement::getElementTag() const
{
	return m_elementTag;
}

int AreaElement::getIJointTag() const
{
	return m_jointTags[0];
}

int AreaElement::getJJointTag() const
{
	return m_jointTags[1];
}

int AreaElement::getKJointTag() const
{
	return m_jointTags[2];
}

int AreaElement::getLJointTag() const
{
	return m_jointTags[3];
}

double AreaElement::getMass() const
{
	auto elementMass = m_section->getMaterial()->getRho() * m_thickness * m_area;

	return elementMass;
}

double AreaElement::getArea() const
{
	return m_area;
}

double AreaElement::getThickness() const
{
	return m_thickness;
}

bool AreaElement::isMeshable() const
{
	return m_meshable;
}

const std::vector<int>& AreaElement::getJointTags() const
{
	return m_jointTags;
}

std::pair<int, int> AreaElement::getMeshDivisions() const
{
	return { m_n1, m_n2 };
}

const std::vector<int>& AreaElement::getSurroundingLineElementTags()
{
	return m_surroundingLineElementTags;
}

const std::shared_ptr<Section> AreaElement::getSection() const
{
	return m_section;
}

AreaElementType AreaElement::getAreaElementType() const
{
	return m_areaElementType;
}

AreaElementFormulation AreaElement::getAreaElementFormulation() const
{
	return m_areaElementFormulation;
}

double AreaElement::getWeight() const
{
	double weight = 9.81 * m_area * m_thickness * m_section->getMaterial()->getRho();

	return weight;
}

std::pair<double, double> AreaElement::getTributaryLineLength() const
{
	auto coordA = Building::getInstance().getJoint(m_jointTags[0])->getCoords();
	auto coordB = Building::getInstance().getJoint(m_jointTags[1])->getCoords();
	auto coordC = Building::getInstance().getJoint(m_jointTags[2])->getCoords();

	auto l1 = (coordB - coordA).norm2();
	auto l2 = (coordC - coordB).norm2();

	auto lx = std::min(l1, l2);
	auto ly = std::min(l1, l2);

	auto wx = lx / 4.0;
	auto wy = (2.0 * ly - lx) * (lx / 2.0) / ly / 2.0;

	if (l1 > l2) {
		return std::make_pair(wy, wx);
	}

	return std::make_pair(wx, wy);
}

const std::vector<std::vector<int>>& AreaElement::getAnalyticalNodeTags() const
{
	return m_analyticalNodeTags;
}

const std::vector<std::vector<utility::Vector3>>& AreaElement::getAnalyticalNodeCoords() const
{
	return m_analyticalNodeCoords;
}

const std::vector<int>& AreaElement::getAnalyticalElementTags() const
{
	return m_analyticalElementTags;
}
