#pragma once

#include <memory>

#include "LoadCase.h"
#include "PointLoad.h"
#include "DistributedLineLoad.h"
#include "DistributedAreaLoad.h"

namespace physicalModel
{
	enum class StaticLoadCaseType
	{
		DEAD,
		LIVE,
		EARTHQUAKE,
		WIND,
		SNOW
	};

	class StaticLoadCase : public LoadCase
	{
	private:
		StaticLoadCaseType m_staticLoadCaseType;
		std::vector<std::shared_ptr<Load>> m_pointLoads;
		std::vector<std::shared_ptr<Load>> m_distributedLineLoads;
		std::vector<std::shared_ptr<Load>> m_distributedAreaLoads;

	public:
		StaticLoadCase(std::string loadCaseTag, StaticLoadCaseType staticLoadCaseType);
		StaticLoadCase() = delete;
		~StaticLoadCase() {}
		
		void addPointLoad(std::shared_ptr<Load> load);
		void addDistributedLineLoad(std::shared_ptr<Load> load);
		void addDistributedAreaLoad(std::shared_ptr<Load> load);

		StaticLoadCaseType getStaticLoadCaseType() const;
		const std::vector<std::shared_ptr<Load>>& getPointLoads() const;
		const std::vector<std::shared_ptr<Load>>& getDistributedLineLoads() const;
		const std::vector<std::shared_ptr<Load>>& getDistributedAreaLoads() const;
	};
}


