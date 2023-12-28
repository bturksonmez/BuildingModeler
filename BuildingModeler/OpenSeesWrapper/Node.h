#pragma once

#include "OpenseesObject.h"
#include "../Utilities/Vector3.h"

namespace opensees
{
	class Node : public OpenseesObject
	{
	private:
		int m_nodeTag;
		utility::Vector3 m_coords;

	public:
		Node(int nodeTag, utility::Vector3 coords);
		Node() = delete;
		~Node() {}

		int getNodeTag() const;
		const utility::Vector3& getCoords() const;

		std::string getOpenseesCommand() const override;
	};
}
