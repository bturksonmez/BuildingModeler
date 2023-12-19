#pragma once

#include "Joint.h"
#include "FloorPlan.h"

namespace physicalModel
{
	class Floor
	{
	private:
		int m_floorNumber;
		static FloorPlan m_floorPlan;

	public:
		Floor(int floorNumber);
		Floor() = delete;
		~Floor() {}

		void setFloorPlan(FloorPlan floorPlan);
		int getFloorNumber() const;
		const FloorPlan& getFloorPlan() const;
	};
}
