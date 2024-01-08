#pragma once

#include "PhysicalModel/Building.h"

namespace buildingModeler
{
    class BuildingModelerAPI
    {
    private:
        static bool jointExists(int jointTag);
        static bool beamExists(int elementTag);
        static bool columnExists(int elementTag);
        static bool slabExists(int elementTag);
        static bool shearWallExists(int elementTag);
        static bool floorExists(int floorNo);
        static bool materialExists(int materialTag);
        static bool sectionExists(int sectionTag);

    public:
        BuildingModelerAPI() {}
        ~BuildingModelerAPI() {}

        // Joint API
        static void addJoint(int jointTag, utility::Vector3 coords, utility::Vector3 massTranslational = {}, utility::Vector3 massRotational = {});
        static void setTranslationalMass(int jointTag, utility::Vector3 massValues);
        static void setRotationalMass(int jointTag, utility::Vector3 massValues);
        static void setConstraintVector(int jointTag, std::vector<int> constraintVector);
        static void setFloorNo(int jointTag, int floorNo);

        // Material API

        // Section API
        
        // Line Element API
        static void addBeam(int elementTag, std::vector<int> jointTags, int sectionTag,
            physicalModel::LineElementFormulation lineElementFormulation);

        // Floor API
    };
}
