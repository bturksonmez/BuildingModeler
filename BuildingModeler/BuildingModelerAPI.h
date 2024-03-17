#pragma once

#include "PhysicalModel/Building.h"
#include "OpenseesWrapper/OpenseesModel.h"
#include "BuildingModelerExceptions.h"

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

        static void invalidateAreaMeshAlongLineElement(int elementTag);
        static bool checkIfJointsCoplanar(const std::vector<utility::Vector3>& joints);
        static bool checkIfJointsCounterClockwise(const std::vector<utility::Vector3>& joints);
        static bool checkIfQuadConvex(const std::vector<utility::Vector3>& joints);

    public:
        BuildingModelerAPI() {}
        ~BuildingModelerAPI() {}

        // Joint API
        static void addJoint(int jointTag, utility::Vector3 coords);
        static void addTranslationalMass(int jointTag, utility::Vector3 massValues);
        static void addRotationalMass(int jointTag, utility::Vector3 massValues);
        static void setConstraintVector(int jointTag, std::vector<int> constraintVector);
        static void setFloorNo(int jointTag, int floorNo);
        static void includeMassFromMembers(bool includeMassFromMembers);

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
        static const std::vector<utility::Vector3>& getNodeCoordinatesOfLineElement(int elementTag);

        // Area Element API
        static void addShearWall(int elementTag, std::vector<int> jointTags, int sectionTag,
            physicalModel::AreaElementFormulation areaElementFormulation);
        static void addSlab(int elementTag, std::vector<int> jointTags, int sectionTag,
            physicalModel::AreaElementFormulation areaElementFormulation);
        static void meshAreaElement(int elementTag, std::optional<int> n1, std::optional<int> n2);
        static void disableMeshForAreaElement(int elementTag);
        static const std::vector<std::vector<utility::Vector3>>& getNodeCoordinatesOfAreaElement(int elementTag);
        
        // Floor API
        static void addFloor(int floorNumber, double height);
        static void makeRigid(int floorNumber, int masterJointTag);
        static void makeFlexible(int floorNumber);

        // Building API
        static void includePDeltaEffects(bool includePDeltEffects);

        // Preprocessing
        static void updateAreaElementProperties();

        // Input File Generation
        static void createInputFile();
    };
}
