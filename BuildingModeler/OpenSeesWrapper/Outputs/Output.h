#pragma once

#include <vector>
#include <string>
#include <unordered_map>

#include "../Utilities/DataReader.h"

namespace opensees
{
	enum class OutputType
	{
		STATIC,
		TRANSIENT,
		MODAL
	};

	class Output
	{
	protected:
		int m_outputTag;
		std::string m_loadTag;
		OutputType m_outputType;
		std::unordered_map<int, std::vector<std::vector<double>>> m_nodeDispOutput;
		static int counter;

		Output(std::string loadTag);

	public:
		Output() = delete;
		virtual ~Output() {}

		int getOutputTag() const;
		std::string getLoadTag() const;
		OutputType getOutputType() const;
		const std::unordered_map<int, std::vector<std::vector<double>>>& getNodeDisplacement() const;
		static void resetCounter();

		virtual bool retrieveOutput() = 0;
	};
}