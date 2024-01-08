#pragma once

#include "PhysicalModel/Building.h"

namespace buildingModeler
{
    class BuildingModelerAPI
    {
    private:
        static bool jointExists(int jointTag);
        static bool lineElementExists(int elementTag);
        static bool areaElementExists(int elementTag);
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
        static void addElasticMaterial(int materialTag, double E, double G, double rho);

        // Section API
        static void addElasticSection1D(int sectionTag, int materialTag, double A, double Iyy, double Izz, double J);
        static void addElasticSection2D(int sectionTag, int materialTag, double thickness);
        
        // Line Element API
        static void addBeam(int elementTag, std::vector<int> jointTags, int sectionTag,
            physicalModel::LineElementFormulation lineElementFormulation);
        static void addColumn(int elementTag, std::vector<int> jointTags, int sectionTag,
            physicalModel::LineElementFormulation lineElementFormulation);
        static void setSegmentRatios(int elementTag, std::vector<double> segmentRatios);
        static void setSection(int elementTag, int segmentNo, int sectionTag);
        static void setSectionModifiers(int elementTag, int segmentNo, double modifierA, double modifierIyy, double modifierIzz, double modifierJ);

        // Floor API
    };
}
