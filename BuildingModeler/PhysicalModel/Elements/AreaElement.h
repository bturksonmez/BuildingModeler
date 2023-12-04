#pragma once

#include <iostream>
#include <vector>
#include <memory>

#include "../Sections/Section.h"

namespace physicalModel
{
	enum class AreaElementType
	{
		SLAB,
		SHEARWALL
	};

	enum class AreaElementFormulation
	{
		LINEAR,
		LINEAR_MODIFIED_SHEAR,
		NONLINEAR_GEOMETRIC,
	};

	class AreaElement
	{
	protected:
		int m_elementTag;
		std::vector<int> m_jointTags;
		std::shared_ptr<Section> m_section;
		AreaElementType m_areaElementType;
		AreaElementFormulation m_areaElementFormulation;

		AreaElement(int elementTag, std::vector<int> jointTags, std::shared_ptr<Section> sections, AreaElementFormulation areaElementFormulation);
	public:
		AreaElement() = delete;
		~AreaElement() {}

		int getElementTag() const;
		int getIJointTag() const;
		int getJJointTag() const;
		int getKJointTag() const;
		int getLJointTag() const;
		AreaElementType getAreaElementType() const;
		AreaElementFormulation getAreaElementFormulation() const;
	};
}
