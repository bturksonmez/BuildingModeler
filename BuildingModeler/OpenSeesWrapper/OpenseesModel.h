#pragma once

#include "Node.h"
#include "Mass.h"
#include "Materials/ElasticMaterial.h"
#include "Sections/ElasticSection.h"
#include "Sections/ElasticMembranePlateSection.h"
#include "GeometricTransformations/LinearGeometricTransformation.h"
#include "GeometricTransformations/PDeltaGeometricTransformation.h"
#include "Elements/ElasticBeamColumnElement.h"
#include "Elements/ShellElement.h"
#include "Constraints/SingleConstraint.h"
#include "Constraints/DiaphragmConstraint.h"

#include <sstream>
#include <fstream>

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
		std::unordered_map<int, std::unique_ptr<Constraint>> m_contraintsSP;
		std::unordered_map<int, std::unique_ptr<Constraint>> m_contraintsDiaphragm;
		std::unordered_map<int, std::unique_ptr<Element>> m_beamColumnElements;
		std::unordered_map<int, std::unique_ptr<Element>> m_quadrilateralElements;
		std::unordered_map<int, std::shared_ptr<Material>> m_materials;
		std::unordered_map<int, std::shared_ptr<GeometricTransformation>> m_geometricTransformation;
		std::unordered_map<int, std::shared_ptr<Section>> m_sections;

		OpenseesModel();

	public:
		~OpenseesModel() {}
		OpenseesModel(OpenseesModel const&) = default;
		OpenseesModel(OpenseesModel&&) = delete;
		OpenseesModel& operator=(OpenseesModel const&) = delete;
		OpenseesModel & operator=(OpenseesModel&&) = delete;

		static OpenseesModel& getInstance();
		Node* getNode(int nodeTag) const;
		Mass* getMass(int nodeTag) const;
		Constraint* getSPConstraint(int nodeTag) const;
		Constraint* getDiaphragmConstraint(int nodeTag) const;
		Element* getBeamColumnElement(int elementTag) const;
		Element* getQuadrilateralElement(int elementTag) const;
		std::shared_ptr<Material> getMaterial(int materialTag) const;
		std::shared_ptr<Section> getSection(int sectionTag) const;

		void toTclFile();

		friend class buildingModeler::OpenseesConverter;
	};
}
