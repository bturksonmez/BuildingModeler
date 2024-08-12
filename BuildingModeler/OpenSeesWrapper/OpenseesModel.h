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
#include "Load/LoadPattern.h"
#include "Analysis/StaticAnalysis.h"
#include "Analysis/ModalAnalysis.h"
#include "Outputs/StaticOutput.h"
#include "Outputs/ModalOutput.h"
#include "../PhysicalModel/Building.h"

#include <sstream>
#include <fstream>
#include <unordered_set>

namespace buildingModeler
{
	class OpenseesConverter;
}

namespace opensees
{
	class OpenseesModel
	{
	private:
		std::string m_modelName = "model";
		std::map<int, std::unique_ptr<Node>> m_nodes;
		std::map<int, std::unique_ptr<Mass>> m_masses;
		std::map<int, std::unique_ptr<Constraint>> m_contraintsSP;
		std::map<int, std::unique_ptr<Constraint>> m_contraintsDiaphragm;
		std::unordered_map<int, std::unique_ptr<Element>> m_beamColumnElements;
		std::unordered_map<int, std::unique_ptr<Element>> m_quadrilateralElements;
		std::map<int, std::shared_ptr<Material>> m_materials;
		std::map<int, std::shared_ptr<GeometricTransformation>> m_geometricTransformation;
		std::map<int, std::shared_ptr<Section>> m_sections;
		std::map<std::string, std::shared_ptr<LoadPattern>> m_loadPatterns;
		std::unordered_map<std::string, std::shared_ptr<Analysis>> m_analyses;
		std::map<std::pair<int, int>, std::vector<int>> m_divisionsBetweenNodes;
		std::map<int, opensees::ElementType> m_outputElements;

		OpenseesModel();

		void createLoadingTclFiles();

	public:
		~OpenseesModel() {}
		OpenseesModel(OpenseesModel const&) = default;
		OpenseesModel(OpenseesModel&&) = delete;
		OpenseesModel& operator=(OpenseesModel const&) = delete;
		OpenseesModel & operator=(OpenseesModel&&) = delete;

		static OpenseesModel& getInstance();
		void clear();
		std::string getModelName() const;
		Node* getNode(int nodeTag) const;
		Mass* getMass(int nodeTag) const;
		Constraint* getSPConstraint(int nodeTag) const;
		Constraint* getDiaphragmConstraint(int nodeTag) const;
		Element* getBeamColumnElement(int elementTag) const;
		Element* getQuadrilateralElement(int elementTag) const;
		std::shared_ptr<Material> getMaterial(int materialTag) const;
		std::shared_ptr<Section> getSection(int sectionTag) const;
		std::shared_ptr<LoadPattern> getLoadPattern(std::string loadingName) const;
		std::shared_ptr<Analysis> getAnalysis(std::string analysesName) const;
		std::vector<int> getDivisionsBetweenNodes(int nodeA, int nodeB) const;
		std::vector<int> getOutputNodes() const;
		const std::map<int, opensees::ElementType>& getOutputElements() const;

		void setModelName(std::string modelName);
		void addDivisionsBetweenNodes(int nodeA, int nodeB, std::vector<int> dividedNodes);
		void addOutputElement(int elementTag, opensees::ElementType elementType);
		void toTclFile();
		void analyze();

		friend class buildingModeler::OpenseesConverter;
	};
}
