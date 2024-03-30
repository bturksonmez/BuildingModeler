#pragma once

#include "PhysicalModel/Building.h"
#include "OpenSeesWrapper/OpenseesModel.h"
#include "BuildingModelerExceptions.h"
#include "PhysicalModel/Load/LoadCase.h"

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
        static bool loadCaseExists(std::string loadCaseTag);
        static bool loadCombinationExists(std::string loadCombinationTag);

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
        static std::vector<int> getConstraintVectorFromAnalyticalModel(int jointTag);
        static std::optional<std::vector<int>> getConstraintVectorFromPhysicalModel(int jointTag);
        static std::vector<std::vector<int>> getConstraintVectorForNodesBetween(int jointTagA, int jointTagB);
        static utility::Vector3 getTranslationalMassForJointFromAnalyticalModel(int jointTag);
        static std::optional<utility::Vector3> getTranslationalMassForJointFromPhysicalModel(int jointTag);
        static utility::Vector3 getRotationalMassForJointFromAnalyticalModel(int jointTag);
        static std::optional<utility::Vector3> getRotationalMassForJointFromPhysicalModel(int jointTag);

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
        static double getLength(int elementTag);

        // Area Element API
        static void addShearWall(int elementTag, std::vector<int> jointTags, int sectionTag,
            physicalModel::AreaElementFormulation areaElementFormulation);
        static void addSlab(int elementTag, std::vector<int> jointTags, int sectionTag,
            physicalModel::AreaElementFormulation areaElementFormulation);
        static void meshAreaElement(int elementTag, std::optional<int> n1 = std::nullopt, std::optional<int> n2 = std::nullopt);
        static void disableMeshForAreaElement(int elementTag);
        static void disableSlabElements(bool disableSlabElements);
        static const std::vector<std::vector<utility::Vector3>>& getNodeCoordinatesOfAreaElement(int elementTag);
        static double getArea(int elementTag);
        
        // Floor API
        static void addFloor(int floorNumber, double height);
        static void makeRigid(int floorNumber, int masterJointTag);
        static void makeFlexible(int floorNumber);
        static void confineFloorMassOnDiaphragmNode(int floorNumber, bool confineFloorMassOnDiaphragmNode);
        static std::optional<utility::Vector3> getDiaphragmMass(int floorNumber);
        static std::optional<utility::Vector2> getMassCenter(int floorNumber);

        // Building API
        static void includePDeltaEffects(bool includePDeltEffects);

        // Loading API
        static void includeDeadLoadFromMembers(bool includeDeadLoadFromMembers);
        static void applyGravityLoadThroughLineElements(bool m_gravityThroughLineElements);
        static void setLiveLoadForFloor(int floorNumber, double liveLoadPerArea);
        static void addLoadCase(std::string loadCaseTag, physicalModel::LoadCaseType loadCaseType);
        static void addPointLoad(std::string loadCaseTag, int jointTag, double fx, double fy, double fz, double mx, double my, double mz);
        static void addDistributedLineLoad(std::string loadCaseTag, int elementTag, double wz, double wy, double wx);
        static void addDistributedAreaLoad(std::string loadCaseTag, int elementTag, double wz, double wy, double wx);
        static void addLoadCombination(std::string loadCombinationTag);
        static void addLoadCaseToLoadCombination(std::string loadCombinationTag, std::string loadCaseTag, double factor);
        static const std::vector<std::shared_ptr<physicalModel::Load>>& getPointLoads(std::string loadCaseTag);
        static const std::vector<std::shared_ptr<physicalModel::Load>>& getDistributedLineLoads(std::string loadCaseTag);
        static const std::vector<std::shared_ptr<physicalModel::Load>>& getDistributedAreaLoads(std::string loadCaseTag);

        // Preprocessing
        static void updateMassSourceFromMembers();
        static void updateAreaElementProperties();
        static void updateDeadAndLiveLoads();

        // Input File Generation
        static void createInputFile();
    };
}
