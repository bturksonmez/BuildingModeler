#pragma once

#include <memory>

#include "PointLoad.h"
#include "DistributedLineLoad.h"
#include "DistributedAreaLoad.h"

namespace physicalModel
{
	enum class LoadCaseType
	{
		DEAD,
		LIVE,
		EARTHQUAKE,
		WIND,
		SNOW
	};

	class LoadCase
	{
	private:
		bool m_active = false;
		std::string m_loadCaseTag;
		LoadCaseType m_loadCaseType;
		std::vector<std::shared_ptr<Load>> m_pointLoads;
		std::vector<std::shared_ptr<Load>> m_distributedLineLoads;
		std::vector<std::shared_ptr<Load>> m_distributedAreaLoads;

	public:
		LoadCase(std::string loadCaseTag, LoadCaseType loadCaseType);
		LoadCase() = delete;
		~LoadCase() {}
		
		void setActive(bool active);
		void addPointLoad(std::shared_ptr<Load> load);
		void addDistributedLineLoad(std::shared_ptr<Load> load);
		void addDistributedAreaLoad(std::shared_ptr<Load> load);

		bool isActive() const;
		std::string getLoadCaseTag() const;
		LoadCaseType getLoadCaseType() const;
		const std::vector<std::shared_ptr<Load>>& getPointLoads() const;
		const std::vector<std::shared_ptr<Load>>& getDistributedLineLoads() const;
		const std::vector<std::shared_ptr<Load>>& getDistributedAreaLoads() const;
	};
}


