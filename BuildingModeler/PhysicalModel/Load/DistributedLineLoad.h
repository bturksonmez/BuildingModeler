#pragma once

#include "Load.h"

namespace physicalModel
{
	class DistributedLineLoad : public Load
	{
	private:
		int m_beamElementTag;

	public:
		DistributedLineLoad(int beamElementTag, double wy = 0, double wz = 0, double wx = 0);
		DistributedLineLoad() = delete;
		~DistributedLineLoad() {}

		int getBeamElementTag() const;
	};
}