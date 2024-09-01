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
    
    EXPECT_NEAR(0.0023349, floor1DRX, epsilon);
    EXPECT_NEAR(0.0052545, floor2DRX, epsilon);
    EXPECT_NEAR(0.0026273, buildingDRX, epsilon);

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

TEST_F(StaticAnalysisTests, TwoStoryFrameWallStructureWithNx3Ny3) {
    typedef buildingModeler::BuildingModelerAPI api;

    // add joints
    // basement floor
    api::addJoint(0, { 0, 0, 0 });
    api::addJoint(1, { 3, 0, 0 });
    api::addJoint(2, { 5, 0, 0 });
    api::addJoint(3, { 8, 0, 0 });
    api::addJoint(4, { 0, 3, 0 });
    api::addJoint(5, { 3, 3, 0 });
    api::addJoint(6, { 5, 3, 0 });
    api::addJoint(7, { 8, 3, 0 });
    api::addJoint(8, { 0, 5, 0 });
    api::addJoint(9, { 3, 5, 0 });
    api::addJoint(10, { 5, 5, 0 });
    api::addJoint(11, { 8, 5, 0 });
    api::addJoint(12, { 0, 8, 0 });
    api::addJoint(13, { 3, 8, 0 });
    api::addJoint(14, { 5, 8, 0 });
    api::addJoint(15, { 8, 8, 0 });
    // first floor
    api::addJoint(16, { 0, 0, 3 });
    api::addJoint(17, { 3, 0, 3 });
    api::addJoint(18, { 5, 0, 3 });
    api::addJoint(19, { 8, 0, 3 });
    api::addJoint(20, { 0, 3, 3 });
    api::addJoint(21, { 3, 3, 3 });
    api::addJoint(22, { 5, 3, 3 });
    api::addJoint(23, { 8, 3, 3 });
    api::addJoint(24, { 0, 5, 3 });
    api::addJoint(25, { 3, 5, 3 });
    api::addJoint(26, { 5, 5, 3 });
    api::addJoint(27, { 8, 5, 3 });
    api::addJoint(28, { 0, 8, 3 });
    api::addJoint(29, { 3, 8, 3 });
    api::addJoint(30, { 5, 8, 3 });
    api::addJoint(31, { 8, 8, 3 });
    // second floor
    api::addJoint(32, { 0, 0, 6 });
    api::addJoint(33, { 3, 0, 6 });
    api::addJoint(34, { 5, 0, 6 });
    api::addJoint(35, { 8, 0, 6 });
    api::addJoint(36, { 0, 3, 6 });
    api::addJoint(37, { 3, 3, 6 });
    api::addJoint(38, { 5, 3, 6 });
    api::addJoint(39, { 8, 3, 6 });
    api::addJoint(40, { 0, 5, 6 });
    api::addJoint(41, { 3, 5, 6 });
    api::addJoint(42, { 5, 5, 6 });
    api::addJoint(43, { 8, 5, 6 });
    api::addJoint(44, { 0, 8, 6 });
    api::addJoint(45, { 3, 8, 6 });
    api::addJoint(46, { 5, 8, 6 });
    api::addJoint(47, { 8, 8, 6 });

    // add constraints
    api::setConstraintVector(0, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(1, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(2, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(3, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(4, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(5, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(6, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(7, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(8, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(9, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(10, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(11, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(12, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(13, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(14, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(15, { 1, 1, 1, 1, 1, 1 });

    // add floors
    api::addFloor(0, 0.0);
    api::addFloor(1, 3.0);
    api::addFloor(2, 6.0);

    // assign nodes to floor
    api::setFloorNo(0, 0);
    api::setFloorNo(1, 0);
    api::setFloorNo(2, 0);
    api::setFloorNo(3, 0);
    api::setFloorNo(4, 0);
    api::setFloorNo(5, 0);
    api::setFloorNo(6, 0);
    api::setFloorNo(7, 0);
    api::setFloorNo(8, 0);
    api::setFloorNo(9, 0);
    api::setFloorNo(10, 0);
    api::setFloorNo(11, 0);
    api::setFloorNo(12, 0);
    api::setFloorNo(13, 0);
    api::setFloorNo(14, 0);
    api::setFloorNo(15, 0);
    api::setFloorNo(16, 1);
    api::setFloorNo(17, 1);
    api::setFloorNo(18, 1);
    api::setFloorNo(19, 1);
    api::setFloorNo(20, 1);
    api::setFloorNo(21, 1);
    api::setFloorNo(22, 1);
    api::setFloorNo(23, 1);
    api::setFloorNo(24, 1);
    api::setFloorNo(25, 1);
    api::setFloorNo(26, 1);
    api::setFloorNo(27, 1);
    api::setFloorNo(28, 1);
    api::setFloorNo(29, 1);
    api::setFloorNo(30, 1);
    api::setFloorNo(31, 1);
    api::setFloorNo(32, 2);
    api::setFloorNo(33, 2);
    api::setFloorNo(34, 2);
    api::setFloorNo(35, 2);
    api::setFloorNo(36, 2);
    api::setFloorNo(37, 2);
    api::setFloorNo(38, 2);
    api::setFloorNo(39, 2);
    api::setFloorNo(40, 2);
    api::setFloorNo(41, 2);
    api::setFloorNo(42, 2);
    api::setFloorNo(43, 2);
    api::setFloorNo(44, 2);
    api::setFloorNo(45, 2);
    api::setFloorNo(46, 2);
    api::setFloorNo(47, 2);

    // add material
    api::addElasticMaterial(1, 30000000, 12500000, 2.4);

    // add section 1D
    api::addElasticSection1D(1, 1, 0.09, 0.000675, 0.000675, 0.00135);

    // add section 2D
    api::addElasticSection2D(2, 1, 0.15);
    api::addElasticSection2D(3, 1, 0.25);

    // add beam elements
    api::addBeam(101, { 16, 17 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(102, { 18, 19 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(103, { 20, 21 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(104, { 21, 22 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(105, { 22, 23 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(106, { 24, 25 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(107, { 25, 26 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(108, { 26, 27 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(109, { 28, 29 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(110, { 30, 31 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(111, { 16, 20 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(112, { 17, 21 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(113, { 18, 22 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(114, { 19, 23 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(115, { 21, 25 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(116, { 22, 26 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(117, { 24, 28 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(118, { 25, 29 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(119, { 26, 30 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(120, { 27, 31 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(201, { 32, 33 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(202, { 34, 35 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(203, { 36, 37 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(204, { 37, 38 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(205, { 38, 39 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(206, { 40, 41 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(207, { 41, 42 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(208, { 42, 43 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(209, { 44, 45 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(210, { 46, 47 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(211, { 32, 36 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(212, { 33, 37 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(213, { 34, 38 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(214, { 35, 39 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(215, { 37, 41 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(216, { 38, 42 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(217, { 40, 44 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(218, { 41, 45 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(219, { 42, 46 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(220, { 43, 47 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);

    // add beam segments
    api::setSegmentRatios(101, { 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 });
    api::setSegmentRatios(102, { 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 });
    api::setSegmentRatios(103, { 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 });
    api::setSegmentRatios(104, { 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 });
    api::setSegmentRatios(105, { 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 });
    api::setSegmentRatios(106, { 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 });
    api::setSegmentRatios(107, { 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 });
    api::setSegmentRatios(108, { 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 });
    api::setSegmentRatios(109, { 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 });
    api::setSegmentRatios(110, { 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 });
    api::setSegmentRatios(111, { 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 });
    api::setSegmentRatios(112, { 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 });
    api::setSegmentRatios(113, { 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 });
    api::setSegmentRatios(114, { 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 });
    api::setSegmentRatios(115, { 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 });
    api::setSegmentRatios(116, { 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 });
    api::setSegmentRatios(117, { 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 });
    api::setSegmentRatios(118, { 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 });
    api::setSegmentRatios(119, { 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 });
    api::setSegmentRatios(120, { 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 });
    api::setSegmentRatios(201, { 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 });
    api::setSegmentRatios(202, { 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 });
    api::setSegmentRatios(203, { 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 });
    api::setSegmentRatios(204, { 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 });
    api::setSegmentRatios(205, { 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 });
    api::setSegmentRatios(206, { 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 });
    api::setSegmentRatios(207, { 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 });
    api::setSegmentRatios(208, { 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 });
    api::setSegmentRatios(209, { 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 });
    api::setSegmentRatios(210, { 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 });
    api::setSegmentRatios(211, { 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 });
    api::setSegmentRatios(212, { 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 });
    api::setSegmentRatios(213, { 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 });
    api::setSegmentRatios(214, { 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 });
    api::setSegmentRatios(215, { 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 });
    api::setSegmentRatios(216, { 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 });
    api::setSegmentRatios(217, { 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 });
    api::setSegmentRatios(218, { 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 });
    api::setSegmentRatios(219, { 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 });
    api::setSegmentRatios(220, { 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 });

    // add column elements
    api::addColumn(1001, { 0, 16 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(1002, { 3, 19 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(1003, { 5, 21 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(1004, { 6, 22 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(1005, { 9, 25 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(1006, { 10, 26 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(1007, { 12, 28 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(1008, { 15, 31 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(2001, { 16, 32 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(2002, { 19, 35 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(2003, { 21, 37 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(2004, { 22, 38 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(2005, { 25, 41 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(2006, { 26, 42 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(2007, { 28, 44 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(2008, { 31, 47 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);

    // add column section modifiers
    api::setSectionModifiers(1001, 0, 1.0, 0.7, 0.7, 1.0);
    api::setSectionModifiers(1002, 0, 1.0, 0.7, 0.7, 1.0);
    api::setSectionModifiers(1003, 0, 1.0, 0.7, 0.7, 1.0);
    api::setSectionModifiers(1004, 0, 1.0, 0.7, 0.7, 1.0);
    api::setSectionModifiers(1005, 0, 1.0, 0.7, 0.7, 1.0);
    api::setSectionModifiers(1006, 0, 1.0, 0.7, 0.7, 1.0);
    api::setSectionModifiers(1007, 0, 1.0, 0.7, 0.7, 1.0);
    api::setSectionModifiers(1008, 0, 1.0, 0.7, 0.7, 1.0);
    api::setSectionModifiers(2001, 0, 1.0, 0.7, 0.7, 1.0);
    api::setSectionModifiers(2002, 0, 1.0, 0.7, 0.7, 1.0);
    api::setSectionModifiers(2003, 0, 1.0, 0.7, 0.7, 1.0);
    api::setSectionModifiers(2004, 0, 1.0, 0.7, 0.7, 1.0);
    api::setSectionModifiers(2005, 0, 1.0, 0.7, 0.7, 1.0);
    api::setSectionModifiers(2006, 0, 1.0, 0.7, 0.7, 1.0);
    api::setSectionModifiers(2007, 0, 1.0, 0.7, 0.7, 1.0);
    api::setSectionModifiers(2008, 0, 1.0, 0.7, 0.7, 1.0);

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

    EXPECT_NEAR(0.0023349, floor1DRX, epsilon);
    EXPECT_NEAR(0.0052545, floor2DRX, epsilon);
    EXPECT_NEAR(0.0026273, buildingDRX, epsilon);

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

TEST_F(StaticAnalysisTests, SingleShearWallX) {
    typedef buildingModeler::BuildingModelerAPI api;

    // add joints
    api::addJoint(0, { 0, 0, 0 });
    api::addJoint(1, { 2, 0, 0 });
    api::addJoint(2, { 2, 0, 4 });
    api::addJoint(3, { 0, 0, 4 });

    // add constraints
    api::setConstraintVector(0, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(1, { 1, 1, 1, 1, 1, 1 });

    // add floors
    api::addFloor(0, 0.0);
    api::addFloor(1, 4.0);

    // assign nodes to floor
    api::setFloorNo(0, 0);
    api::setFloorNo(1, 0);
    api::setFloorNo(2, 1);
    api::setFloorNo(3, 1);

    // add material
    api::addElasticMaterial(1, 30000000, 12500000, 2.4);

    // add section 2D
    api::addElasticSection2D(1, 1, 0.3);

    // add shear wall element
    api::addShearWall(1001, { 0, 1, 2, 3 }, 1, physicalModel::AreaElementFormulation::LINEAR);

    //mesh the element
    api::meshAreaElement(1001, 4, 8);

    // add gravity and live loads
    api::includeDeadLoadFromMembers(true);
    api::applyGravityLoadThroughLineElements(false);
    api::updateDeadAndLiveLoads();

    // add earthquake loads
    api::addStaticLoadCase("eq", physicalModel::StaticLoadCaseType::EARTHQUAKE);
    api::addPointLoad("eq", 3, 300, 0, 0, 0, 0, 0);

    // add load combination
    api::addStaticLoadCombination("combo1");
    api::addLoadCaseToStaticLoadCombination("combo1", "dead", 1.0);
    api::addLoadCaseToStaticLoadCombination("combo1", "eq", 1.0);
    api::setStaticLoadCombinationActive("combo1", true);

    // create analytical model and tcl file
    api::createAnalyticalModel();
    api::createModelAndLoadingFiles();
    api::analyze();

    // floating-point comparison tolerance
    const double epsilon = 1e-2;
    const double epsilonSmall = 1e-6;

    double axialForce = api::getShearWallForceZ(1001, "combo1");
    double shearForce = api::getShearWallForceX(1001, "combo1");
    double moment = api::getShearWallMomentYY(1001, "combo1");
    double bottomCR = api::getShearWallCR(1001, "combo1", 5);
    double bottomDR = api::getShearWallDR(1001, "combo1", 1);

    double topDispJoint2 = api::getTranslationalDispX(2, "combo1");
    double topDispJoint3 = api::getTranslationalDispX(3, "combo1");
    double expectedBottomCR = (topDispJoint2 + topDispJoint3) / 2.0 / 4.0;

    EXPECT_NEAR(52.974, axialForce, epsilon);
    EXPECT_NEAR(-300, shearForce, epsilon);
    EXPECT_NEAR(-1200, moment, epsilon);
    EXPECT_NEAR(expectedBottomCR, bottomCR, epsilonSmall);
    EXPECT_NEAR(bottomDR, bottomCR, epsilonSmall);
}

TEST_F(StaticAnalysisTests, SingleShearWallY) {
    typedef buildingModeler::BuildingModelerAPI api;

    // add joints
    api::addJoint(0, { 0, 2, 0 });
    api::addJoint(1, { 0, 0, 0 });
    api::addJoint(2, { 0, 0, 4 });
    api::addJoint(3, { 0, 2, 4 });

    // add constraints
    api::setConstraintVector(0, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(1, { 1, 1, 1, 1, 1, 1 });

    // add floors
    api::addFloor(0, 0.0);
    api::addFloor(1, 4.0);

    // assign nodes to floor
    api::setFloorNo(0, 0);
    api::setFloorNo(1, 0);
    api::setFloorNo(2, 1);
    api::setFloorNo(3, 1);

    // add material
    api::addElasticMaterial(1, 30000000, 12500000, 2.4);

    // add section 2D
    api::addElasticSection2D(1, 1, 0.3);

    // add shear wall element
    api::addShearWall(1001, { 0, 1, 2, 3 }, 1, physicalModel::AreaElementFormulation::LINEAR);

    //mesh the element
    api::meshAreaElement(1001, 4, 8);

    // add gravity and live loads
    api::includeDeadLoadFromMembers(true);
    api::applyGravityLoadThroughLineElements(false);
    api::updateDeadAndLiveLoads();

    // add earthquake loads
    api::addStaticLoadCase("eq", physicalModel::StaticLoadCaseType::EARTHQUAKE);
    api::addPointLoad("eq", 3, 0, 300, 0, 0, 0, 0);

    // add load combination
    api::addStaticLoadCombination("combo1");
    api::addLoadCaseToStaticLoadCombination("combo1", "dead", 1.0);
    api::addLoadCaseToStaticLoadCombination("combo1", "eq", 1.0);
    api::setStaticLoadCombinationActive("combo1", true);

    // create analytical model and tcl file
    api::createAnalyticalModel();
    api::createModelAndLoadingFiles();
    api::analyze();

    // floating-point comparison tolerance
    const double epsilon = 1e-2;
    const double epsilonSmall = 1e-6;

    double axialForce = api::getShearWallForceZ(1001, "combo1");
    double shearForce = api::getShearWallForceY(1001, "combo1");
    double moment = api::getShearWallMomentXX(1001, "combo1");
    double bottomCR = api::getShearWallCR(1001, "combo1", 4);
    double bottomDR = api::getShearWallDR(1001, "combo1", 2);

    double topDispJoint2 = api::getTranslationalDispY(2, "combo1");
    double topDispJoint3 = api::getTranslationalDispY(3, "combo1");
    double expectedBottomCR = (topDispJoint2 + topDispJoint3) / 2.0 / 4.0;

    EXPECT_NEAR(52.974, axialForce, epsilon);
    EXPECT_NEAR(-300, shearForce, epsilon);
    EXPECT_NEAR(1200, moment, epsilon);
    EXPECT_NEAR(expectedBottomCR, bottomCR, epsilonSmall);
    EXPECT_NEAR(bottomDR, bottomCR, epsilonSmall);
}