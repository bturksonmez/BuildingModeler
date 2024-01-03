#pragma once

#include "Joint.h"
#include "Elements/ColumnElement.h"
#include "Elements/BeamElement.h"
#include "Elements/ShearWallElement.h"
#include "Floor.h"

#include <unordered_map>
#include <memory>

namespace physicalModel
{
    class BuildingModeler;

    class Building
    {
    private:
        std::unordered_map<int, std::unique_ptr<Joint>> m_joints;
        std::unordered_map<int, std::unique_ptr<LineElement>> m_columns;
        std::unordered_map<int, std::unique_ptr<LineElement>> m_beams;
        std::unordered_map<int, std::unique_ptr<AreaElement>> m_slabs;
        std::unordered_map<int, std::unique_ptr<AreaElement>> m_shearWalls;
        std::unordered_map<int, std::unique_ptr<Floor>> m_floors;

        Building() {}

    public:
        ~Building() {}
        Building(Building const&) = delete;
        Building(Building&&) = delete;
        Building& operator=(Building const&) = delete;
        Building& operator=(Building&&) = delete;

        static Building& getInstance();
        Joint* getJoint(int jointTag) const;
        LineElement* getColumn(int elementTag) const;
        LineElement* getBeam(int elementTag) const;
        AreaElement* getSlab(int elementTag) const;
        AreaElement* getShearWall(int elementTag) const;
        Floor* getFloor(int floorNumber) const;

        friend class BuildingModeler;
    };
}
