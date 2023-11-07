#pragma once

#include "Object.h"
#include "Load/Load.h"

namespace opensees
{
	class Element;

	class Node : public Object
	{
	private:
		int m_nodeTag;
		std::vector<double> m_coords;
		std::vector<double> m_massValues;
		std::unordered_set<std::shared_ptr<Element>> m_connectedElements;
		std::vector<std::shared_ptr<Load>> m_nodalLoads;

	public:
		Node(int nodeTag, std::vector<double> coords, std::vector<double> massValues = {});
		Node() = delete;
		~Node() {}

		int getNodeTag() const;
		const std::vector<double>& getCoords() const;
		const std::vector<double>& getMassValues() const;
		const std::unordered_set<std::shared_ptr<Element>>& getConnectedElements() const;
		const std::vector<std::shared_ptr<Load>>& getNodalLoads() const;
		void setMassValues(std::vector<double> massValues);
		void addConnectedElements(std::shared_ptr<Element> element);
		void addNodalLoad(std::shared_ptr<Load> nodalLoad);

		std::string getOpenseesCommand() const override;
	};
}
