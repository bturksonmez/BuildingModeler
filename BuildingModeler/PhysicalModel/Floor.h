#pragma once

#include "../Utilities/Vector2.h"

namespace physicalModel
{
	class Floor
	{
	private:
		int m_floorNumber;
		bool m_isRigid;
		std::vector<int> m_joints;
		utility::Vector2 m_massCenter;
		utility::Vector2 m_stiffnessCenter;

		void updateMassCenter();
		void updateStiffnessCenter();

	public:
		Floor(int floorNumber);
		Floor() = delete;
		~Floor() {}

		int getFloorNumber() const;
		const std::vector<int>& getJoints() const;
		const utility::Vector2& getMassCenter() const;
		const utility::Vector2& getStiffnessCenter() const;
		void addJoint(int jointTag);
		void makeRigid();
		void makeFlexible();
		void updateProperties();
	};
}
