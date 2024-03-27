#pragma once

#include "LoadCase.h"

namespace physicalModel
{
	class LoadCombination
	{
	private:
		std::string m_loadCombinationTag;
		std::vector<std::pair<std::shared_ptr<LoadCase>, double>> m_loadCases;

	public:
		LoadCombination(std::string loadCombinationTag);
		LoadCombination() = delete;
		~LoadCombination() {}

		void addLoadCase(std::shared_ptr<LoadCase> load, double factor);

		std::string getLoadCombinationTag() const;
		const std::vector<std::pair<std::shared_ptr<LoadCase>, double>>& getLoadCases() const;
	};
}


