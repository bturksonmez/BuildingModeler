#pragma once

#include "../Utilities/Vector3.h"

#include <optional>

namespace physicalModel
{
	class Joint
	{
	private:
		int m_jointTag;
		utility::Vector3 m_coords;
		std::optional<utility::Vector3> m_massTranslational;
		std::optional<utility::Vector3> m_massRotational;
		std::vector<int> m_connectedBeamTags;
		std::vector<int> m_connectedColumnTags;
		std::vector<int> m_connectedSlabTags;
		std::vector<int> m_connectedWallTags;
		std::optional<std::vector<int>> m_constraintVector;
		int m_floorNo = -1;

	public:
		Joint(int jointTag, utility::Vector3 coords);
		Joint(Joint&& other) = default;
		Joint& operator=(Joint&& other) = default;
		Joint() = delete;
		~Joint() {}

		void addTranslationalMass(utility::Vector3 massValues);
		void addRotationalMass(utility::Vector3 massValues);
		void addConnectedBeam(int elementTag);
		void addConnectedColumn(int elementTag);
		void addConnectedSlab(int elementTag);
		void addConnectedWall(int elementTag);
		void setConstraintVector(std::vector<int> constraintVector);
		void setFloorNo(int floorNo);

		// physical model getters
		int getJointTag() const;
		utility::Vector3 getCoords() const;
		std::optional<utility::Vector3> getTranslationalMass() const;
		std::optional<utility::Vector3> getRotationalMass() const;
		const std::vector<int>& getConnectedBeamTags() const;
		const std::vector<int>& getConnectedColumnTags() const;
		const std::vector<int>& getConnectedSlabTags() const;
		const std::vector<int>& getConnectedWallTags() const;
		std::optional<std::vector<int>> getConstraintVector() const;
		int getFloorNo() const;

		//analytical model getters
		const std::vector<int>& getConstraintVectorFromAnalyticalNode() const;
		const utility::Vector3& getTranslationalMassFromAnalyticalNode() const;
		const utility::Vector3& getRotationalMassFromAnalyticalNode() const;
	};
}
