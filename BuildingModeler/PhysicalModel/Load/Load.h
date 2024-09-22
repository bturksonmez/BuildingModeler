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
		int m_uniqueID;
		LoadType m_loadType;
		std::vector<double> m_loadVector;
		static int counter;

		Load() {}

	public:
		virtual ~Load() {}

		int getUniqueID() const;
		LoadType getLoadType() const;
		std::vector<double> getLoadVector() const;
		static void resetCounter();
	};
}

