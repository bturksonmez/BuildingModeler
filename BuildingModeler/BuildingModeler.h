#pragma once

#include "PhysicalModel/Building.h"

namespace buildingModeler
{
    class BuildingModeler 
    {
    public:
        BuildingModeler() {}
        ~BuildingModeler() {}

        // Joint API
        static void addJoint(int jointTag, utility::Vector3 coords, utility::Vector3 massTranslational = {}, utility::Vector3 massRotational = {});
        static void setTranslationalMass(int jointTag, utility::Vector3 massValues);
        static void setRotationalMass(int jointTag, utility::Vector3 massValues);
        static void setConstraintVector(int jointTag, std::vector<int> constraintVector);
        static void setFloorNo(int jointTag, int floorNo);

    };
}
