#pragma once

#include "../Utilities/Vector3.h"

namespace physicalModel
{
	class Joint
	{
	private:
		int m_jointTag;
		utility::Vector3 m_coords;
		utility::Vector3 m_massTranslational;
		utility::Vector3 m_massRotational;
		std::vector<int> m_connectedBeamTags;
		std::vector<int> m_connectedColumnTags;
		std::vector<int> m_connectedSlabTags;
		std::vector<int> m_connectedWallTags;
		std::vector<int> m_constraintVector;
		int m_floorNo;

	public:
		Joint(int jointTag, utility::Vector3 coords, utility::Vector3 massTranslational = {}, utility::Vector3 massRotational = {});
		Joint(Joint&& other) = default;
		Joint& operator=(Joint&& other) = default;
		Joint() = delete;
		~Joint() {}

		void setTranslationalMass(utility::Vector3 massValues);
		void setRotationalMass(utility::Vector3 massValues);
		void addConnectedBeam(int elementTag);
		void addConnectedColumn(int elementTag);
		void addConnectedSlab(int elementTag);
		void addConnectedWall(int elementTag);
		void setConstraintVector(std::vector<int> constraintVector);
		void setFloorNo(int floorNo);

		int getJointTag() const;
		utility::Vector3 getCoords() const;
		utility::Vector3 getTranslationalMass() const;
		utility::Vector3 getRotationalMass() const;
		const std::vector<int>& getConnectedBeamTags() const;
		const std::vector<int>& getConnectedColumnTags() const;
		const std::vector<int>& getConnectedSlabTags() const;
		const std::vector<int>& getConnectedWallTags() const;
		const std::vector<int>& getConstraintVector() const;
		int getFloorNo() const;
	};
}
