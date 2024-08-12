#pragma once

#include "../OpenseesObject.h"
#include "../Outputs/Output.h"

namespace opensees
{
	enum class AnalysisType
	{
		STATIC,
		MODAL
	};

	class Analysis : public OpenseesObject
	{
	protected:
		size_t m_analysisID;
		std::string m_sourceModelName;
		std::string m_analysisName;
		AnalysisType m_analysisType;
		std::shared_ptr<Output> m_output;
		static size_t counter;

		Analysis(std::string sourceModelName, std::string analysisName, std::shared_ptr<Output> output);

	public:
		Analysis() = delete;
		~Analysis() {}

		size_t getAnalysisID() const;
		std::string getSourceModelName() const;
		std::string getAnalysisName() const;
		AnalysisType getAnalysisType() const;
		std::shared_ptr<Output> getOutput() const;
		
		void perform();
	};
}