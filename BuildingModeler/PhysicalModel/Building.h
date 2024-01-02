#pragma once

#include "Joint.h"
#include "Elements/LineElement.h"
#include "Elements/AreaElement.h"
#include "Floor.h"

#include <memory>

namespace physicalModel
{
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
    };
}
