#pragma once

#include "Load.h"

namespace physicalModel
{
	class DistributedLineLoad : Load
	{
	private:
		int m_beamElementTag;

		DistributedLineLoad(int beamElementTag, double wz = 0, double wy = 0, double wx = 0);

	public:
		DistributedLineLoad() = delete;
		~DistributedLineLoad() {}

		int getBeamElementTag() const;
	};
}