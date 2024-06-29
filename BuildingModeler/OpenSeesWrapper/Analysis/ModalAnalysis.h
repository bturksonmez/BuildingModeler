#pragma once

#include "Analysis.h"
#include "../Load/LoadPattern.h"

namespace opensees
{
	class ModalAnalysis : public Analysis
	{
	private:
		size_t m_numberOfModes;

	public:
		ModalAnalysis(std::string sourceModelName, size_t numberOfModes);
		~ModalAnalysis() {}

		size_t getNumberOfModes() const;
		std::string getOpenseesCommand() const override;
	};
}