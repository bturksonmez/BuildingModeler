#pragma once

#include "PhysicalModel/Building.h"
#include "OpenSeesWrapper/OpenseesModel.h"
#include "BuildingModelerExceptions.h"
#include "PhysicalModel/Load/LoadCase.h"

// To do export class is to be written for dll

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
        static bool staticLoadCombinationExists(std::string staticLoadCombinationTag);

        static void invalidateAreaMeshAlongLineElement(int elementTag);
        static bool checkIfJointsCoplanar(const std::vector<utility::Vector3>& joints);
        static bool checkIfJointsCounterClockwise(const std::vector<utility::Vector3>& joints);
        static bool checkIfQuadConvex(const std::vector<utility::Vector3>& joints);
        static bool checkIfShearWallVertical(const std::vector<utility::Vector3>& joints);

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
        static utility::Vector3 getCoordinates(int jointTag);
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
        static void addElasticSection1D(int sectionTag, int materialTag, physicalModel::Shape* shape);
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
        static double getArea(int elementTag, int segmentNo = 0);
        static double getMomentOfInertiaYY(int elementTag, int segmentNo = 0);
        static double getMomentOfInertiaZZ(int elementTag, int segmentNo = 0);
        static double getTorsionalConstant(int elementTag, int segmentNo = 0);

        // Area Element API
        static void addShearWall(int elementTag, std::vector<int> jointTags, int sectionTag,
            physicalModel::AreaElementFormulation areaElementFormulation);
        static void addSlab(int elementTag, std::vector<int> jointTags, int sectionTag,
            physicalModel::AreaElementFormulation areaElementFormulation);
        static void meshAreaElement(int elementTag, std::optional<int> n1 = std::nullopt, std::optional<int> n2 = std::nullopt);
        static void disableMeshForAreaElement(int elementTag);
        static void disableSlabElements(bool disableSlabElements);
        static const std::vector<int>& getJointTags(int elementTag);
        static const std::vector<std::vector<utility::Vector3>>& getNodeCoordinatesOfAreaElement(int elementTag);
        static double getSurfaceArea(int elementTag);
        static std::vector<int> getShearWallElementTags();
        static std::vector<int> getSlabElementTags();
        
        // Floor API
        static void addFloor(int floorNumber, double height);
        static void makeRigid(int floorNumber, int masterJointTag);
        static void makeFlexible(int floorNumber);
        static void confineFloorMassOnDiaphragmNode(int floorNumber, bool confineFloorMassOnDiaphragmNode);
        static std::optional<utility::Vector3> getDiaphragmMass(int floorNumber);
        static std::optional<utility::Vector2> getMassCenter(int floorNumber);

        // Building API
        static void includePDeltaEffects(bool includePDeltEffects);
        static double getBuildingWeight();
        static void clear();
        static void clearAnalyticalModel();

        // Loading API
        static void includeDeadLoadFromMembers(bool includeDeadLoadFromMembers);
        static void applyGravityLoadThroughLineElements(bool gravityThroughLineElements);
        static void setLiveLoadForFloor(int floorNumber, double liveLoadPerArea);
        static void setLiveLoadMassContributionForFloor(int floorNumber, double liveLoadMassContributionFactor);
        static void addStaticLoadCase(std::string loadCaseTag, physicalModel::StaticLoadCaseType loadCaseType);
        static void addModalLoadCase(std::string loadCaseTag, size_t numberOfModes);
        static void addPointLoad(std::string loadCaseTag, int jointTag, double fx, double fy, double fz, double mx, double my, double mz);
        static void addDistributedLineLoad(std::string loadCaseTag, int elementTag, double wz, double wy, double wx);
        static void addDistributedAreaLoad(std::string loadCaseTag, int elementTag, double wz, double wy, double wx);
        static void addStaticLoadCombination(std::string staticLoadCombinationTag);
        static void addLoadCaseToStaticLoadCombination(std::string staticLoadCombinationTag, std::string loadCaseTag, double factor);
        static void setLoadCaseActive(std::string loadCaseTag, bool active);
        static void setStaticLoadCombinationActive(std::string staticLoadCombinationTag, bool active);
        static const std::vector<std::shared_ptr<physicalModel::Load>>& getPointLoads(std::string loadCaseTag);
        static const std::vector<std::shared_ptr<physicalModel::Load>>& getDistributedLineLoads(std::string loadCaseTag);
        static const std::vector<std::shared_ptr<physicalModel::Load>>& getDistributedAreaLoads(std::string loadCaseTag);
        static const std::vector<std::shared_ptr<opensees::Load>>& getLoadsFromAnalyticalModel(std::string loadPatternTag);

        // Preprocessing
        static void updateMassSourceFromMembers();
        static void updateAreaElementProperties();
        static void updateDeadAndLiveLoads();

        // Postprocessing
        // displacement
        static std::vector<double> getDisplacements(int jointTag, std::string analysisTag, size_t timeStep = 0);
        static double getFloorDR(int floorNumber, std::string analysisTag, size_t dof, size_t timeStep = 0);
        static double getBuildingDR(std::string analysisTag, size_t dof, size_t timeStep = 0);
        static double getTranslationalDispX(int jointTag, std::string analysisTag, size_t timeStep = 0);
        static double getTranslationalDispY(int jointTag, std::string analysisTag, size_t timeStep = 0);
        static double getTranslationalDispZ(int jointTag, std::string analysisTag, size_t timeStep = 0);
        static double getRotationalDispX(int jointTag, std::string analysisTag, size_t timeStep = 0);
        static double getRotationalDispY(int jointTag, std::string analysisTag, size_t timeStep = 0);
        static double getRotationalDispZ(int jointTag, std::string analysisTag, size_t timeStep = 0);

        // line element
        static double getLineElementForceX(int elementTag, std::string analysisTag, size_t timeStep = 0, bool atIJoint = true);
        static double getLineElementForceY(int elementTag, std::string analysisTag, size_t timeStep = 0, bool atIJoint = true);
        static double getLineElementForceZ(int elementTag, std::string analysisTag, size_t timeStep = 0, bool atIJoint = true);
        static double getLineElementMomentXX(int elementTag, std::string analysisTag, size_t timeStep = 0, bool atIJoint = true);
        static double getLineElementMomentYY(int elementTag, std::string analysisTag, size_t timeStep = 0, bool atIJoint = true);
        static double getLineElementMomentZZ(int elementTag, std::string analysisTag, size_t timeStep = 0, bool atIJoint = true);
        static double getLineElementDR(int elementTag, std::string analysisTag, size_t dof, size_t timeStep = 0);
        static double getLineElementCR(int elementTag, std::string analysisTag, size_t dofRot, size_t timeStep = 0, bool fromIJoint = true);
        static double getLineElementDisplacement(int elementTag, size_t segmentNode, std::string analysisTag, size_t dof, size_t timeStep = 0);

        // shear wall
        static double getShearWallForceX(int elementTag, std::string analysisTag, size_t timeStep = 0, bool atBottom = true);
        static double getShearWallForceY(int elementTag, std::string analysisTag, size_t timeStep = 0, bool atBottom = true);
        static double getShearWallForceZ(int elementTag, std::string analysisTag, size_t timeStep = 0, bool atBottom = true);
        static double getShearWallMomentXX(int elementTag, std::string analysisTag, size_t timeStep = 0, bool atBottom = true);
        static double getShearWallMomentYY(int elementTag, std::string analysisTag, size_t timeStep = 0, bool atBottom = true);
        static double getShearWallMomentZZ(int elementTag, std::string analysisTag, size_t timeStep = 0, bool atBottom  = true);
        static double getShearWallDR(int elementTag, std::string analysisTag, size_t dof, size_t timeStep = 0);
        static double getShearWallCR(int elementTag, std::string analysisTag, size_t dofRot, size_t timeStep = 0, bool fromBottom = true);

        // modal output
        static double getPeriod(std::string analysisTag, int modeNumber);
        static double getFundamentalPeriod(std::string analysisTag, size_t dof);

        // Input File Generation
        static void createAnalyticalModel();
        static void createModelAndLoadingFiles();
        static void analyze();
    };
}
