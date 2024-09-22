#include "Section.h"

using namespace physicalModel;

Section::~Section()
{
	if (m_shape != std::nullopt) {
		delete m_shape.value();
	}
}

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
	if (m_shape != std::nullopt) {
		return m_shape.value()->getA();
	}

	return std::nullopt;
}

std::optional<double> Section::getIyy() const
{
	if (m_shape != std::nullopt) {
		return m_shape.value()->getIyy();
	}

	return std::nullopt;
}

std::optional<double> Section::getIzz() const
{
	if (m_shape != std::nullopt) {
		return m_shape.value()->getIzz();
	}

	return std::nullopt;
}

std::optional<double> Section::getJ() const
{
	if (m_shape != std::nullopt) {
		return m_shape.value()->getJ();
	}

	return std::nullopt;
}