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
		int m_masterJoint = -1;
		std::vector<int> m_jointTags;
		utility::Vector2 m_massCenter;
		utility::Vector2 m_stiffnessCenter;

		void updateMassCenter();
		void updateStiffnessCenter();

	public:
		Floor(int floorNumber, double height = 0);
		Floor(Floor&& other) = default;
		Floor& operator=(Floor&& other) = default;
		Floor() = delete;
		~Floor() {}

		void addJoint(int jointTag);
		void makeRigid(int masterJoint);
		void makeFlexible();
		void updateProperties();

		int getFloorNumber() const;
		bool isRigid() const;
		const std::vector<int>& getJoints() const;
		const utility::Vector2& getMassCenter() const;
		const utility::Vector2& getStiffnessCenter() const;
	};
}
