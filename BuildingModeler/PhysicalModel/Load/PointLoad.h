#pragma once

#include "Load.h"

namespace physicalModel
{
	class PointLoad : Load
	{
	private:
		int m_jointTag;

		PointLoad(int jointTag, double fx = 0, double fy = 0, double fz = 0, double mx = 0, double my = 0, double mz = 0);

	public:
		PointLoad() = delete;
		~PointLoad() {}

		int getJointTag() const;
	};
}