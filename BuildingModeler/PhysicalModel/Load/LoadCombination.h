#pragma once

#include "LoadCase.h"

namespace physicalModel
{
	class LoadCombination
	{
	private:
		bool m_active = false;
		std::string m_loadCombinationTag;
		std::vector<std::pair<std::shared_ptr<LoadCase>, double>> m_loadCases;

	public:
		LoadCombination(std::string loadCombinationTag);
		LoadCombination() = delete;
		~LoadCombination() {}

		void setActive(bool active);
		void addLoadCase(std::shared_ptr<LoadCase> load, double factor);

		bool isActive() const;
		std::string getLoadCombinationTag() const;
		const std::vector<std::pair<std::shared_ptr<LoadCase>, double>>& getLoadCases() const;
	};
}


