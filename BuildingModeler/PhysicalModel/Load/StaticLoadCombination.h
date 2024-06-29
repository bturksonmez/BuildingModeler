#pragma once

#include "LoadCase.h"

namespace physicalModel
{
	class StaticLoadCombination
	{
	private:
		bool m_active = false;
		std::string m_loadCombinationTag;
		std::vector<std::pair<std::shared_ptr<LoadCase>, double>> m_loadCases;

	public:
		StaticLoadCombination(std::string loadCombinationTag);
		StaticLoadCombination() = delete;
		~StaticLoadCombination() {}

		void setActive(bool active);
		void addLoadCase(std::shared_ptr<LoadCase> load, double factor);

		bool isActive() const;
		std::string getLoadCombinationTag() const;
		const std::vector<std::pair<std::shared_ptr<LoadCase>, double>>& getLoadCases() const;
	};
}


