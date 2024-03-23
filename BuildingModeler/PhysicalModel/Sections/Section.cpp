#include "Section.h"

using namespace physicalModel;

int Section::getSectionTag() const
{
	return m_sectionTag;
}

SectionType Section::getSectionType() const
{
	return m_sectionType;
}

const std::shared_ptr<Material> Section::getMaterial() const
{
	return m_material;
}

std::optional<double> Section::getA() const
{
	return m_A;
}

std::optional<double> Section::getIyy() const
{
	return m_Iyy;
}

std::optional<double> Section::getIzz() const
{
	return m_Izz;
}

std::optional<double> Section::getJ() const
{
	return m_J;
}