#pragma once

#include "PhysicalModel/Building.h"

namespace buildingModeler
{
    class BuildingModelerAPI
    {
    public:
        BuildingModelerAPI() {}
        ~BuildingModelerAPI() {}

        // Joint API
        static void addJoint(int jointTag, utility::Vector3 coords, utility::Vector3 massTranslational = {}, utility::Vector3 massRotational = {});
        static void setTranslationalMass(int jointTag, utility::Vector3 massValues);
        static void setRotationalMass(int jointTag, utility::Vector3 massValues);
        static void setConstraintVector(int jointTag, std::vector<int> constraintVector);
        static void setFloorNo(int jointTag, int floorNo);
        
        // Line Element API
        static void addBeam(int elementTag, std::vector<int> jointTags, std::vector<std::shared_ptr<physicalModel::Section>> sections,
            std::vector<std::shared_ptr<physicalModel::SectionModifiers>> sectionModifiers, physicalModel::LineElementFormulation lineElementFormulation);

    };
}
