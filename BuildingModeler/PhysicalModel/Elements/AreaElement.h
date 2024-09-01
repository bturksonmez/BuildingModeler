#pragma once

#include <iostream>
#include <vector>
#include <memory>

#include "../Sections/Section.h"
#include "../../Utilities/Vector3.h"

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
		double m_area;
		double m_thickness;
		std::vector<int> m_surroundingLineElementTags;
		std::vector<int> m_surroundingShearWallTags;
		std::shared_ptr<Section> m_section;
		std::vector<std::vector<int>> m_analyticalNodeTags;
		std::vector<std::vector<utility::Vector3>> m_analyticalNodeCoords;
		std::vector<int> m_analyticalElementTags;
		AreaElementType m_areaElementType;
		AreaElementFormulation m_areaElementFormulation;

		AreaElement(int elementTag, std::vector<int> jointTags, std::shared_ptr<Section> section, AreaElementFormulation areaElementFormulation);

	private:
		double calculateArea();
		
	public:
		AreaElement() = delete;
		AreaElement(AreaElement&& other) = default;
		AreaElement& operator=(AreaElement&& other) = default;
		virtual ~AreaElement() {}

		void addSurroundingLineElement(int index, int surroundingLineElementTag);
		void addSurroundingShearWallElement(int index, int surroundingShearWallTag);
		void mesh(bool meshable, int n1 = -1, int n2 = -1);
		void setMeshable(bool meshable);
		void addAnalyticalNodeTags(std::vector<int> analyticalNodeTags);
		void addAnalyticalElementTag(int analyticalElementTag);

		// physical model getters
		int getElementTag() const;
		int getIJointTag() const;
		int getJJointTag() const;
		int getKJointTag() const;
		int getLJointTag() const;
		double getMass() const;
		double getArea() const;
		double getThickness() const;
		bool isMeshable() const;
		const std::vector<int>& getJointTags() const;
		std::pair<int, int> getMeshDivisions() const;
		const std::vector<int>& getSurroundingLineElementTags();
		const std::vector<int>& getSurroundingShearWallTags();
		const std::shared_ptr<Section> getSection() const;
		AreaElementType getAreaElementType() const;
		AreaElementFormulation getAreaElementFormulation() const;
		double getWeight() const;
		std::pair<double, double> getTributaryLineLength() const; // to be called only for rectangle areas

		// analytical model getters
		const std::vector<std::vector<int>>& getAnalyticalNodeTags() const;
		const std::vector<std::vector<utility::Vector3>>& getAnalyticalNodeCoords() const;
		const std::vector<int>& getAnalyticalElementTags() const;
	};
}
