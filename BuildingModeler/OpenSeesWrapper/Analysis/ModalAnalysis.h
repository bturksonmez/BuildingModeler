#pragma once

#include "Analysis.h"
#include "../Load/LoadPattern.h"

namespace opensees
{
	class ModalAnalysis : public Analysis
	{
	private:
		size_t m_numberOfModes;
		std::vector<int> m_masterNodeTags;

	public:
		ModalAnalysis(std::string sourceModelName, std::string analysisName, std::shared_ptr<Output> output, size_t numberOfModes, std::vector<int> masterNodeTags);
		~ModalAnalysis() {}

		size_t getNumberOfModes() const;
		const std::vector<int>& getMasterNodeTags() const;
		std::string getOpenseesCommand() const override;
	};
}