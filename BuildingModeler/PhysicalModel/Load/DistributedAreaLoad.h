#pragma once

#include "Load.h"

namespace physicalModel
{
	class DistributedAreaLoad : public Load
	{
	private:
		int m_areaElementTag;

	public:
		DistributedAreaLoad(int areaElementTag, double wy = 0, double wz = 0, double wx = 0);
		DistributedAreaLoad() = delete;
		~DistributedAreaLoad() {}

		int getAreaElementTag() const;
	};
}