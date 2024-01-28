#pragma once

#include "Joint.h"
#include "Materials/ElasticMaterial.h"
#include "Sections/ElasticSection1D.h"
#include "Sections/ElasticSection2D.h"
#include "Elements/ColumnElement.h"
#include "Elements/BeamElement.h"
#include "Elements/ShearWallElement.h"
#include "Elements/SlabElement.h"
#include "Floor.h"

#include <unordered_map>
#include <utility>
#include <memory>

namespace buildingModeler
{
    class BuildingModelerAPI;
}

namespace physicalModel
{
    class Building
    {
    private:
        bool m_includeMassFromMembers = true;
        bool m_includePDeltaEffects = false;
        std::unordered_map<int, std::unique_ptr<Joint>> m_joints;
        std::unordered_map<int, std::unique_ptr<LineElement>> m_lineElements;
        std::unordered_map<int, std::unique_ptr<AreaElement>> m_areaElements;
        std::unordered_map<int, std::unique_ptr<Floor>> m_floors;
        std::unordered_map<int, std::shared_ptr<Material>> m_materials;
        std::unordered_map<int, std::shared_ptr<Section>> m_sections;

        Building() {}

        void convertJoints();
        void convertMaterials();
        void convertSections();
        void convertLineElements(bool includePDeltaEffects);
        void convertAreaElements();

    public:
        ~Building() {}
        Building(Building const&) = default;
        Building(Building&&) = delete;
        Building& operator=(Building const&) = delete;
        Building& operator=(Building&&) = delete;

        void deleteJoint(int jointTag);

        static Building& getInstance();
        Joint* getJoint(int jointTag) const;
        LineElement* getLineElement(int elementTag) const;
        AreaElement* getAreaElement(int elementTag) const;
        Floor* getFloor(int floorNumber) const;
        std::shared_ptr<Material> getMaterial(int materialTag) const;
        std::shared_ptr<Section> getSection(int sectionTag) const;

        void updateSurroundingLineElements(); // for area elements
        void toAnalyticalModel();

        friend class buildingModeler::BuildingModelerAPI;
    };
}
