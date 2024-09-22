#include <gtest/gtest.h>
#include "../BuildingModeler/BuildingModelerAPI.h"
#include "../BuildingModeler/Utilities/Vector3.h"

class LoadTests : public ::testing::Test {
protected:
    void TearDown() override {
        typedef buildingModeler::BuildingModelerAPI api;

        api::clear();
    }
};

TEST_F(LoadTests, SelfWeightWithFramesOnlySuccess) {
    typedef buildingModeler::BuildingModelerAPI api;

    // add joints
    api::addJoint(1, { 0, 0, 0 });
    api::addJoint(2, { 3, 0, 0 });
    api::addJoint(3, { 6, 0, 0 });
    api::addJoint(4, { 0, 3, 0 });
    api::addJoint(5, { 3, 3, 0 });
    api::addJoint(6, { 6, 3, 0 });
    api::addJoint(7, { 0, 6, 0 });
    api::addJoint(8, { 3, 6, 0 });
    api::addJoint(9, { 6, 6, 0 });
    api::addJoint(10, { 0, 0, 3 });
    api::addJoint(11, { 3, 0, 3 });
    api::addJoint(12, { 6, 0, 3 });
    api::addJoint(13, { 0, 3, 3 });
    api::addJoint(14, { 3, 3, 3 });
    api::addJoint(15, { 6, 3, 3 });
    api::addJoint(16, { 0, 6, 3 });
    api::addJoint(17, { 3, 6, 3 });
    api::addJoint(18, { 6, 6, 3 });
    api::addJoint(19, { 0, 0, 6 });
    api::addJoint(20, { 3, 0, 6 });
    api::addJoint(21, { 6, 0, 6 });
    api::addJoint(22, { 0, 3, 6 });
    api::addJoint(23, { 3, 3, 6 });
    api::addJoint(24, { 6, 3, 6 });
    api::addJoint(25, { 0, 6, 6 });
    api::addJoint(26, { 3, 6, 6 });
    api::addJoint(27, { 6, 6, 6 });

    // set constraints
    api::setConstraintVector(1, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(2, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(3, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(4, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(5, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(6, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(7, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(8, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(9, { 1, 1, 1, 1, 1, 1 });

    // add floors
    api::addFloor(0, 0.0);
    api::addFloor(1, 3.0);
    api::addFloor(2, 6.0);

    // assign nodes to floor
    api::setFloorNo(1, 0);
    api::setFloorNo(2, 0);
    api::setFloorNo(3, 0);
    api::setFloorNo(4, 0);
    api::setFloorNo(5, 0);
    api::setFloorNo(6, 0);
    api::setFloorNo(7, 0);
    api::setFloorNo(8, 0);
    api::setFloorNo(9, 0);
    api::setFloorNo(10, 1);
    api::setFloorNo(11, 1);
    api::setFloorNo(12, 1);
    api::setFloorNo(13, 1);
    api::setFloorNo(14, 1);
    api::setFloorNo(15, 1);
    api::setFloorNo(16, 1);
    api::setFloorNo(17, 1);
    api::setFloorNo(18, 1);
    api::setFloorNo(19, 2);
    api::setFloorNo(20, 2);
    api::setFloorNo(21, 2);
    api::setFloorNo(22, 2);
    api::setFloorNo(23, 2);
    api::setFloorNo(24, 2);
    api::setFloorNo(25, 2);
    api::setFloorNo(26, 2);
    api::setFloorNo(27, 2);

    // add material
    api::addElasticMaterial(1, 20, 20, 2.4);

    // add section 1D
    api::addElasticSection1D(1, 1, new physicalModel::ArbitraryShape(0.25, 0.02, 0.02, 0.04));

    // add beam elements
    api::addBeam(1, { 10, 11 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(2, { 11, 12 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(3, { 13, 14 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(4, { 14, 15 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(5, { 16, 17 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(6, { 17, 18 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(7, { 10, 13 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(8, { 13, 16 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(9, { 11, 14 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(10, { 14, 17 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(11, { 12, 15 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(12, { 15, 18 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(13, { 19, 20 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(14, { 20, 21 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(15, { 22, 23 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(16, { 23, 24 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(17, { 25, 26 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(18, { 26, 27 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(19, { 19, 22 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(20, { 22, 25 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(21, { 20, 23 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(22, { 23, 26 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(23, { 21, 24 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(24, { 24, 27 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);

    // add column elements
    api::addColumn(101, { 1, 10 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(102, { 2, 11 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(103, { 3, 12 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(104, { 4, 13 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(105, { 5, 14 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(106, { 6, 15 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(107, { 7, 16 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(108, { 8, 17 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(109, { 9, 18 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(110, { 10, 19 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(111, { 11, 20 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(112, { 12, 21 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(113, { 13, 22 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(114, { 14, 23 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(115, { 15, 24 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(116, { 16, 25 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(117, { 17, 26 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(118, { 18, 27 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);

    // loading
    api::includeDeadLoadFromMembers(true);
    api::updateDeadAndLiveLoads();

    // floating-point comparison tolerance
    const double epsilon = 1e-6;

    // retrieve loads from load cases
    auto pointLoads = api::getPointLoads("dead");
    auto lineLoads = api::getDistributedLineLoads("dead");
    auto areaLoads = api::getDistributedAreaLoads("dead");
    ASSERT_EQ(0, areaLoads.size());

    // calculate total weight from members
    auto totalWeight = 0.0;

    for (auto pointLoad : pointLoads) {
        totalWeight += pointLoad->getLoadVector()[2];
    }

    for (auto lineLoad : lineLoads) {
        auto load1 = lineLoad->getLoadVector()[1] * api::getLength(std::dynamic_pointer_cast<physicalModel::DistributedLineLoad>(lineLoad)->getBeamElementTag());
        auto load2 = lineLoad->getLoadVector()[2] * api::getLength(std::dynamic_pointer_cast<physicalModel::DistributedLineLoad>(lineLoad)->getBeamElementTag());
        totalWeight += load1;
        totalWeight += load2;
    }

    auto expectedTotalWeight = -741.636;

    EXPECT_NEAR(expectedTotalWeight, totalWeight, epsilon);
}

TEST_F(LoadTests, SelfWeightAndLiveLoadSuccess) {
    typedef buildingModeler::BuildingModelerAPI api;

    // add joints
    api::addJoint(1, { 0, 0, 0 });
    api::addJoint(2, { 3, 0, 0 });
    api::addJoint(3, { 6, 0, 0 });
    api::addJoint(4, { 0, 3, 0 });
    api::addJoint(5, { 3, 3, 0 });
    api::addJoint(6, { 6, 3, 0 });
    api::addJoint(7, { 0, 6, 0 });
    api::addJoint(8, { 3, 6, 0 });
    api::addJoint(9, { 6, 6, 0 });
    api::addJoint(10, { 0, 0, 3 });
    api::addJoint(11, { 3, 0, 3 });
    api::addJoint(12, { 6, 0, 3 });
    api::addJoint(13, { 0, 3, 3 });
    api::addJoint(14, { 3, 3, 3 });
    api::addJoint(15, { 6, 3, 3 });
    api::addJoint(16, { 0, 6, 3 });
    api::addJoint(17, { 3, 6, 3 });
    api::addJoint(18, { 6, 6, 3 });
    api::addJoint(19, { 0, 0, 6 });
    api::addJoint(20, { 3, 0, 6 });
    api::addJoint(21, { 6, 0, 6 });
    api::addJoint(22, { 0, 3, 6 });
    api::addJoint(23, { 3, 3, 6 });
    api::addJoint(24, { 6, 3, 6 });
    api::addJoint(25, { 0, 6, 6 });
    api::addJoint(26, { 3, 6, 6 });
    api::addJoint(27, { 6, 6, 6 });

    // set constraints
    api::setConstraintVector(1, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(2, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(3, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(4, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(5, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(6, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(7, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(8, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(9, { 1, 1, 1, 1, 1, 1 });

    // add floors
    api::addFloor(0, 0.0);
    api::addFloor(1, 3.0);
    api::addFloor(2, 6.0);

    // assign nodes to floor
    api::setFloorNo(1, 0);
    api::setFloorNo(2, 0);
    api::setFloorNo(3, 0);
    api::setFloorNo(4, 0);
    api::setFloorNo(5, 0);
    api::setFloorNo(6, 0);
    api::setFloorNo(7, 0);
    api::setFloorNo(8, 0);
    api::setFloorNo(9, 0);
    api::setFloorNo(10, 1);
    api::setFloorNo(11, 1);
    api::setFloorNo(12, 1);
    api::setFloorNo(13, 1);
    api::setFloorNo(14, 1);
    api::setFloorNo(15, 1);
    api::setFloorNo(16, 1);
    api::setFloorNo(17, 1);
    api::setFloorNo(18, 1);
    api::setFloorNo(19, 2);
    api::setFloorNo(20, 2);
    api::setFloorNo(21, 2);
    api::setFloorNo(22, 2);
    api::setFloorNo(23, 2);
    api::setFloorNo(24, 2);
    api::setFloorNo(25, 2);
    api::setFloorNo(26, 2);
    api::setFloorNo(27, 2);

    // add material
    api::addElasticMaterial(1, 20, 20, 2.4);

    // add section 1D
    api::addElasticSection1D(1, 1, new physicalModel::ArbitraryShape(0.25, 0.02, 0.02, 0.04));

    // add beam elements
    api::addBeam(1, { 10, 11 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(2, { 11, 12 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(3, { 13, 14 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(4, { 14, 15 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(5, { 16, 17 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(6, { 17, 18 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(7, { 10, 13 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(8, { 13, 16 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(9, { 11, 14 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(10, { 14, 17 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(11, { 12, 15 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(12, { 15, 18 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(13, { 19, 20 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(14, { 20, 21 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(15, { 22, 23 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(16, { 23, 24 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(17, { 25, 26 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(18, { 26, 27 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(19, { 19, 22 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(20, { 22, 25 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(21, { 20, 23 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(22, { 23, 26 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(23, { 21, 24 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(24, { 24, 27 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);

    // add column elements
    api::addColumn(101, { 1, 10 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(102, { 2, 11 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(103, { 3, 12 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(104, { 4, 13 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(105, { 5, 14 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(106, { 6, 15 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(107, { 7, 16 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(108, { 8, 17 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(109, { 9, 18 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(110, { 10, 19 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(111, { 11, 20 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(112, { 12, 21 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(113, { 13, 22 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(114, { 14, 23 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(115, { 15, 24 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(116, { 16, 25 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(117, { 17, 26 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(118, { 18, 27 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);

    // add section 2D
    api::addElasticSection2D(2, 1, 0.15);

    // add area element
    api::addSlab(1, { 10, 11, 14, 13 }, 2, physicalModel::AreaElementFormulation::LINEAR);
    api::addSlab(2, { 11, 12, 15, 14 }, 2, physicalModel::AreaElementFormulation::LINEAR);
    api::addSlab(3, { 13, 14, 17, 16 }, 2, physicalModel::AreaElementFormulation::LINEAR);
    api::addSlab(4, { 14, 15, 18, 17 }, 2, physicalModel::AreaElementFormulation::LINEAR);
    api::addSlab(5, { 19, 20, 23, 22 }, 2, physicalModel::AreaElementFormulation::LINEAR);
    api::addSlab(6, { 20, 21, 24, 23 }, 2, physicalModel::AreaElementFormulation::LINEAR);
    api::addSlab(7, { 22, 23, 26, 25 }, 2, physicalModel::AreaElementFormulation::LINEAR);
    api::addSlab(8, { 23, 24, 27, 26 }, 2, physicalModel::AreaElementFormulation::LINEAR);

    // loading
    api::updateAreaElementProperties();
    api::applyGravityLoadThroughLineElements(false);
    api::includeDeadLoadFromMembers(true);
    api::setLiveLoadForFloor(1, 2.0);
    api::setLiveLoadForFloor(2, 2.0);
    api::updateDeadAndLiveLoads();

    // floating-point comparison tolerance
    const double epsilon = 1e-6;

    // retrieve loads from load cases
    auto pointLoads = api::getPointLoads("dead");
    auto lineLoads = api::getDistributedLineLoads("dead");
    auto areaLoads = api::getDistributedAreaLoads("dead");
    auto pointLoadsL = api::getPointLoads("live");
    ASSERT_EQ(0, pointLoadsL.size());
    auto lineLoadsL = api::getDistributedLineLoads("live");
    ASSERT_EQ(0, lineLoadsL.size());
    auto areaLoadsL = api::getDistributedAreaLoads("live");

    // calculate total weight from members
    auto totalWeight = 0.0;

    for (auto pointLoad : pointLoads) {
        totalWeight += pointLoad->getLoadVector()[2];
    }

    for (auto lineLoad : lineLoads) {
        auto load1 = lineLoad->getLoadVector()[1] * api::getLength(std::dynamic_pointer_cast<physicalModel::DistributedLineLoad>(lineLoad)->getBeamElementTag());
        auto load2 = lineLoad->getLoadVector()[2] * api::getLength(std::dynamic_pointer_cast<physicalModel::DistributedLineLoad>(lineLoad)->getBeamElementTag());
        totalWeight += load1;
        totalWeight += load2;
    }

    for (auto areaLoad : areaLoads) {
        auto load = areaLoad->getLoadVector()[1] * api::getSurfaceArea(std::dynamic_pointer_cast<physicalModel::DistributedAreaLoad>(areaLoad)->getAreaElementTag());
        totalWeight += load;
    }

    for (auto areaLoad : areaLoadsL) {
        auto load = areaLoad->getLoadVector()[1] * api::getSurfaceArea(std::dynamic_pointer_cast<physicalModel::DistributedAreaLoad>(areaLoad)->getAreaElementTag());
        totalWeight += load;
    }

    auto expectedTotalWeight = -1139.9112;

    EXPECT_NEAR(expectedTotalWeight, totalWeight, epsilon);
}

TEST_F(LoadTests, SelfWeightAndLiveLoadThroughLineElementsSuccess) {
    typedef buildingModeler::BuildingModelerAPI api;

    // add joints
    api::addJoint(1, { 0, 0, 0 });
    api::addJoint(2, { 3, 0, 0 });
    api::addJoint(3, { 6, 0, 0 });
    api::addJoint(4, { 0, 3, 0 });
    api::addJoint(5, { 3, 3, 0 });
    api::addJoint(6, { 6, 3, 0 });
    api::addJoint(7, { 0, 6, 0 });
    api::addJoint(8, { 3, 6, 0 });
    api::addJoint(9, { 6, 6, 0 });
    api::addJoint(10, { 0, 0, 3 });
    api::addJoint(11, { 3, 0, 3 });
    api::addJoint(12, { 6, 0, 3 });
    api::addJoint(13, { 0, 3, 3 });
    api::addJoint(14, { 3, 3, 3 });
    api::addJoint(15, { 6, 3, 3 });
    api::addJoint(16, { 0, 6, 3 });
    api::addJoint(17, { 3, 6, 3 });
    api::addJoint(18, { 6, 6, 3 });
    api::addJoint(19, { 0, 0, 6 });
    api::addJoint(20, { 3, 0, 6 });
    api::addJoint(21, { 6, 0, 6 });
    api::addJoint(22, { 0, 3, 6 });
    api::addJoint(23, { 3, 3, 6 });
    api::addJoint(24, { 6, 3, 6 });
    api::addJoint(25, { 0, 6, 6 });
    api::addJoint(26, { 3, 6, 6 });
    api::addJoint(27, { 6, 6, 6 });

    // set constraints
    api::setConstraintVector(1, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(2, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(3, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(4, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(5, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(6, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(7, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(8, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(9, { 1, 1, 1, 1, 1, 1 });

    // add floors
    api::addFloor(0, 0.0);
    api::addFloor(1, 3.0);
    api::addFloor(2, 6.0);

    // assign nodes to floor
    api::setFloorNo(1, 0);
    api::setFloorNo(2, 0);
    api::setFloorNo(3, 0);
    api::setFloorNo(4, 0);
    api::setFloorNo(5, 0);
    api::setFloorNo(6, 0);
    api::setFloorNo(7, 0);
    api::setFloorNo(8, 0);
    api::setFloorNo(9, 0);
    api::setFloorNo(10, 1);
    api::setFloorNo(11, 1);
    api::setFloorNo(12, 1);
    api::setFloorNo(13, 1);
    api::setFloorNo(14, 1);
    api::setFloorNo(15, 1);
    api::setFloorNo(16, 1);
    api::setFloorNo(17, 1);
    api::setFloorNo(18, 1);
    api::setFloorNo(19, 2);
    api::setFloorNo(20, 2);
    api::setFloorNo(21, 2);
    api::setFloorNo(22, 2);
    api::setFloorNo(23, 2);
    api::setFloorNo(24, 2);
    api::setFloorNo(25, 2);
    api::setFloorNo(26, 2);
    api::setFloorNo(27, 2);

    // add material
    api::addElasticMaterial(1, 20, 20, 2.4);

    // add section 1D
    api::addElasticSection1D(1, 1, new physicalModel::ArbitraryShape(0.25, 0.02, 0.02, 0.04));

    // add beam elements
    api::addBeam(1, { 10, 11 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(2, { 11, 12 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(3, { 13, 14 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(4, { 14, 15 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(5, { 16, 17 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(6, { 17, 18 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(7, { 10, 13 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(8, { 13, 16 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(9, { 11, 14 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(10, { 14, 17 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(11, { 12, 15 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(12, { 15, 18 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(13, { 19, 20 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(14, { 20, 21 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(15, { 22, 23 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(16, { 23, 24 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(17, { 25, 26 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(18, { 26, 27 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(19, { 19, 22 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(20, { 22, 25 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(21, { 20, 23 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(22, { 23, 26 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(23, { 21, 24 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(24, { 24, 27 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);

    // add column elements
    api::addColumn(101, { 1, 10 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(102, { 2, 11 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(103, { 3, 12 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(104, { 4, 13 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(105, { 5, 14 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(106, { 6, 15 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(107, { 7, 16 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(108, { 8, 17 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(109, { 9, 18 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(110, { 10, 19 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(111, { 11, 20 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(112, { 12, 21 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(113, { 13, 22 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(114, { 14, 23 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(115, { 15, 24 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(116, { 16, 25 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(117, { 17, 26 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(118, { 18, 27 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);

    // add section 2D
    api::addElasticSection2D(2, 1, 0.15);

    // add area element
    api::addSlab(1, { 10, 11, 14, 13 }, 2, physicalModel::AreaElementFormulation::LINEAR);
    api::addSlab(2, { 11, 12, 15, 14 }, 2, physicalModel::AreaElementFormulation::LINEAR);
    api::addSlab(3, { 13, 14, 17, 16 }, 2, physicalModel::AreaElementFormulation::LINEAR);
    api::addSlab(4, { 14, 15, 18, 17 }, 2, physicalModel::AreaElementFormulation::LINEAR);
    api::addSlab(5, { 19, 20, 23, 22 }, 2, physicalModel::AreaElementFormulation::LINEAR);
    api::addSlab(6, { 20, 21, 24, 23 }, 2, physicalModel::AreaElementFormulation::LINEAR);
    api::addSlab(7, { 22, 23, 26, 25 }, 2, physicalModel::AreaElementFormulation::LINEAR);
    api::addSlab(8, { 23, 24, 27, 26 }, 2, physicalModel::AreaElementFormulation::LINEAR);

    // loading
    api::updateAreaElementProperties();
    api::applyGravityLoadThroughLineElements(true);
    api::includeDeadLoadFromMembers(true);
    api::setLiveLoadForFloor(1, 2.0);
    api::setLiveLoadForFloor(2, 2.0);
    api::updateDeadAndLiveLoads();

    // floating-point comparison tolerance
    const double epsilon = 1e-6;

    // retrieve loads from load cases
    auto pointLoads = api::getPointLoads("dead");
    auto lineLoads = api::getDistributedLineLoads("dead");
    auto areaLoads = api::getDistributedAreaLoads("dead");
    ASSERT_EQ(0, areaLoads.size());
    auto pointLoadsL = api::getPointLoads("live");
    ASSERT_EQ(0, pointLoadsL.size());
    auto lineLoadsL = api::getDistributedLineLoads("live");
    auto areaLoadsL = api::getDistributedAreaLoads("live");
    ASSERT_EQ(0, areaLoadsL.size());

    // calculate total weight from members
    auto totalWeight = 0.0;

    for (auto pointLoad : pointLoads) {
        totalWeight += pointLoad->getLoadVector()[2];
    }

    for (auto lineLoad : lineLoads) {
        auto load1 = lineLoad->getLoadVector()[1] * api::getLength(std::dynamic_pointer_cast<physicalModel::DistributedLineLoad>(lineLoad)->getBeamElementTag());
        auto load2 = lineLoad->getLoadVector()[2] * api::getLength(std::dynamic_pointer_cast<physicalModel::DistributedLineLoad>(lineLoad)->getBeamElementTag());
        totalWeight += load1;
        totalWeight += load2;
    }

    for (auto lineLoad : lineLoadsL) {
        auto load = lineLoad->getLoadVector()[1] * api::getLength(std::dynamic_pointer_cast<physicalModel::DistributedLineLoad>(lineLoad)->getBeamElementTag());
        totalWeight += load;
    }

    auto expectedTotalWeight = -1139.9112;

    EXPECT_NEAR(expectedTotalWeight, totalWeight, epsilon);
}

TEST_F(LoadTests, SelfWeightLiveEqWindLoadCombinationSuccess) {
    typedef buildingModeler::BuildingModelerAPI api;

    // add joints
    api::addJoint(1, { 0, 0, 0 });
    api::addJoint(2, { 3, 0, 0 });
    api::addJoint(3, { 6, 0, 0 });
    api::addJoint(4, { 0, 3, 0 });
    api::addJoint(5, { 3, 3, 0 });
    api::addJoint(6, { 6, 3, 0 });
    api::addJoint(7, { 0, 6, 0 });
    api::addJoint(8, { 3, 6, 0 });
    api::addJoint(9, { 6, 6, 0 });
    api::addJoint(10, { 0, 0, 3 });
    api::addJoint(11, { 3, 0, 3 });
    api::addJoint(12, { 6, 0, 3 });
    api::addJoint(13, { 0, 3, 3 });
    api::addJoint(14, { 3, 3, 3 });
    api::addJoint(15, { 6, 3, 3 });
    api::addJoint(16, { 0, 6, 3 });
    api::addJoint(17, { 3, 6, 3 });
    api::addJoint(18, { 6, 6, 3 });
    api::addJoint(19, { 0, 0, 6 });
    api::addJoint(20, { 3, 0, 6 });
    api::addJoint(21, { 6, 0, 6 });
    api::addJoint(22, { 0, 3, 6 });
    api::addJoint(23, { 3, 3, 6 });
    api::addJoint(24, { 6, 3, 6 });
    api::addJoint(25, { 0, 6, 6 });
    api::addJoint(26, { 3, 6, 6 });
    api::addJoint(27, { 6, 6, 6 });

    // add masses
    api::addTranslationalMass(10, { 10.0, 10.0, 0.0 });
    api::addTranslationalMass(12, { 10.0, 10.0, 0.0 });
    api::addTranslationalMass(16, { 10.0, 10.0, 0.0 });
    api::addTranslationalMass(18, { 10.0, 10.0, 0.0 });
    api::addTranslationalMass(19, { 10.0, 10.0, 0.0 });
    api::addTranslationalMass(21, { 10.0, 10.0, 0.0 });
    api::addTranslationalMass(25, { 10.0, 10.0, 0.0 });
    api::addTranslationalMass(27, { 10.0, 10.0, 0.0 });

    // set constraints
    api::setConstraintVector(1, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(2, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(3, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(4, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(5, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(6, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(7, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(8, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(9, { 1, 1, 1, 1, 1, 1 });

    // add floors
    api::addFloor(0, 0.0);
    api::addFloor(1, 3.0);
    api::addFloor(2, 6.0);

    // assign nodes to floor
    api::setFloorNo(1, 0);
    api::setFloorNo(2, 0);
    api::setFloorNo(3, 0);
    api::setFloorNo(4, 0);
    api::setFloorNo(5, 0);
    api::setFloorNo(6, 0);
    api::setFloorNo(7, 0);
    api::setFloorNo(8, 0);
    api::setFloorNo(9, 0);
    api::setFloorNo(10, 1);
    api::setFloorNo(11, 1);
    api::setFloorNo(12, 1);
    api::setFloorNo(13, 1);
    api::setFloorNo(14, 1);
    api::setFloorNo(15, 1);
    api::setFloorNo(16, 1);
    api::setFloorNo(17, 1);
    api::setFloorNo(18, 1);
    api::setFloorNo(19, 2);
    api::setFloorNo(20, 2);
    api::setFloorNo(21, 2);
    api::setFloorNo(22, 2);
    api::setFloorNo(23, 2);
    api::setFloorNo(24, 2);
    api::setFloorNo(25, 2);
    api::setFloorNo(26, 2);
    api::setFloorNo(27, 2);

    // add material
    api::addElasticMaterial(1, 20, 20, 2.4);

    // add section 1D
    api::addElasticSection1D(1, 1, new physicalModel::ArbitraryShape(0.25, 0.02, 0.02, 0.04));

    // add beam elements
    api::addBeam(1, { 10, 11 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(2, { 11, 12 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(3, { 13, 14 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(4, { 14, 15 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(5, { 16, 17 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(6, { 17, 18 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(7, { 10, 13 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(8, { 13, 16 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(9, { 11, 14 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(10, { 14, 17 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(11, { 12, 15 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(12, { 15, 18 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(13, { 19, 20 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(14, { 20, 21 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(15, { 22, 23 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(16, { 23, 24 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(17, { 25, 26 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(18, { 26, 27 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(19, { 19, 22 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(20, { 22, 25 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(21, { 20, 23 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(22, { 23, 26 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(23, { 21, 24 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(24, { 24, 27 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);

    // add column elements
    api::addColumn(101, { 1, 10 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(102, { 2, 11 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(103, { 3, 12 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(104, { 4, 13 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(105, { 5, 14 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(106, { 6, 15 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(107, { 7, 16 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(108, { 8, 17 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(109, { 9, 18 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(110, { 10, 19 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(111, { 11, 20 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(112, { 12, 21 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(113, { 13, 22 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(114, { 14, 23 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(115, { 15, 24 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(116, { 16, 25 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(117, { 17, 26 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(118, { 18, 27 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);

    // add section 2D
    api::addElasticSection2D(2, 1, 0.15);

    // add area element
    api::addSlab(1, { 10, 11, 14, 13 }, 2, physicalModel::AreaElementFormulation::LINEAR);
    api::addSlab(2, { 11, 12, 15, 14 }, 2, physicalModel::AreaElementFormulation::LINEAR);
    api::addSlab(3, { 13, 14, 17, 16 }, 2, physicalModel::AreaElementFormulation::LINEAR);
    api::addSlab(4, { 14, 15, 18, 17 }, 2, physicalModel::AreaElementFormulation::LINEAR);
    api::addSlab(5, { 19, 20, 23, 22 }, 2, physicalModel::AreaElementFormulation::LINEAR);
    api::addSlab(6, { 20, 21, 24, 23 }, 2, physicalModel::AreaElementFormulation::LINEAR);
    api::addSlab(7, { 22, 23, 26, 25 }, 2, physicalModel::AreaElementFormulation::LINEAR);
    api::addSlab(8, { 23, 24, 27, 26 }, 2, physicalModel::AreaElementFormulation::LINEAR);

    // make rigid
    api::makeRigid(1, 100);
    api::confineFloorMassOnDiaphragmNode(1, true);
    api::makeRigid(2, 200);
    api::confineFloorMassOnDiaphragmNode(2, true);

    // loading
    api::updateAreaElementProperties();
    api::applyGravityLoadThroughLineElements(false);
    api::includeDeadLoadFromMembers(true);
    api::setLiveLoadForFloor(1, 2.0);
    api::setLiveLoadForFloor(2, 2.0);
    api::updateDeadAndLiveLoads();

    // additional loading
    api::addStaticLoadCase("WIND", physicalModel::StaticLoadCaseType::WIND);
    api::addPointLoad("WIND", 100, 100, 100, 0, 0, 0, 0);
    api::addStaticLoadCase("EQ", physicalModel::StaticLoadCaseType::EARTHQUAKE);
    api::addPointLoad("EQ", 100, 200, 200, 0, 0, 0, 0);
    api::addPointLoad("EQ", 200, 150, 200, 0, 0, 0, 0);

    // create load combination
    api::addStaticLoadCombination("COMB");
    api::setStaticLoadCombinationActive("COMB", true);
    api::addLoadCaseToStaticLoadCombination("COMB", "dead", 1.4);
    api::addLoadCaseToStaticLoadCombination("COMB", "live", 1.6);
    api::addLoadCaseToStaticLoadCombination("COMB", "WIND", 1.0);
    api::addLoadCaseToStaticLoadCombination("COMB", "EQ", 1.2);

    // create analytical model
    physicalModel::Building::getInstance().toAnalyticalModel();

    // floating-point comparison tolerance
    const double epsilon = 1e-6;

    // retrieve loads from load cases
    auto loads = api::getLoadsFromAnalyticalModel("COMB");

    // calculate total weight from members
    auto actualFx = 0.0;
    auto actualFy = 0.0;
    auto actualFz = 0.0;

    for (auto load : loads) {
        
        if (auto elementLoad = std::dynamic_pointer_cast<opensees::ElementLoad>(load)) {

            auto loadVec = elementLoad->getLoadVector();
            auto length = dynamic_cast<opensees::BeamColumnElement*>(opensees::OpenseesModel::getInstance().getBeamColumnElement(elementLoad->getElementTag()))->getLength();

            actualFz += loadVec[1] * length;
            actualFz += loadVec[2] * length;
        }
        else {

            auto loadVec = load->getLoadVector();

            actualFx += loadVec[0];
            actualFy += loadVec[1];
            actualFz += loadVec[2];
        }
    }
    
    auto expectedFx = 520.0;
    auto expectedFy = 580.0;
    auto expectedFz = -1624.67568;
    
    EXPECT_NEAR(expectedFx, actualFx, epsilon);
    EXPECT_NEAR(expectedFy, actualFy, epsilon);
    EXPECT_NEAR(expectedFz, actualFz, epsilon);
}

TEST_F(LoadTests, SelfWeightLiveEqWindLoadCombinationThroughLineElementsSuccess) {
    typedef buildingModeler::BuildingModelerAPI api;

    // add joints
    api::addJoint(1, { 0, 0, 0 });
    api::addJoint(2, { 3, 0, 0 });
    api::addJoint(3, { 6, 0, 0 });
    api::addJoint(4, { 0, 3, 0 });
    api::addJoint(5, { 3, 3, 0 });
    api::addJoint(6, { 6, 3, 0 });
    api::addJoint(7, { 0, 6, 0 });
    api::addJoint(8, { 3, 6, 0 });
    api::addJoint(9, { 6, 6, 0 });
    api::addJoint(10, { 0, 0, 3 });
    api::addJoint(11, { 3, 0, 3 });
    api::addJoint(12, { 6, 0, 3 });
    api::addJoint(13, { 0, 3, 3 });
    api::addJoint(14, { 3, 3, 3 });
    api::addJoint(15, { 6, 3, 3 });
    api::addJoint(16, { 0, 6, 3 });
    api::addJoint(17, { 3, 6, 3 });
    api::addJoint(18, { 6, 6, 3 });
    api::addJoint(19, { 0, 0, 6 });
    api::addJoint(20, { 3, 0, 6 });
    api::addJoint(21, { 6, 0, 6 });
    api::addJoint(22, { 0, 3, 6 });
    api::addJoint(23, { 3, 3, 6 });
    api::addJoint(24, { 6, 3, 6 });
    api::addJoint(25, { 0, 6, 6 });
    api::addJoint(26, { 3, 6, 6 });
    api::addJoint(27, { 6, 6, 6 });

    // add masses
    api::addTranslationalMass(10, { 10.0, 10.0, 0.0 });
    api::addTranslationalMass(12, { 10.0, 10.0, 0.0 });
    api::addTranslationalMass(16, { 10.0, 10.0, 0.0 });
    api::addTranslationalMass(18, { 10.0, 10.0, 0.0 });
    api::addTranslationalMass(19, { 10.0, 10.0, 0.0 });
    api::addTranslationalMass(21, { 10.0, 10.0, 0.0 });
    api::addTranslationalMass(25, { 10.0, 10.0, 0.0 });
    api::addTranslationalMass(27, { 10.0, 10.0, 0.0 });

    // set constraints
    api::setConstraintVector(1, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(2, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(3, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(4, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(5, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(6, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(7, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(8, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(9, { 1, 1, 1, 1, 1, 1 });

    // add floors
    api::addFloor(0, 0.0);
    api::addFloor(1, 3.0);
    api::addFloor(2, 6.0);

    // assign nodes to floor
    api::setFloorNo(1, 0);
    api::setFloorNo(2, 0);
    api::setFloorNo(3, 0);
    api::setFloorNo(4, 0);
    api::setFloorNo(5, 0);
    api::setFloorNo(6, 0);
    api::setFloorNo(7, 0);
    api::setFloorNo(8, 0);
    api::setFloorNo(9, 0);
    api::setFloorNo(10, 1);
    api::setFloorNo(11, 1);
    api::setFloorNo(12, 1);
    api::setFloorNo(13, 1);
    api::setFloorNo(14, 1);
    api::setFloorNo(15, 1);
    api::setFloorNo(16, 1);
    api::setFloorNo(17, 1);
    api::setFloorNo(18, 1);
    api::setFloorNo(19, 2);
    api::setFloorNo(20, 2);
    api::setFloorNo(21, 2);
    api::setFloorNo(22, 2);
    api::setFloorNo(23, 2);
    api::setFloorNo(24, 2);
    api::setFloorNo(25, 2);
    api::setFloorNo(26, 2);
    api::setFloorNo(27, 2);

    // add material
    api::addElasticMaterial(1, 20, 20, 2.4);

    // add section 1D
    api::addElasticSection1D(1, 1, new physicalModel::ArbitraryShape(0.25, 0.02, 0.02, 0.04));

    // add beam elements
    api::addBeam(1, { 10, 11 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(2, { 11, 12 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(3, { 13, 14 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(4, { 14, 15 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(5, { 16, 17 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(6, { 17, 18 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(7, { 10, 13 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(8, { 13, 16 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(9, { 11, 14 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(10, { 14, 17 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(11, { 12, 15 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(12, { 15, 18 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(13, { 19, 20 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(14, { 20, 21 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(15, { 22, 23 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(16, { 23, 24 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(17, { 25, 26 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(18, { 26, 27 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(19, { 19, 22 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(20, { 22, 25 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(21, { 20, 23 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(22, { 23, 26 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(23, { 21, 24 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(24, { 24, 27 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);

    // add column elements
    api::addColumn(101, { 1, 10 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(102, { 2, 11 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(103, { 3, 12 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(104, { 4, 13 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(105, { 5, 14 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(106, { 6, 15 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(107, { 7, 16 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(108, { 8, 17 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(109, { 9, 18 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(110, { 10, 19 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(111, { 11, 20 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(112, { 12, 21 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(113, { 13, 22 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(114, { 14, 23 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(115, { 15, 24 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(116, { 16, 25 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(117, { 17, 26 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addColumn(118, { 18, 27 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);

    // add section 2D
    api::addElasticSection2D(2, 1, 0.15);

    // add area element
    api::addSlab(1, { 10, 11, 14, 13 }, 2, physicalModel::AreaElementFormulation::LINEAR);
    api::addSlab(2, { 11, 12, 15, 14 }, 2, physicalModel::AreaElementFormulation::LINEAR);
    api::addSlab(3, { 13, 14, 17, 16 }, 2, physicalModel::AreaElementFormulation::LINEAR);
    api::addSlab(4, { 14, 15, 18, 17 }, 2, physicalModel::AreaElementFormulation::LINEAR);
    api::addSlab(5, { 19, 20, 23, 22 }, 2, physicalModel::AreaElementFormulation::LINEAR);
    api::addSlab(6, { 20, 21, 24, 23 }, 2, physicalModel::AreaElementFormulation::LINEAR);
    api::addSlab(7, { 22, 23, 26, 25 }, 2, physicalModel::AreaElementFormulation::LINEAR);
    api::addSlab(8, { 23, 24, 27, 26 }, 2, physicalModel::AreaElementFormulation::LINEAR);

    // make rigid
    api::makeRigid(1, 100);
    api::confineFloorMassOnDiaphragmNode(1, true);
    api::makeRigid(2, 200);
    api::confineFloorMassOnDiaphragmNode(2, true);

    // loading
    api::updateAreaElementProperties();
    api::applyGravityLoadThroughLineElements(true);
    api::includeDeadLoadFromMembers(true);
    api::setLiveLoadForFloor(1, 2.0);
    api::setLiveLoadForFloor(2, 2.0);
    api::updateDeadAndLiveLoads();

    // additional loading
    api::addStaticLoadCase("WIND", physicalModel::StaticLoadCaseType::WIND);
    api::addPointLoad("WIND", 100, 100, 100, 0, 0, 0, 0);
    api::addStaticLoadCase("EQ", physicalModel::StaticLoadCaseType::EARTHQUAKE);
    api::addPointLoad("EQ", 100, 200, 200, 0, 0, 0, 0);
    api::addPointLoad("EQ", 200, 150, 200, 0, 0, 0, 0);

    // create load combination
    api::addStaticLoadCombination("COMB");
    api::setStaticLoadCombinationActive("COMB", true);
    api::addLoadCaseToStaticLoadCombination("COMB", "dead", 1.4);
    api::addLoadCaseToStaticLoadCombination("COMB", "live", 1.6);
    api::addLoadCaseToStaticLoadCombination("COMB", "WIND", 1.0);
    api::addLoadCaseToStaticLoadCombination("COMB", "EQ", 1.2);

    // create analytical model
    physicalModel::Building::getInstance().toAnalyticalModel();

    // floating-point comparison tolerance
    const double epsilon = 1e-6;

    // retrieve loads from load cases
    auto loads = api::getLoadsFromAnalyticalModel("COMB");

    // calculate total weight from members
    auto actualFx = 0.0;
    auto actualFy = 0.0;
    auto actualFz = 0.0;

    for (auto load : loads) {

        if (auto elementLoad = std::dynamic_pointer_cast<opensees::ElementLoad>(load)) {

            auto loadVec = elementLoad->getLoadVector();
            auto length = dynamic_cast<opensees::BeamColumnElement*>(opensees::OpenseesModel::getInstance().getBeamColumnElement(elementLoad->getElementTag()))->getLength();

            actualFz += loadVec[1] * length;
            actualFz += loadVec[2] * length;
        }
        else {

            auto loadVec = load->getLoadVector();

            actualFx += loadVec[0];
            actualFy += loadVec[1];
            actualFz += loadVec[2];
        }
    }

    auto expectedFx = 520.0;
    auto expectedFy = 580.0;
    auto expectedFz = -1624.67568;

    EXPECT_NEAR(expectedFx, actualFx, epsilon);
    EXPECT_NEAR(expectedFy, actualFy, epsilon);
    EXPECT_NEAR(expectedFz, actualFz, epsilon);
}
