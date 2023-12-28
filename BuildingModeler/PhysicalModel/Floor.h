#pragma once

#include <iostream>
#include <vector>

namespace physicalModel
{
	class Floor
	{
	private:
		int m_floorNumber;
		bool m_isRigid;
		std::vector<int> m_joints;

		void updateMassCenter();
		void updateStiffnessCenter();

	public:
		Floor(int floorNumber);
		Floor() = delete;
		~Floor() {}

		int getFloorNumber() const;
		const std::vector<int>& getJoints() const;
		void addJoint(int jointTag);
		void makeRigid();
		void makeFlexible();
		void updateProperties();
		std::vector<double> getMassCenter();
	};
}
