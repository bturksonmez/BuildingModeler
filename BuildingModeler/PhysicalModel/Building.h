#pragma once

#include "Joint.h"
#include "Materials/ElasticMaterial.h"
#include "Sections/Rectangle.h"
#include "Sections/Circle.h"
#include "Sections/ArbitraryShape.h"
#include "Sections/ElasticSection1D.h"
#include "Sections/ElasticSection2D.h"
#include "Elements/ColumnElement.h"
#include "Elements/BeamElement.h"
#include "Elements/ShearWallElement.h"
#include "Elements/SlabElement.h"
#include "Floor.h"
#include "Load/StaticLoadCase.h"
#include "Load/ModalLoadCase.h"
#include "Load/StaticLoadCombination.h"


#include <map>
#include <utility>
#include <memory>

namespace buildingModeler
{
    class BuildingModelerAPI;
}

namespace opensees
{
    class OpenseesModel;
}

namespace physicalModel
{
    class Building
    {
    private:
        bool m_includeMassFromMembers = true;
        bool m_includePDeltaEffects = false;
        bool m_disableSlabElements = false;
        bool m_includeDeadLoadFromMembers = true;
        bool m_gravityThroughLineElements = true;
        std::map<int, std::unique_ptr<Joint>> m_joints;
        std::map<int, std::unique_ptr<LineElement>> m_lineElements;
        std::map<int, std::unique_ptr<AreaElement>> m_areaElements;
        std::map<int, std::unique_ptr<Floor>> m_floors;
        std::map<int, std::shared_ptr<Material>> m_materials;
        std::map<int, std::shared_ptr<Section>> m_sections;
        std::map<int, std::shared_ptr<Load>> m_pointLoads;
        std::map<int, std::shared_ptr<Load>> m_distributedLineLoads;
        std::map<int, std::shared_ptr<Load>> m_distributedAreaLoads;
        std::map<std::string, std::shared_ptr<LoadCase>> m_loadCases;
        std::map<std::string, std::shared_ptr<StaticLoadCombination>> m_staticLoadCombinations;


        Building();

        void addMemberMasses();
        void convertJoints();
        void convertMaterials();
        void convertSections();
        void convertLineElements();
        void convertAreaElements();
        void applyRigidDiaphragms();
        void applyLoads();

    public:
        ~Building() {}
        Building(Building const&) = default;
        Building(Building&&) = delete;
        Building& operator=(Building const&) = delete;
        Building& operator=(Building&&) = delete;

        void deleteJoint(int jointTag);

        static Building& getInstance();
        void clear();
        double getTotalWeight() const;
        Joint* getJoint(int jointTag) const;
        LineElement* getLineElement(int elementTag) const;
        AreaElement* getAreaElement(int elementTag) const;
        Floor* getFloor(int floorNumber) const;
        std::shared_ptr<Material> getMaterial(int materialTag) const;
        std::shared_ptr<Section> getSection(int sectionTag) const;
        std::shared_ptr<Load> getPointLoad(int loadID) const;
        std::shared_ptr<Load> getDistributedLineLoad(int loadID) const;
        std::shared_ptr<Load> getDistributedAreaLoad(int loadID) const;
        std::shared_ptr<LoadCase> getLoadCase(std::string loadCaseTag) const;
        std::shared_ptr<StaticLoadCombination> getStaticLoadCombination(std::string loadCombinationTag) const;

        void updateSurroundingLineElements(); // for area elements
        void toAnalyticalModel();

        std::map<std::pair<int, int>, int> jointHint;

        friend class buildingModeler::BuildingModelerAPI;
        friend class opensees::OpenseesModel;
    };
}
