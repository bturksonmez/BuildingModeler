#pragma once

#include "Joint.h"
#include "Materials/ElasticMaterial.h"
#include "Sections/ElasticSection1D.h"
#include "Sections/ElasticSection2D.h"
#include "Elements/ColumnElement.h"
#include "Elements/BeamElement.h"
#include "Elements/ShearWallElement.h"
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
        std::unordered_map<int, std::unique_ptr<Joint>> m_joints;
        std::unordered_map<int, std::unique_ptr<LineElement>> m_lineElements;
        std::unordered_map<int, std::unique_ptr<AreaElement>> m_areaElements;
        std::unordered_map<int, std::unique_ptr<Floor>> m_floors;
        std::unordered_map<int, std::shared_ptr<Material>> m_materials;
        std::unordered_map<int, std::shared_ptr<Section>> m_sections;

        Building() {}

    public:
        ~Building() {}
        Building(Building const&) = default;
        Building(Building&&) = delete;
        Building& operator=(Building const&) = delete;
        Building& operator=(Building&&) = delete;

        void deleteJoint(int jointTag);
        void deleteLineElement(int elementTag);
        void deleteAreaElement(int elementTag);
        void deleteFloor(int floorNumber);
        void deleteMaterial(int materialTag);
        void deleteSection(int sectionTag);

        static Building& getInstance();
        Joint* getJoint(int jointTag) const;
        LineElement* getLineElement(int elementTag) const;
        AreaElement* getAreaElement(int elementTag) const;
        Floor* getFloor(int floorNumber) const;
        std::shared_ptr<Material> getMaterial(int materialTag) const;
        std::shared_ptr<Section> getSection(int sectionTag) const;

        friend class buildingModeler::BuildingModelerAPI;
    };
}
