#pragma once

#include "OpenseesObject.h"

namespace opensees
{
	class Node : public OpenseesObject
	{
	private:
		int m_nodeTag;
		std::vector<double> m_coords;
		std::vector<double> m_massValues;

	public:
		Node(int nodeTag, std::vector<double> coords, std::vector<double> massValues = {});
		Node() = delete;
		~Node() {}

		int getNodeTag() const;
		const std::vector<double>& getCoords() const;
		const std::vector<double>& getMassValues() const;
		void setMassValues(std::vector<double> massValues);

		std::string getOpenseesCommand() const override;
	};
}
