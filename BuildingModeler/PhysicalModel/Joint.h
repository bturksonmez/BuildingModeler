#pragma once

#include <iostream>
#include <vector>

namespace physicalModel
{
	class Joint
	{
	private:
		int m_jointTag;
		std::vector<double> m_coords;
		std::vector<double> m_massValues;
		std::vector<int> m_connectedBeamTags;
		std::vector<int> m_connectedColumnTags;
		std::vector<int> m_connectedSlabElementTags;
		std::vector<int> m_connectedWallTags;
		int m_floorNo;

	public:
		Joint(int nodeTag, std::vector<double> coords, std::vector<double> massValues = {});
		Joint() = delete;
		~Joint() {}

		int getJointTag() const;
		const std::vector<double>& getCoords() const;
		const std::vector<double>& getMassValues() const;
		const std::vector<int>& getConnectedBeamTags() const;
		const std::vector<int>& getConnectedColumnTags() const;
		const std::vector<int>& getConnectedSlabElementTags() const;
		const std::vector<int>& getConnectedWallTags() const;
		int getFloorNo() const;
		void setMassValues(std::vector<double> massValues);
		void addConnectedBeam(int elementTag);
		void addConnectedColumn(int elementTag);
		void addConnectedSlabElement(int elementTag);
		void addConnectedWall(int elementTag);
		void setFloorNo(int floorNo);
	};
}
