#pragma once

#include <iostream>
#include <vector>
#include <memory>

#include "ILineElementOutputFetcher.h"
#include "../Sections/Section.h"
#include "../../Utilities/Vector3.h"

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

	class LineElement : public ILineElementOutputFetcher
	{
	protected:
		int m_elementTag;
		std::vector<int> m_jointTags;
		double m_length;
		std::vector<double> m_segmentLengths;
		std::vector<double> m_segmentRelativeLengths;
		std::vector<std::shared_ptr<Section>> m_sections;
		std::vector<std::shared_ptr<SectionModifiers>> m_sectionModifiers;
		std::vector<int> m_analyticalNodeTags;
		std::vector<utility::Vector3> m_analyticalNodeCoords;
		std::vector<int> m_analyticalElementTags;
		LineElementType m_lineElementType;
		LineElementFormulation m_lineElementFormulation;

		LineElement() {}
		LineElement(int elementTag, std::vector<int> jointTags, std::shared_ptr<Section> section, LineElementFormulation lineElementFormulation);

	private:
		double calculateLength();

	public:
		LineElement(LineElement&& other) = default;
		LineElement& operator=(LineElement&& other) = default;
		virtual ~LineElement() {}

		void setSegmentRelativeLengths(std::vector<double> segmentRelativeLengths);
		void setSection(int segmentNo, std::shared_ptr<Section> section);
		void setSectionModifiers(int segmentNo, std::shared_ptr<SectionModifiers> sectionModifier);
		void addAnalyticalNodeTag(int analyticalNodeTag);
		void addAnalyticalElementTag(int analyticalElementTag);

		// physical model getters
		int getElementTag() const;
		int getIJointTag() const;
		int getJJointTag() const;
		double getMass() const;
		double getLength() const;
		const std::vector<double>& getSegmentLengths() const;
		const std::vector<double>& getSegmentRelativeLengths() const;
		const std::shared_ptr<Section> getSection(int segmentNo) const;
		const std::shared_ptr<SectionModifiers> getSectionModifiers(int segmentNo) const;
		LineElementType getLineElementType() const;
		LineElementFormulation getLineElementFormulation() const;
		double getWeight() const;

		// analytical model getters
		std::vector<int> getAnalyticalNodeTags() const;
		const std::vector<utility::Vector3>& getAnalyticalNodeCoords() const;
		const std::vector<int>& getAnalyticalElementTags() const;

		// output fetcher
		double calculateForceX(std::string analysisTag, bool atIJoint, size_t timeStep) override;
		double calculateForceY(std::string analysisTag, bool atIJoint, size_t timeStep) override;
		double calculateForceZ(std::string analysisTag, bool atIJoint, size_t timeStep) override;
		double calculateMomentXX(std::string analysisTag, bool atIJoint, size_t timeStep) override;
		double calculateMomentYY(std::string analysisTag, bool atIJoint, size_t timeStep) override;
		double calculateMomentZZ(std::string analysisTag, bool atIJoint, size_t timeStep) override;
		double calculateDR(std::string analysisTag, size_t dof,  bool fromIJoint, size_t timeStep) override;
		double calculateChordRotation(std::string analysisTag, size_t dofRot, size_t dofRelDisp, bool fromIJoint, size_t timeStep) override;
		double calculateDisplacement(std::string analysisTag, size_t segmentNode, size_t dof, size_t timeStep) override;
	};
}
