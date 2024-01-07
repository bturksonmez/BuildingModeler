#pragma once

#include <iostream>
#include <vector>
#include <memory>

#include "../Sections/Section.h"

namespace physicalModel
{
	struct SectionModifiers
	{
		double m_modA;
		double m_modIyy;
		double m_modIzz;
		double m_modJ;

		SectionModifiers(double modA = 1.0, double modIyy = 1.0, double modIzz = 1.0, double modJ = 1.0)
			: m_modA(modA), m_modIyy(modIyy), m_modIzz(modIzz), m_modJ(modJ) {}
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
		std::vector<double> m_segmentLengths;
		std::vector<std::shared_ptr<Section>> m_sections;
		std::vector<std::shared_ptr<SectionModifiers>> m_sectionModifiers;
		LineElementType m_lineElementType;
		LineElementFormulation m_lineElementFormulation;

		LineElement() {}
		LineElement(int elementTag, std::vector<int> jointTags, std::shared_ptr<Section> sections, LineElementFormulation lineElementFormulation);

	public:
		LineElement(LineElement&& other) = default;
		LineElement& operator=(LineElement&& other) = default;
		virtual ~LineElement() {}

		void setSegmentLengths(std::vector<double> segmentLengths);
		void setSection(int segmentNo, std::shared_ptr<Section> section);
		void setSectionModifiers(int segmentNo, std::shared_ptr<SectionModifiers> sectionModifier);

		int getElementTag() const;
		int getIJointTag() const;
		int getJJointTag() const;
		const std::vector<double>& getSegmentLengths() const;
		const std::shared_ptr<Section> getSection(int segmentNo) const;
		const std::shared_ptr<SectionModifiers> getSectionModifiers(int segmentNo) const;
		LineElementType getLineElementType() const;
		LineElementFormulation getLineElementFormulation() const;
	};
}
