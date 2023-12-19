#include "FloorPlan.h"

using namespace std;
using namespace physicalModel;

FloorPlan::FloorPlan(vector<double> xAxes, vector<double> yAxes, vector<vector<bool>> activeJoints, vector<vector<bool>> disabledColumns)
{
	if (activeJoints.empty()) {
		activeJoints.resize(yAxes.size() + 1);

		for (size_t i = 0; i <= yAxes.size(); ++i) {
			activeJoints[i].resize(xAxes.size() + 1, true);
		}
	}

	if (disabledColumns.empty()) {
		disabledColumns.resize(yAxes.size() + 1);

		for (size_t i = 0; i <= yAxes.size(); ++i) {
			disabledColumns[i].resize(xAxes.size() + 1, false);
		}
	}

	double coordY = 0.0;
	for (size_t i = 0; i <= yAxes.size(); ++i) {
		double coordX = 0.0;

		for (size_t j = 0; j <= xAxes.size(); ++j) {
			m_jointLocationsXY[i].push_back({ coordX, yAxes[i] });
			coordX += (j == xAxes.size() ? 0 : xAxes[j]);
		}

		coordY += (i == yAxes.size() ? 0 : yAxes[i]);
	}
}

const vector<double>& FloorPlan::getXAxes() const
{
	return m_xAxes;
}

const vector<double>& FloorPlan::getYAxes() const
{
	return m_xAxes;
}

const vector<vector<pair<double, double>>>& FloorPlan::getJointLocationsXY() const
{
	return m_jointLocationsXY;
}

const vector<vector<bool>>& FloorPlan::getActiveJoints() const
{
	return m_activeJoints;
}

const vector<vector<bool>>& FloorPlan::getDisabledColumns() const
{
	return m_disabledColumns;
}