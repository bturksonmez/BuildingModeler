#pragma once

#include <iostream>
#include <vector>

namespace physicalModel
{
	enum class LoadType
	{
		POINT_LOAD,
		DISTRIBUTED_LINE_LOAD,
		DISTRIBUTED_AREA_LOAD
	};

	class Load
	{
	protected:
		LoadType m_loadType;
		std::vector<double> m_loadVector;

		Load() {}

	public:
		~Load() {}

		LoadType getLoadType() const;
		const std::vector<double>& getLoadVector() const;
	};
}

