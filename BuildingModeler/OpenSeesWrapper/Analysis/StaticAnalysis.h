#pragma once

#include "Analysis.h"
#include "../Load/LoadPattern.h"

namespace opensees
{
	class StaticAnalysis : public Analysis
	{
	private:
		std::shared_ptr<LoadPattern> m_loadPattern;

	public:
		StaticAnalysis(std::string sourceModelName, std::string analysisName, std::shared_ptr<Output> output, std::shared_ptr<LoadPattern> loadPattern);
		~StaticAnalysis() {}

		std::shared_ptr<LoadPattern> getLoadPattern() const;
		std::string getOpenseesCommand() const override;
	};
}