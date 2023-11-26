#pragma once

#include "OpenseesObject.h"
#include "Node.h"

namespace opensees
{
	class Mass : public OpenseesObject
	{
	protected:
		std::shared_ptr<Node> m_node;
		std::vector<double> m_massValues;

	public:
		Mass(std::shared_ptr<Node>, std::vector<double> massValues);
		Mass() = delete;
		~Mass() {}

		const std::shared_ptr<Node> getNode() const;
		const std::vector<double>& getMassValues() const;

		std::string getOpenseesCommand() const override;
	};
}
