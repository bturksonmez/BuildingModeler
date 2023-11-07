#pragma once

#include "Load.h"

namespace opensees
{
	class Node;

	class NodalLoad : Load
	{
	private:
		std::weak_ptr<Node> m_node;

		NodalLoad(std::shared_ptr<Node> node, std::vector<double> loadVector);

	public:
		NodalLoad() = delete;
		~NodalLoad() {}

		const std::shared_ptr<Node> getNode() const;
		std::string getOpenseesCommand() const override;
	};
}

