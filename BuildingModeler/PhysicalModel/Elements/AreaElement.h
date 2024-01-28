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
		bool m_meshable;
		int m_n1; // number of division along IJ path for meshing
		int m_n2; // number of division along JK path for meshing
		std::vector<int> m_jointTags;
		std::vector<int> m_surroundingLineElementTags;
		std::shared_ptr<Section> m_section;
		AreaElementType m_areaElementType;
		AreaElementFormulation m_areaElementFormulation;

		AreaElement(int elementTag, std::vector<int> jointTags, std::shared_ptr<Section> section, AreaElementFormulation areaElementFormulation);
	public:
		AreaElement() = delete;
		AreaElement(AreaElement&& other) = default;
		AreaElement& operator=(AreaElement&& other) = default;
		virtual ~AreaElement() {}

		void addSurroundingLineElement(int index, int surroundingLineElementTag);
		void mesh(bool meshable, int n1, int n2);

		int getElementTag() const;
		int getIJointTag() const;
		int getJJointTag() const;
		int getKJointTag() const;
		int getLJointTag() const;
		const std::vector<int> getJointTags() const;
		bool isMeshable() const;
		std::pair<int, int> getMeshDivisions() const;
		std::vector<int> getSurroundingLineElementTags();
		const std::shared_ptr<Section> getSection() const;
		AreaElementType getAreaElementType() const;
		AreaElementFormulation getAreaElementFormulation() const;
	};
}
