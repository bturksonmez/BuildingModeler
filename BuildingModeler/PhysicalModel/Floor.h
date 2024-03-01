#pragma once

#include "../Utilities/Vector2.h"

#include <optional>

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
		std::optional<utility::Vector2> m_massCenter;
		std::optional<utility::Vector2> m_stiffnessCenter;

	public:
		Floor(int floorNumber, double height = 0);
		Floor(Floor&& other) = default;
		Floor& operator=(Floor&& other) = default;
		Floor() = delete;
		~Floor() {}

		void addJoint(int jointTag);
		void makeRigid(int masterJoint);
		void makeFlexible();
		bool updateMassCenter();
		void updateStiffnessCenter();

		int getFloorNumber() const;
		double getFloorHeight() const;
		double getFloorMass() const;
		bool isRigid() const;
		int getMassCenterJointTag() const;
		const std::vector<int>& getJoints() const;
		std::optional<utility::Vector2> getMassCenter() const;
		std::optional<utility::Vector2> getStiffnessCenter() const;
	};
}
