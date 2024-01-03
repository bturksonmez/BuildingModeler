#pragma once

#include "../Utilities/Vector2.h"

namespace physicalModel
{
	class Floor
	{
	private:
		int m_floorNumber;
		double m_height;
		double m_mass;
		bool m_isRigid;
		int m_masterJoint;
		std::vector<int> m_joints;
		utility::Vector2 m_massCenter;
		utility::Vector2 m_stiffnessCenter;

		void updateMassCenter();
		void updateStiffnessCenter();

	public:
		Floor(int floorNumber, double height);
		Floor(Floor&& other) = default;
		Floor& operator=(Floor&& other) = default;
		Floor() = delete;
		~Floor() {}

		int getFloorNumber() const;
		const std::vector<int>& getJoints() const;
		const utility::Vector2& getMassCenter() const;
		const utility::Vector2& getStiffnessCenter() const;
		void addJoint(int jointTag);
		void makeRigid(int masterJoint);
		void makeFlexible();
		void updateProperties();
	};
}
