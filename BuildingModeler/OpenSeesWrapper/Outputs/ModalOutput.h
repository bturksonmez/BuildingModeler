#pragma once

#include "Output.h"

namespace opensees
{
	class ModalOutput : public Output
	{
	private:
		std::vector<double> m_periods;
		std::vector<int> m_masterNodeTags;
		std::vector<std::vector<double>> m_modeShapeX;
		std::vector<std::vector<double>> m_modeShapeY;
		std::vector<std::vector<double>> m_modeShapeXY;


		bool retrievePeriods();
		void retrieveModeShapes();
		void retrieveModeShapesAsDisplacements();
		bool isMonotonicallyIncreasingEigenVector(const std::vector<double>& eigenVector);
		// Compares if desired eigen vector is bigger in amplitude compared to its candidate
		bool isBetterThanCandidate(const std::vector<double>& desiredEigenVector, const std::vector<double>& candidateEigenVector);

	public:
		ModalOutput(std::string loadTag, std::vector<int> masterNodeTags);
		ModalOutput() = delete;
		~ModalOutput() {}

		const std::vector<std::vector<double>>& getModeShapeX();
		const std::vector<std::vector<double>>& getModeShapeY();
		const std::vector<std::vector<double>>& getModeShapeXY();
		const std::vector<double>& getPeriods() const;
		double getFundamentalPeriod(size_t dof);

		bool retrieveOutput() override;
	};
}