#include "Section.h"

using namespace std;
using namespace opensees;

int Section::getSectionTag() const
{
	return m_sectionTag;
}

SectionType Section::getSectionType() const
{
	return m_sectionType;
}

const shared_ptr<Material> Section::getMaterial() const
{
	return m_material;
}

double Section::getA() const
{
	return m_A;
}

double Section::getIyy() const
{
	return m_Iyy;
}

double Section::getIzz() const
{
	return m_Izz;
}

double Section::getJ() const
{
	return m_J;
}