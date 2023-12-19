#pragma once

#include <iostream>
#include <vector>

namespace physicalModel
{
	class FloorPlan
	{
	private:;
		std::vector<double> m_xAxes;
		std::vector<double> m_yAxes;
		std::vector<std::vector<std::pair<double, double>>> m_jointLocationsXY;
		std::vector<std::vector<bool>> m_activeJoints;
		std::vector<std::vector<bool>> m_disabledColumns;

	public:
		FloorPlan(std::vector<double> xAxes, std::vector<double> yAxes, std::vector<std::vector<bool>> activeJoints = {}, std::vector<std::vector<bool>> disabledColumns = {});
		FloorPlan() = delete;
		~FloorPlan() {}

		const std::vector<double>& getXAxes() const;
		const std::vector<double>& getYAxes() const;
		const std::vector<std::vector<std::pair<double, double>>>& getJointLocationsXY() const;
		const std::vector<std::vector<bool>>& getActiveJoints() const;
		const std::vector<std::vector<bool>>& getDisabledColumns() const;
	};
}
