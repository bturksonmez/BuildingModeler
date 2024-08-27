#include <gtest/gtest.h>
#include "../BuildingModeler/BuildingModelerAPI.h"
#include "../BuildingModeler/Utilities/Vector3.h"

class StaticAnalysisTests : public ::testing::Test {
protected:
    void TearDown() override {
        typedef buildingModeler::BuildingModelerAPI api;

        api::clear();
    }
};

TEST_F(StaticAnalysisTests, TwoStoryFrameStructureWithNx1Ny1) {
    typedef buildingModeler::BuildingModelerAPI api;

    // add joints
    api::addJoint(0, { 0, 0, 0 });
    api::addJoint(1, { 2, 0, 0 });
    api::addJoint(2, { 2, 2, 0 });
    api::addJoint(3, { 0, 2, 0 });
    api::addJoint(4, { 0, 0, 2 });
    api::addJoint(5, { 2, 0, 2 });
    api::addJoint(6, { 2, 2, 2 });
    api::addJoint(7, { 0, 2, 2 });
    api::addJoint(8, { 0, 0, 4 });
    api::addJoint(9, { 2, 0, 4 });
    api::addJoint(10, { 2, 2, 4 });
    api::addJoint(11, { 0, 2, 4 });

    // add constraints
    api::setConstraintVector(0, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(1, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(2, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(3, { 1, 1, 1, 1, 1, 1 });

    // add floors
    api::addFloor(0, 0.0);
    api::addFloor(1, 2.0);
    api::addFloor(2, 4.0);

    // assign nodes to floor
    api::setFloorNo(0, 0);
    api::setFloorNo(1, 0);
    api::setFloorNo(2, 0);
    api::setFloorNo(3, 0);
    api::setFloorNo(4, 1);
    api::setFloorNo(5, 1);
    api::setFloorNo(6, 1);
    api::setFloorNo(7, 1);
    api::setFloorNo(8, 2);
    api::setFloorNo(9, 2);
    api::setFloorNo(10, 2);
    api::setFloorNo(11, 2);

    // add material
    api::addElasticMaterial(1, 30000000, 12500000, 2.4);

    // add section 1D
    api::addElasticSection1D(1, 1, 0.2025, 0.0034171875, 0.0034171875, 0.006834375);

    // add section 2D
    api::addElasticSection2D(2, 1, 0.12);

    // add beam elements
    api::addBeam(11, { 4, 5 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(12, { 5, 6 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(13, { 6, 7 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(14, { 7, 4 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(21, { 8, 9 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(22, { 9, 10 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(23, { 10, 11 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(24, { 11, 8 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);

    // add beam segments
    api::setSegmentRatios(11, { 0.5, 0.5 });
    api::setSegmentRatios(12, { 0.5, 0.5 });
    api::setSegmentRatios(13, { 0.5, 0.5 });
    api::setSegmentRatios(14, { 0.5, 0.5 });
    api::setSegmentRatios(21, { 0.5, 0.5 });
    api::setSegmentRatios(22, { 0.5, 0.5 });
    api::setSegmentRatios(23, { 0.5, 0.5 });
    api::setSegmentRatios(24, { 0.5, 0.5 });

    // add beam section modifiers
    api::setSectionModifiers(11, 0, 1.0, 0.3, 0.3, 1.0);
    api::setSectionModifiers(11, 1, 1.0, 0.3, 0.3, 1.0);
    api::setSectionModifiers(12, 0, 1.0, 0.3, 0.3, 1.0);
    api::setSectionModifiers(12, 1, 1.0, 0.3, 0.3, 1.0);
    api::setSectionModifiers(13, 0, 1.0, 0.3, 0.3, 1.0);
    api::setSectionModifiers(13, 1, 1.0, 0.3, 0.3, 1.0);
    api::setSectionModifiers(14, 0, 1.0, 0.3, 0.3, 1.0);
    api::setSectionModifiers(14, 1, 1.0, 0.3, 0.3, 1.0);
    api::setSectionModifiers(21, 0, 1.0, 0.3, 0.3, 1.0);
    api::setSectionModifiers(21, 1, 1.0, 0.3, 0.3, 1.0);
    api::setSectionModifiers(22, 0, 1.0, 0.3, 0.3, 1.0);
    api::setSectionModifiers(22, 1, 1.0, 0.3, 0.3, 1.0);
    api::setSectionModifiers(23, 0, 1.0, 0.3, 0.3, 1.0);
    api::setSectionModifiers(23, 1, 1.0, 0.3, 0.3, 1.0);
    api::setSectionModifiers(24, 0, 1.0, 0.3, 0.3, 1.0);
    api::setSectionModifiers(24, 1, 1.0, 0.3, 0.3, 1.0);

    // add column elements
    api::addColumn(101, { 0, 4 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(102, { 1, 5 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(103, { 2, 6 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(104, { 3, 7 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(201, { 4, 8 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(202, { 5, 9 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(203, { 6, 10 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(204, { 7, 11 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);

    // add column section modifiers
    api::setSectionModifiers(101, 0, 1.0, 0.7, 0.7, 1.0);
    api::setSectionModifiers(102, 0, 1.0, 0.7, 0.7, 1.0);
    api::setSectionModifiers(103, 0, 1.0, 0.7, 0.7, 1.0);
    api::setSectionModifiers(104, 0, 1.0, 0.7, 0.7, 1.0);
    api::setSectionModifiers(201, 0, 1.0, 0.7, 0.7, 1.0);
    api::setSectionModifiers(202, 0, 1.0, 0.7, 0.7, 1.0);
    api::setSectionModifiers(203, 0, 1.0, 0.7, 0.7, 1.0);
    api::setSectionModifiers(204, 0, 1.0, 0.7, 0.7, 1.0);

    // add slab elements
    api::addSlab(1001, { 4, 5, 6, 7 }, 2, physicalModel::AreaElementFormulation::LINEAR);
    api::addSlab(2001, { 8, 9, 10, 11 }, 2, physicalModel::AreaElementFormulation::LINEAR);

    // update connectivity between line and area elements
    api::updateAreaElementProperties();

    //mesh the element
    api::meshAreaElement(1001);
    api::meshAreaElement(2001);

    // make rigid
    api::updateMassSourceFromMembers();
    api::makeRigid(1, 1000);
    api::makeRigid(2, 2000);
    api::confineFloorMassOnDiaphragmNode(1, true);
    api::confineFloorMassOnDiaphragmNode(2, true);

    // add modal analysis
    api::addModalLoadCase("modal", 3);
    api::setLoadCaseActive("modal", true);

    // add gravity and live loads
    api::includeDeadLoadFromMembers(true);
    //api::applyGravityLoadThroughLineElements(false);
    api::setLiveLoadForFloor(1, 2);
    api::setLiveLoadForFloor(2, 2);
    api::updateDeadAndLiveLoads();

    // add earthquake loads
    api::addStaticLoadCase("eq", physicalModel::StaticLoadCaseType::EARTHQUAKE);
    api::addPointLoad("eq", 1000, 300, 0, 0, 0, 0, 0);
    api::addPointLoad("eq", 2000, 600, 0, 0, 0, 0, 0);

    // add load combination
    api::addStaticLoadCombination("combo1");
    api::addLoadCaseToStaticLoadCombination("combo1", "dead", 1.0);
    api::addLoadCaseToStaticLoadCombination("combo1", "live", 0.3);
    api::addLoadCaseToStaticLoadCombination("combo1", "eq", 1.0);
    api::setStaticLoadCombinationActive("combo1", true);

    // create analytical model and tcl file
    api::createAnalyticalModel();
    api::createModelAndLoadingFiles();
    api::analyze();

    // floating-point comparison tolerance
    const double epsilon = 1e-3;

    // get floor DRs and building DR
    double floor1DRX = api::getFloorDR(1, "combo1", 1);
    double floor2DRX = api::getFloorDR(2, "combo1", 1);
    double buildingDRX = api::getBuildingDR("combo1", 1);
    
    //EXPECT_NEAR(0.0023349, floor1DRX, epsilon);
    //EXPECT_NEAR(0.0052545, floor2DRX, epsilon);
    //EXPECT_NEAR(0.0026273, buildingDRX, epsilon);

    // get axial forces for bottom columns
    double axialForce101 = api::getLineElementForceZ(101, "combo1", 0, true);
    double axialForce102 = api::getLineElementForceZ(102, "combo1", 0, true);
    double axialForce103 = api::getLineElementForceZ(103, "combo1", 0, true);
    double axialForce104 = api::getLineElementForceZ(104, "combo1", 0, true);
    double totalAxialForce = axialForce101 + axialForce102 + axialForce103 + axialForce104;

    EXPECT_NEAR(160.897, totalAxialForce, epsilon);

    // get shear forces for bottom columns in X direction
    double shearForce101 = api::getLineElementForceX(101, "combo1", 0, true);
    double shearForce102 = api::getLineElementForceX(102, "combo1", 0, true);
    double shearForce103 = api::getLineElementForceX(103, "combo1", 0, true);
    double shearForce104 = api::getLineElementForceX(104, "combo1", 0, true);
    double totalShearForce = shearForce101 + shearForce102 + shearForce103 + shearForce104;

    EXPECT_NEAR(-900.0, totalShearForce, epsilon);
}