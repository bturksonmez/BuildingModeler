#pragma once

#include <iostream>
#include <vector>
#include <memory>

#include "../Sections/Section.h"

namespace physicalModel
{
	struct SectionModifiers
	{
		double m_modifierA;
		double m_modifierIyy;
		double m_modifierIzz;
		double m_modifierJ;

		SectionModifiers(double modifierA = 1.0, double modifierIyy = 1.0, double modifierIzz = 1.0, double modifierJ = 1.0)
			: m_modifierA(modifierA), m_modifierIyy(modifierIyy), m_modifierIzz(modifierIzz), m_modifierJ(modifierJ) {}
	};

	enum class LineElementType
	{
		BEAM,
		COLUMN
	};

	enum class LineElementFormulation
	{
		LINEAR_EULER_BERNOULLI,
		LINEAR_TIMOSHENKO,
		NONLINEAR_DISP_BASED,
		NONLINEAR_FORCE_BASED,
		NONLINEAR_BEAM_WITH_HINGES
	};

	class LineElement
	{
	protected:
		int m_elementTag;
		std::vector<int> m_jointTags;
		double m_length;
		std::vector<double> m_segmentLengths;
		std::vector<double> m_segmentRelativeLengths;
		std::vector<std::shared_ptr<Section>> m_sections;
		std::vector<std::shared_ptr<SectionModifiers>> m_sectionModifiers;
		LineElementType m_lineElementType;
		LineElementFormulation m_lineElementFormulation;

		LineElement() {}
		LineElement(int elementTag, std::vector<int> jointTags, std::shared_ptr<Section> sections, LineElementFormulation lineElementFormulation);

	private:
		double calculateLength(int jointI, int jointJ);

	public:
		LineElement(LineElement&& other) = default;
		LineElement& operator=(LineElement&& other) = default;
		virtual ~LineElement() {}

		void setSegmentRelativeLengths(std::vector<double> segmentRelativeLengths);
		void setSection(int segmentNo, std::shared_ptr<Section> section);
		void setSectionModifiers(int segmentNo, std::shared_ptr<SectionModifiers> sectionModifier);

		int getElementTag() const;
		int getIJointTag() const;
		int getJJointTag() const;
		const std::vector<double>& getSegmentLengths() const;
		const std::vector<double>& getSegmentRelativeLengths() const;
		const std::shared_ptr<Section> getSection(int segmentNo) const;
		const std::shared_ptr<SectionModifiers> getSectionModifiers(int segmentNo) const;
		LineElementType getLineElementType() const;
		LineElementFormulation getLineElementFormulation() const;
	};
}
