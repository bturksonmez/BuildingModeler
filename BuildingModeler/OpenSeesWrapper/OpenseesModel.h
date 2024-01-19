#pragma once

#include "Node.h"
#include "Mass.h"
#include "Materials/ElasticMaterial.h"
#include "Sections/ElasticSection.h"
#include "Sections/ElasticMembranePlateSection.h"
#include "GeometricTransformations/LinearGeometricTransformation.h"
#include "Elements/ElasticBeamColumnElement.h"
#include "Elements/ShellElement.h"
#include "Constraints/SingleConstraint.h"
#include "Constraints/DiaphragmConstraint.h"

namespace buildingModeler
{
	class OpenseesConverter;
}

namespace opensees
{
	class OpenseesModel
	{
	private:
		std::unordered_map<int, std::unique_ptr<Node>> m_nodes;
		std::unordered_map<int, std::unique_ptr<Mass>> m_masses;

		OpenseesModel() {}

	public:
		~OpenseesModel() {}
		OpenseesModel(OpenseesModel const&) = default;
		OpenseesModel(OpenseesModel&&) = delete;
		OpenseesModel& operator=(OpenseesModel const&) = delete;
		OpenseesModel & operator=(OpenseesModel&&) = delete;

		static OpenseesModel& getInstance();
		Node* getNode(int nodeTag) const;
		Mass* getMass(int nodeTag) const;

		friend class buildingModeler::OpenseesConverter;
	};
}
