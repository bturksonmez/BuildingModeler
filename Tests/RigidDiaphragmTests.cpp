#include <gtest/gtest.h>
#include "../BuildingModeler/BuildingModelerAPI.h"
#include "../BuildingModeler/Utilities/Vector3.h"

class RigidDiaphragmTests : public ::testing::Test {
protected:
    void TearDown() override {
        typedef buildingModeler::BuildingModelerAPI api;

        api::clear();
    }
};

TEST_F(RigidDiaphragmTests, DiaphragmMassXYSuccess) {
    typedef buildingModeler::BuildingModelerAPI api;

    // add joints
    api::addJoint(1, { 0, 0, 0 });
    api::addJoint(2, { 2, 0, 0 });
    api::addJoint(3, { 7, 0, 0 });
    api::addJoint(4, { 0, 2, 0 });
    api::addJoint(5, { 2, 2, 0 });
    api::addJoint(6, { 7, 2, 0 });
    api::addJoint(7, { 0, 7, 0 });
    api::addJoint(8, { 2, 7, 0 });
    api::addJoint(9, { 7, 7, 0 });

    // add masses
    api::addTranslationalMass(1, { 10.0, 10.0, 0.0 });
    api::addTranslationalMass(2, { 10.0, 10.0, 0.0 });
    api::addTranslationalMass(3, { 10.0, 10.0, 0.0 });
    api::addTranslationalMass(4, { 10.0, 10.0, 0.0 });
    api::addTranslationalMass(5, { 10.0, 10.0, 0.0 });
    api::addTranslationalMass(6, { 10.0, 10.0, 0.0 });
    api::addTranslationalMass(7, { 10.0, 10.0, 0.0 });
    api::addTranslationalMass(8, { 10.0, 10.0, 0.0 });
    api::addTranslationalMass(9, { 10.0, 10.0, 0.0 });

    // add floors
    api::addFloor(1, 0.0);

    // assign nodes to floor
    api::setFloorNo(1, 1);
    api::setFloorNo(2, 1);
    api::setFloorNo(3, 1);
    api::setFloorNo(4, 1);
    api::setFloorNo(5, 1);
    api::setFloorNo(6, 1);
    api::setFloorNo(7, 1);
    api::setFloorNo(8, 1);
    api::setFloorNo(9, 1);

    // make rigid
    api::makeRigid(1, 10);
    api::confineFloorMassOnDiaphragmNode(1, true);

    // create analytical model
    physicalModel::Building::getInstance().toAnalyticalModel();

    // floating-point comparison tolerance
    const double epsilon = 1e-6;

    // retrieve translational and rotational masses for the diaphragm node from physical and analytical model and assert them
    auto transMassPhysical = api::getTranslationalMassForJointFromPhysicalModel(10);
    auto rotMassPhysical = api::getRotationalMassForJointFromPhysicalModel(10);
    auto transMassAnalytical = api::getTranslationalMassForJointFromAnalyticalModel(10);
    auto rotMassAnalytical = api::getRotationalMassForJointFromAnalyticalModel(10);

    auto expectedTransMass = utility::Vector3{ 90, 90, 0 };
    auto expectedRotMass = utility::Vector3{ 0, 0, 1560 };

    EXPECT_NEAR(expectedTransMass.x, transMassPhysical.value().x, epsilon);
    EXPECT_NEAR(expectedTransMass.y, transMassPhysical.value().y, epsilon);
    EXPECT_NEAR(expectedTransMass.z, transMassPhysical.value().z, epsilon);
    EXPECT_NEAR(expectedTransMass.x, transMassAnalytical.x, epsilon);
    EXPECT_NEAR(expectedTransMass.y, transMassAnalytical.y, epsilon);
    EXPECT_NEAR(expectedTransMass.z, transMassAnalytical.z, epsilon);
    EXPECT_NEAR(expectedRotMass.x, rotMassPhysical.value().x, epsilon);
    EXPECT_NEAR(expectedRotMass.y, rotMassPhysical.value().y, epsilon);
    EXPECT_NEAR(expectedRotMass.z, rotMassPhysical.value().z, epsilon);
    EXPECT_NEAR(expectedRotMass.x, rotMassAnalytical.x, epsilon);
    EXPECT_NEAR(expectedRotMass.y, rotMassAnalytical.y, epsilon);
    EXPECT_NEAR(expectedRotMass.z, rotMassAnalytical.z, epsilon);

    // retrieve diaphragm mass and mass center for a floor
    auto diaphragmMass = api::getDiaphragmMass(1);
    auto massCenter = api::getMassCenter(1);

    auto expectedDiaphragmMass = utility::Vector3{ 90, 90,1560 };
    auto expectedMassCenter = utility::Vector2{ 3, 3 };

    EXPECT_NEAR(expectedDiaphragmMass.x, diaphragmMass.value().x, epsilon);
    EXPECT_NEAR(expectedDiaphragmMass.y, diaphragmMass.value().y, epsilon);
    EXPECT_NEAR(expectedDiaphragmMass.z, diaphragmMass.value().z, epsilon);
    EXPECT_NEAR(expectedMassCenter.x, massCenter.value().x, epsilon);
    EXPECT_NEAR(expectedMassCenter.y, massCenter.value().y, epsilon);

    // Check to make sure physical model masses are not transferred to analytical model when those
    // masses are confined to diaphragm node
    transMassPhysical = api::getTranslationalMassForJointFromPhysicalModel(1);
    rotMassPhysical = api::getRotationalMassForJointFromPhysicalModel(1);
    transMassAnalytical = api::getTranslationalMassForJointFromAnalyticalModel(1);
    rotMassAnalytical = api::getRotationalMassForJointFromAnalyticalModel(1);

    auto expectedMass = utility::Vector3{ 10, 10, 0 };
    auto expectedDummyVec = utility::Vector3{ 0, 0, 0 };

    EXPECT_NEAR(expectedMass.x, transMassPhysical.value().x, epsilon);
    EXPECT_NEAR(expectedMass.y, transMassPhysical.value().y, epsilon);
    EXPECT_NEAR(expectedMass.z, transMassPhysical.value().z, epsilon);
    EXPECT_NEAR(expectedDummyVec.x, transMassAnalytical.x, epsilon);
    EXPECT_NEAR(expectedDummyVec.y, transMassAnalytical.y, epsilon);
    EXPECT_NEAR(expectedDummyVec.z, transMassAnalytical.z, epsilon);
    ASSERT_EQ(std::nullopt, rotMassPhysical);
    EXPECT_NEAR(expectedDummyVec.x, rotMassAnalytical.x, epsilon);
    EXPECT_NEAR(expectedDummyVec.y, rotMassAnalytical.y, epsilon);
    EXPECT_NEAR(expectedDummyVec.z, rotMassAnalytical.z, epsilon);
}

TEST_F(RigidDiaphragmTests, DiaphragmMassXYSuccessWithMemberMasses) {
    typedef buildingModeler::BuildingModelerAPI api;

    // add joints
    api::addJoint(1, { 0, 0, 0 });
    api::addJoint(2, { 2, 0, 0 });
    api::addJoint(3, { 7, 0, 0 });
    api::addJoint(4, { 0, 2, 0 });
    api::addJoint(5, { 2, 2, 0 });
    api::addJoint(6, { 7, 2, 0 });
    api::addJoint(7, { 0, 7, 0 });
    api::addJoint(8, { 2, 7, 0 });
    api::addJoint(9, { 7, 7, 0 });

    // add masses
    api::addTranslationalMass(1, { 10.0, 10.0, 0.0 });
    api::addTranslationalMass(2, { 10.0, 10.0, 0.0 });
    api::addTranslationalMass(3, { 10.0, 10.0, 0.0 });
    api::addTranslationalMass(4, { 10.0, 10.0, 0.0 });
    api::addTranslationalMass(5, { 10.0, 10.0, 0.0 });
    api::addTranslationalMass(6, { 10.0, 10.0, 0.0 });
    api::addTranslationalMass(7, { 10.0, 10.0, 0.0 });
    api::addTranslationalMass(8, { 10.0, 10.0, 0.0 });
    api::addTranslationalMass(9, { 10.0, 10.0, 0.0 });

    // add floors
    api::addFloor(1, 0.0);

    // assign nodes to floor
    api::setFloorNo(1, 1);
    api::setFloorNo(2, 1);
    api::setFloorNo(3, 1);
    api::setFloorNo(4, 1);
    api::setFloorNo(5, 1);
    api::setFloorNo(6, 1);
    api::setFloorNo(7, 1);
    api::setFloorNo(8, 1);
    api::setFloorNo(9, 1);

    // add material
    api::addElasticMaterial(1, 20, 20, 2);

    // add section 2D
    api::addElasticSection2D(1, 1, 0.5);

    // add area element
    api::addSlab(1, { 1, 2, 5, 4 }, 1, physicalModel::AreaElementFormulation::LINEAR);
    api::addSlab(2, { 2, 3, 6, 5 }, 1, physicalModel::AreaElementFormulation::LINEAR);
    api::addSlab(3, { 4, 5, 8, 7 }, 1, physicalModel::AreaElementFormulation::LINEAR);
    api::addSlab(4, { 5, 6, 9, 8 }, 1, physicalModel::AreaElementFormulation::LINEAR);

    // add section 1D
    api::addElasticSection1D(2, 1, new physicalModel::ArbitraryShape(0.5, 0.02, 0.02, 0.04));

    // add line element
    api::addBeam(1, { 1, 2 }, 2, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(2, { 2, 3 }, 2, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(3, { 4, 5 }, 2, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(4, { 5, 6 }, 2, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(5, { 7, 8 }, 2, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(6, { 8, 9 }, 2, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(7, { 1, 4 }, 2, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(8, { 4, 7 }, 2, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(9, { 2, 5 }, 2, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(10, { 5, 8 }, 2, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(11, { 3, 6 }, 2, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(12, { 6, 9 }, 2, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);

    // set segment ratios
    api::setSegmentRatios(1, { 0.5, 0.5 });
    api::setSegmentRatios(2, { 0.5, 0.5 });
    api::setSegmentRatios(3, { 0.5, 0.5 });
    api::setSegmentRatios(4, { 0.5, 0.5 });
    api::setSegmentRatios(5, { 0.5, 0.5 });
    api::setSegmentRatios(6, { 0.5, 0.5 });
    api::setSegmentRatios(7, { 0.5, 0.5 });
    api::setSegmentRatios(8, { 0.5, 0.5 });
    api::setSegmentRatios(9, { 0.5, 0.5 });
    api::setSegmentRatios(10, { 0.5, 0.5 });
    api::setSegmentRatios(11, { 0.5, 0.5 });
    api::setSegmentRatios(12, { 0.5, 0.5 });

    // update connectivity between line and area elements
    api::updateAreaElementProperties();

    //mesh the element
    api::meshAreaElement(1);
    api::meshAreaElement(2);
    api::meshAreaElement(3);
    api::meshAreaElement(4);

    // make rigid
    api::updateMassSourceFromMembers();
    api::makeRigid(1, 10);
    api::confineFloorMassOnDiaphragmNode(1, true);

    // create analytical model
    physicalModel::Building::getInstance().toAnalyticalModel();

    // floating-point comparison tolerance
    const double epsilon = 1e-6;

    // retrieve translational and rotational masses for the diaphragm node from physical and analytical model and assert them
    auto transMassPhysical = api::getTranslationalMassForJointFromPhysicalModel(10);
    auto rotMassPhysical = api::getRotationalMassForJointFromPhysicalModel(10);
    auto transMassAnalytical = api::getTranslationalMassForJointFromAnalyticalModel(10);
    auto rotMassAnalytical = api::getRotationalMassForJointFromAnalyticalModel(10);

    auto expectedTransMass = utility::Vector3{ 181, 181, 0 };
    auto expectedRotMass = utility::Vector3{ 0, 0, 2960.46408839779 };

    EXPECT_NEAR(expectedTransMass.x, transMassPhysical.value().x, epsilon);
    EXPECT_NEAR(expectedTransMass.y, transMassPhysical.value().y, epsilon);
    EXPECT_NEAR(expectedTransMass.z, transMassPhysical.value().z, epsilon);
    EXPECT_NEAR(expectedTransMass.x, transMassAnalytical.x, epsilon);
    EXPECT_NEAR(expectedTransMass.y, transMassAnalytical.y, epsilon);
    EXPECT_NEAR(expectedTransMass.z, transMassAnalytical.z, epsilon);
    EXPECT_NEAR(expectedRotMass.x, rotMassPhysical.value().x, epsilon);
    EXPECT_NEAR(expectedRotMass.y, rotMassPhysical.value().y, epsilon);
    EXPECT_NEAR(expectedRotMass.z, rotMassPhysical.value().z, epsilon);
    EXPECT_NEAR(expectedRotMass.x, rotMassAnalytical.x, epsilon);
    EXPECT_NEAR(expectedRotMass.y, rotMassAnalytical.y, epsilon);
    EXPECT_NEAR(expectedRotMass.z, rotMassAnalytical.z, epsilon);
    
    // retrieve diaphragm mass and mass center for a floor
    auto diaphragmMass = api::getDiaphragmMass(1);
    auto massCenter = api::getMassCenter(1);
    
    auto expectedDiaphragmMass = utility::Vector3{ 181, 181, 2960.46408839779 };
    auto expectedMassCenter = utility::Vector2{ 3.193370166, 3.193370166 };
    
    EXPECT_NEAR(expectedDiaphragmMass.x, diaphragmMass.value().x, epsilon);
    EXPECT_NEAR(expectedDiaphragmMass.y, diaphragmMass.value().y, epsilon);
    EXPECT_NEAR(expectedDiaphragmMass.z, diaphragmMass.value().z, epsilon);
    EXPECT_NEAR(expectedMassCenter.x, massCenter.value().x, epsilon);
    EXPECT_NEAR(expectedMassCenter.y, massCenter.value().y, epsilon);
    
    // Check to make sure physical model masses are not transferred to analytical model when those
    // masses are confined to diaphragm node
    transMassPhysical = api::getTranslationalMassForJointFromPhysicalModel(1);
    rotMassPhysical = api::getRotationalMassForJointFromPhysicalModel(1);
    transMassAnalytical = api::getTranslationalMassForJointFromAnalyticalModel(1);
    rotMassAnalytical = api::getRotationalMassForJointFromAnalyticalModel(1);

    auto expectedMass = utility::Vector3{ 13, 13, 0 };
    auto expectedDummyVec = utility::Vector3{ 0, 0, 0 };

    EXPECT_NEAR(expectedMass.x, transMassPhysical.value().x, epsilon);
    EXPECT_NEAR(expectedMass.y, transMassPhysical.value().y, epsilon);
    EXPECT_NEAR(expectedMass.z, transMassPhysical.value().z, epsilon);
    EXPECT_NEAR(expectedDummyVec.x, transMassAnalytical.x, epsilon);
    EXPECT_NEAR(expectedDummyVec.y, transMassAnalytical.y, epsilon);
    EXPECT_NEAR(expectedDummyVec.z, transMassAnalytical.z, epsilon);
    ASSERT_EQ(std::nullopt, rotMassPhysical);
    EXPECT_NEAR(expectedDummyVec.x, rotMassAnalytical.x, epsilon);
    EXPECT_NEAR(expectedDummyVec.y, rotMassAnalytical.y, epsilon);
    EXPECT_NEAR(expectedDummyVec.z, rotMassAnalytical.z, epsilon);
}

TEST_F(RigidDiaphragmTests, DiaphragmMassXSuccess) {
    typedef buildingModeler::BuildingModelerAPI api;

    // add joints
    api::addJoint(1, { 0, 0, 0 });
    api::addJoint(2, { 2, 0, 0 });
    api::addJoint(3, { 7, 0, 0 });
    api::addJoint(4, { 0, 2, 0 });
    api::addJoint(5, { 2, 2, 0 });
    api::addJoint(6, { 7, 2, 0 });
    api::addJoint(7, { 0, 7, 0 });
    api::addJoint(8, { 2, 7, 0 });
    api::addJoint(9, { 7, 7, 0 });

    // add masses
    api::addTranslationalMass(1, { 10.0, 0.0, 0.0 });
    api::addTranslationalMass(2, { 10.0, 0.0, 0.0 });
    api::addTranslationalMass(3, { 10.0, 0.0, 0.0 });
    api::addTranslationalMass(4, { 10.0, 0.0, 0.0 });
    api::addTranslationalMass(5, { 10.0, 0.0, 0.0 });
    api::addTranslationalMass(6, { 10.0, 0.0, 0.0 });
    api::addTranslationalMass(7, { 10.0, 0.0, 0.0 });
    api::addTranslationalMass(8, { 10.0, 0.0, 0.0 });
    api::addTranslationalMass(9, { 10.0, 0.0, 0.0 });

    // add floors
    api::addFloor(1, 0.0);

    // assign nodes to floor
    api::setFloorNo(1, 1);
    api::setFloorNo(2, 1);
    api::setFloorNo(3, 1);
    api::setFloorNo(4, 1);
    api::setFloorNo(5, 1);
    api::setFloorNo(6, 1);
    api::setFloorNo(7, 1);
    api::setFloorNo(8, 1);
    api::setFloorNo(9, 1);

    // make rigid
    api::makeRigid(1, 10);
    api::confineFloorMassOnDiaphragmNode(1, true);

    // create analytical model
    physicalModel::Building::getInstance().toAnalyticalModel();

    // floating-point comparison tolerance
    const double epsilon = 1e-6;

    // retrieve translational and rotational masses for the diaphragm node from physical and analytical model and assert them
    auto transMassPhysical = api::getTranslationalMassForJointFromPhysicalModel(10);
    auto rotMassPhysical = api::getRotationalMassForJointFromPhysicalModel(10);
    auto transMassAnalytical = api::getTranslationalMassForJointFromAnalyticalModel(10);
    auto rotMassAnalytical = api::getRotationalMassForJointFromAnalyticalModel(10);

    auto expectedTransMass = utility::Vector3{ 90, 0, 0 };
    auto expectedRotMass = utility::Vector3{ 0, 0, 0 };

    EXPECT_NEAR(expectedTransMass.x, transMassPhysical.value().x, epsilon);
    EXPECT_NEAR(expectedTransMass.y, transMassPhysical.value().y, epsilon);
    EXPECT_NEAR(expectedTransMass.z, transMassPhysical.value().z, epsilon);
    EXPECT_NEAR(expectedTransMass.x, transMassAnalytical.x, epsilon);
    EXPECT_NEAR(expectedTransMass.y, transMassAnalytical.y, epsilon);
    EXPECT_NEAR(expectedTransMass.z, transMassAnalytical.z, epsilon);
    ASSERT_EQ(std::nullopt, rotMassPhysical);
    EXPECT_NEAR(expectedRotMass.x, rotMassAnalytical.x, epsilon);
    EXPECT_NEAR(expectedRotMass.y, rotMassAnalytical.y, epsilon);
    EXPECT_NEAR(expectedRotMass.z, rotMassAnalytical.z, epsilon);

    // retrieve diaphragm mass and mass center for a floor
    auto diaphragmMass = api::getDiaphragmMass(1);
    auto massCenter = api::getMassCenter(1);
    
    auto expectedDiaphragmMass = utility::Vector3{ 90, 0, 0 };
    auto expectedMassCenter = utility::Vector2{ 3, 3 };
    
    EXPECT_NEAR(expectedDiaphragmMass.x, diaphragmMass.value().x, epsilon);
    EXPECT_NEAR(expectedDiaphragmMass.y, diaphragmMass.value().y, epsilon);
    EXPECT_NEAR(expectedDiaphragmMass.z, diaphragmMass.value().z, epsilon);
    EXPECT_NEAR(expectedMassCenter.x, massCenter.value().x, epsilon);
    EXPECT_NEAR(expectedMassCenter.y, massCenter.value().y, epsilon);
    
    // Check to make sure physical model masses are not transferred to analytical model when those
    // masses are confined to diaphragm node
    transMassPhysical = api::getTranslationalMassForJointFromPhysicalModel(1);
    rotMassPhysical = api::getRotationalMassForJointFromPhysicalModel(1);
    transMassAnalytical = api::getTranslationalMassForJointFromAnalyticalModel(1);
    rotMassAnalytical = api::getRotationalMassForJointFromAnalyticalModel(1);
    
    auto expectedMass = utility::Vector3{ 10, 0, 0 };
    auto expectedDummyVec = utility::Vector3{ 0, 0, 0 };
    
    EXPECT_NEAR(expectedMass.x, transMassPhysical.value().x, epsilon);
    EXPECT_NEAR(expectedMass.y, transMassPhysical.value().y, epsilon);
    EXPECT_NEAR(expectedMass.z, transMassPhysical.value().z, epsilon);
    EXPECT_NEAR(expectedDummyVec.x, transMassAnalytical.x, epsilon);
    EXPECT_NEAR(expectedDummyVec.y, transMassAnalytical.y, epsilon);
    EXPECT_NEAR(expectedDummyVec.z, transMassAnalytical.z, epsilon);
    ASSERT_EQ(std::nullopt, rotMassPhysical);
    EXPECT_NEAR(expectedDummyVec.x, rotMassAnalytical.x, epsilon);
    EXPECT_NEAR(expectedDummyVec.y, rotMassAnalytical.y, epsilon);
    EXPECT_NEAR(expectedDummyVec.z, rotMassAnalytical.z, epsilon);
}

TEST_F(RigidDiaphragmTests, DiaphragmMassYSuccess) {
    typedef buildingModeler::BuildingModelerAPI api;

    // add joints
    api::addJoint(1, { 0, 0, 0 });
    api::addJoint(2, { 2, 0, 0 });
    api::addJoint(3, { 7, 0, 0 });
    api::addJoint(4, { 0, 2, 0 });
    api::addJoint(5, { 2, 2, 0 });
    api::addJoint(6, { 7, 2, 0 });
    api::addJoint(7, { 0, 7, 0 });
    api::addJoint(8, { 2, 7, 0 });
    api::addJoint(9, { 7, 7, 0 });

    // add masses
    api::addTranslationalMass(1, { 0.0, 10.0, 0.0 });
    api::addTranslationalMass(2, { 0.0, 10.0, 0.0 });
    api::addTranslationalMass(3, { 0.0, 10.0, 0.0 });
    api::addTranslationalMass(4, { 0.0, 10.0, 0.0 });
    api::addTranslationalMass(5, { 0.0, 10.0, 0.0 });
    api::addTranslationalMass(6, { 0.0, 10.0, 0.0 });
    api::addTranslationalMass(7, { 0.0, 10.0, 0.0 });
    api::addTranslationalMass(8, { 0.0, 10.0, 0.0 });
    api::addTranslationalMass(9, { 0.0, 10.0, 0.0 });

    // add floors
    api::addFloor(1, 0.0);

    // assign nodes to floor
    api::setFloorNo(1, 1);
    api::setFloorNo(2, 1);
    api::setFloorNo(3, 1);
    api::setFloorNo(4, 1);
    api::setFloorNo(5, 1);
    api::setFloorNo(6, 1);
    api::setFloorNo(7, 1);
    api::setFloorNo(8, 1);
    api::setFloorNo(9, 1);

    // make rigid
    api::makeRigid(1, 10);
    api::confineFloorMassOnDiaphragmNode(1, true);

    // create analytical model
    physicalModel::Building::getInstance().toAnalyticalModel();

    // floating-point comparison tolerance
    const double epsilon = 1e-6;

    // retrieve translational and rotational masses for the diaphragm node from physical and analytical model and assert them
    auto transMassPhysical = api::getTranslationalMassForJointFromPhysicalModel(10);
    auto rotMassPhysical = api::getRotationalMassForJointFromPhysicalModel(10);
    auto transMassAnalytical = api::getTranslationalMassForJointFromAnalyticalModel(10);
    auto rotMassAnalytical = api::getRotationalMassForJointFromAnalyticalModel(10);

    auto expectedTransMass = utility::Vector3{ 0, 90, 0 };
    auto expectedRotMass = utility::Vector3{ 0, 0, 0 };

    EXPECT_NEAR(expectedTransMass.x, transMassPhysical.value().x, epsilon);
    EXPECT_NEAR(expectedTransMass.y, transMassPhysical.value().y, epsilon);
    EXPECT_NEAR(expectedTransMass.z, transMassPhysical.value().z, epsilon);
    EXPECT_NEAR(expectedTransMass.x, transMassAnalytical.x, epsilon);
    EXPECT_NEAR(expectedTransMass.y, transMassAnalytical.y, epsilon);
    EXPECT_NEAR(expectedTransMass.z, transMassAnalytical.z, epsilon);
    ASSERT_EQ(std::nullopt, rotMassPhysical);
    EXPECT_NEAR(expectedRotMass.x, rotMassAnalytical.x, epsilon);
    EXPECT_NEAR(expectedRotMass.y, rotMassAnalytical.y, epsilon);
    EXPECT_NEAR(expectedRotMass.z, rotMassAnalytical.z, epsilon);

    // retrieve diaphragm mass and mass center for a floor
    auto diaphragmMass = api::getDiaphragmMass(1);
    auto massCenter = api::getMassCenter(1);

    auto expectedDiaphragmMass = utility::Vector3{ 0, 90, 0 };
    auto expectedMassCenter = utility::Vector2{ 3, 3 };

    EXPECT_NEAR(expectedDiaphragmMass.x, diaphragmMass.value().x, epsilon);
    EXPECT_NEAR(expectedDiaphragmMass.y, diaphragmMass.value().y, epsilon);
    EXPECT_NEAR(expectedDiaphragmMass.z, diaphragmMass.value().z, epsilon);
    EXPECT_NEAR(expectedMassCenter.x, massCenter.value().x, epsilon);
    EXPECT_NEAR(expectedMassCenter.y, massCenter.value().y, epsilon);

    // Check to make sure physical model masses are not transferred to analytical model when those
    // masses are confined to diaphragm node
    transMassPhysical = api::getTranslationalMassForJointFromPhysicalModel(1);
    rotMassPhysical = api::getRotationalMassForJointFromPhysicalModel(1);
    transMassAnalytical = api::getTranslationalMassForJointFromAnalyticalModel(1);
    rotMassAnalytical = api::getRotationalMassForJointFromAnalyticalModel(1);

    auto expectedMass = utility::Vector3{ 0, 10, 0 };
    auto expectedDummyVec = utility::Vector3{ 0, 0, 0 };

    EXPECT_NEAR(expectedMass.x, transMassPhysical.value().x, epsilon);
    EXPECT_NEAR(expectedMass.y, transMassPhysical.value().y, epsilon);
    EXPECT_NEAR(expectedMass.z, transMassPhysical.value().z, epsilon);
    EXPECT_NEAR(expectedDummyVec.x, transMassAnalytical.x, epsilon);
    EXPECT_NEAR(expectedDummyVec.y, transMassAnalytical.y, epsilon);
    EXPECT_NEAR(expectedDummyVec.z, transMassAnalytical.z, epsilon);
    ASSERT_EQ(std::nullopt, rotMassPhysical);
    EXPECT_NEAR(expectedDummyVec.x, rotMassAnalytical.x, epsilon);
    EXPECT_NEAR(expectedDummyVec.y, rotMassAnalytical.y, epsilon);
    EXPECT_NEAR(expectedDummyVec.z, rotMassAnalytical.z, epsilon);
}

TEST_F(RigidDiaphragmTests, MasterJointAlreadyExistsFail) {
    typedef buildingModeler::BuildingModelerAPI api;

    // add joints
    api::addJoint(1, { 0, 0, 0 });
    api::addJoint(2, { 2, 0, 0 });
    api::addJoint(3, { 7, 0, 0 });
    api::addJoint(4, { 0, 2, 0 });
    api::addJoint(5, { 2, 2, 0 });
    api::addJoint(6, { 7, 2, 0 });
    api::addJoint(7, { 0, 7, 0 });
    api::addJoint(8, { 2, 7, 0 });
    api::addJoint(9, { 7, 7, 0 });

    // add masses
    api::addTranslationalMass(1, { 0.0, 10.0, 0.0 });
    api::addTranslationalMass(2, { 0.0, 10.0, 0.0 });
    api::addTranslationalMass(3, { 0.0, 10.0, 0.0 });
    api::addTranslationalMass(4, { 0.0, 10.0, 0.0 });
    api::addTranslationalMass(5, { 0.0, 10.0, 0.0 });
    api::addTranslationalMass(6, { 0.0, 10.0, 0.0 });
    api::addTranslationalMass(7, { 0.0, 10.0, 0.0 });
    api::addTranslationalMass(8, { 0.0, 10.0, 0.0 });
    api::addTranslationalMass(9, { 0.0, 10.0, 0.0 });

    // add floors
    api::addFloor(1, 0.0);

    // assign nodes to floor
    api::setFloorNo(1, 1);
    api::setFloorNo(2, 1);
    api::setFloorNo(3, 1);
    api::setFloorNo(4, 1);
    api::setFloorNo(5, 1);
    api::setFloorNo(6, 1);
    api::setFloorNo(7, 1);
    api::setFloorNo(8, 1);
    api::setFloorNo(9, 1);

    // make rigid
    api::makeRigid(1, 10);

    try {
        api::makeRigid(1, 10);
        FAIL() << "Expected buildingModeler::EntityFoundException";
    }
    catch (const buildingModeler::EntityFoundException& e) {
        std::string expected = "Master joint with tag 10 for the floor 1 already exists. Please make floor flexible than try making rigid with a specified master joint tag again.";
        std::string actual = e.what();
        EXPECT_EQ(expected, actual);
    }
    catch (...) {
        FAIL() << "Expected buildingModeler::EntityFoundException";
    }
}

TEST_F(RigidDiaphragmTests, FloorNotFoundFail) {
    typedef buildingModeler::BuildingModelerAPI api;

    // add joints
    api::addJoint(1, { 0, 0, 0 });
    api::addJoint(2, { 2, 0, 0 });
    api::addJoint(3, { 7, 0, 0 });
    api::addJoint(4, { 0, 2, 0 });
    api::addJoint(5, { 2, 2, 0 });
    api::addJoint(6, { 7, 2, 0 });
    api::addJoint(7, { 0, 7, 0 });
    api::addJoint(8, { 2, 7, 0 });
    api::addJoint(9, { 7, 7, 0 });

    // add masses
    api::addTranslationalMass(1, { 0.0, 10.0, 0.0 });
    api::addTranslationalMass(2, { 0.0, 10.0, 0.0 });
    api::addTranslationalMass(3, { 0.0, 10.0, 0.0 });
    api::addTranslationalMass(4, { 0.0, 10.0, 0.0 });
    api::addTranslationalMass(5, { 0.0, 10.0, 0.0 });
    api::addTranslationalMass(6, { 0.0, 10.0, 0.0 });
    api::addTranslationalMass(7, { 0.0, 10.0, 0.0 });
    api::addTranslationalMass(8, { 0.0, 10.0, 0.0 });
    api::addTranslationalMass(9, { 0.0, 10.0, 0.0 });

    // add floors
    api::addFloor(1, 0.0);

    // assign nodes to floor
    api::setFloorNo(1, 1);
    api::setFloorNo(2, 1);
    api::setFloorNo(3, 1);
    api::setFloorNo(4, 1);
    api::setFloorNo(5, 1);
    api::setFloorNo(6, 1);
    api::setFloorNo(7, 1);
    api::setFloorNo(8, 1);
    api::setFloorNo(9, 1);


    try {
        api::makeRigid(2, 10);
        FAIL() << "Expected buildingModeler::EntityNotFoundException";
    }
    catch (const buildingModeler::EntityNotFoundException& e) {
        std::string expected = "Floor number 2 does not exist.";
        std::string actual = e.what();
        EXPECT_EQ(expected, actual);
    }
    catch (...) {
        FAIL() << "Expected buildingModeler::EntityNotFoundException";
    }
}

TEST_F(RigidDiaphragmTests, FloorAlreadyRigidFail) {
    typedef buildingModeler::BuildingModelerAPI api;

    // add joints
    api::addJoint(1, { 0, 0, 0 });
    api::addJoint(2, { 2, 0, 0 });
    api::addJoint(3, { 7, 0, 0 });
    api::addJoint(4, { 0, 2, 0 });
    api::addJoint(5, { 2, 2, 0 });
    api::addJoint(6, { 7, 2, 0 });
    api::addJoint(7, { 0, 7, 0 });
    api::addJoint(8, { 2, 7, 0 });
    api::addJoint(9, { 7, 7, 0 });

    // add masses
    api::addTranslationalMass(1, { 0.0, 10.0, 0.0 });
    api::addTranslationalMass(2, { 0.0, 10.0, 0.0 });
    api::addTranslationalMass(3, { 0.0, 10.0, 0.0 });
    api::addTranslationalMass(4, { 0.0, 10.0, 0.0 });
    api::addTranslationalMass(5, { 0.0, 10.0, 0.0 });
    api::addTranslationalMass(6, { 0.0, 10.0, 0.0 });
    api::addTranslationalMass(7, { 0.0, 10.0, 0.0 });
    api::addTranslationalMass(8, { 0.0, 10.0, 0.0 });
    api::addTranslationalMass(9, { 0.0, 10.0, 0.0 });

    // add floors
    api::addFloor(1, 0.0);

    // assign nodes to floor
    api::setFloorNo(1, 1);
    api::setFloorNo(2, 1);
    api::setFloorNo(3, 1);
    api::setFloorNo(4, 1);
    api::setFloorNo(5, 1);
    api::setFloorNo(6, 1);
    api::setFloorNo(7, 1);
    api::setFloorNo(8, 1);
    api::setFloorNo(9, 1);

    // make rigid
    api::makeRigid(1, 10);

    try {
        api::makeRigid(1, 11);
        FAIL() << "Expected buildingModeler::InvalidOperationException";
    }
    catch (const buildingModeler::InvalidOperationException& e) {
        std::string expected = "Floor 1 is already rigid.";
        std::string actual = e.what();
        EXPECT_EQ(expected, actual);
    }
    catch (...) {
        FAIL() << "Expected buildingModeler::InvalidOperationException";
    }
}

TEST_F(RigidDiaphragmTests, NoMassFail) {
    typedef buildingModeler::BuildingModelerAPI api;

    // add joints
    api::addJoint(1, { 0, 0, 0 });
    api::addJoint(2, { 2, 0, 0 });
    api::addJoint(3, { 7, 0, 0 });
    api::addJoint(4, { 0, 2, 0 });
    api::addJoint(5, { 2, 2, 0 });
    api::addJoint(6, { 7, 2, 0 });
    api::addJoint(7, { 0, 7, 0 });
    api::addJoint(8, { 2, 7, 0 });
    api::addJoint(9, { 7, 7, 0 });

    // add floors
    api::addFloor(1, 0.0);

    // assign nodes to floor
    api::setFloorNo(1, 1);
    api::setFloorNo(2, 1);
    api::setFloorNo(3, 1);
    api::setFloorNo(4, 1);
    api::setFloorNo(5, 1);
    api::setFloorNo(6, 1);
    api::setFloorNo(7, 1);
    api::setFloorNo(8, 1);
    api::setFloorNo(9, 1);

    try {
        api::makeRigid(1, 10);
        FAIL() << "Expected buildingModeler::InvalidOperationException";
    }
    catch (const buildingModeler::InvalidOperationException& e) {
        std::string expected = "Floor 1 has no mass.";
        std::string actual = e.what();
        EXPECT_EQ(expected, actual);
    }
    catch (...) {
        FAIL() << "Expected buildingModeler::InvalidOperationException";
    }
}

TEST_F(RigidDiaphragmTests, FloorNotFoundFlexibleFail) {
    typedef buildingModeler::BuildingModelerAPI api;

    // add joints
    api::addJoint(1, { 0, 0, 0 });
    api::addJoint(2, { 2, 0, 0 });
    api::addJoint(3, { 7, 0, 0 });
    api::addJoint(4, { 0, 2, 0 });
    api::addJoint(5, { 2, 2, 0 });
    api::addJoint(6, { 7, 2, 0 });
    api::addJoint(7, { 0, 7, 0 });
    api::addJoint(8, { 2, 7, 0 });
    api::addJoint(9, { 7, 7, 0 });

    try {
        api::makeFlexible(1);
        FAIL() << "Expected buildingModeler::EntityNotFoundException";
    }
    catch (const buildingModeler::EntityNotFoundException& e) {
        std::string expected = "Floor number 1 does not exist.";
        std::string actual = e.what();
        EXPECT_EQ(expected, actual);
    }
    catch (...) {
        FAIL() << "Expected buildingModeler::EntityNotFoundException";
    }
}

TEST_F(RigidDiaphragmTests, FloorAlreadyFlexibleFail) {
    typedef buildingModeler::BuildingModelerAPI api;

    // add joints
    api::addJoint(1, { 0, 0, 0 });
    api::addJoint(2, { 2, 0, 0 });
    api::addJoint(3, { 7, 0, 0 });
    api::addJoint(4, { 0, 2, 0 });
    api::addJoint(5, { 2, 2, 0 });
    api::addJoint(6, { 7, 2, 0 });
    api::addJoint(7, { 0, 7, 0 });
    api::addJoint(8, { 2, 7, 0 });
    api::addJoint(9, { 7, 7, 0 });

    // add floors
    api::addFloor(1, 0.0);

    // assign nodes to floor
    api::setFloorNo(1, 1);
    api::setFloorNo(2, 1);
    api::setFloorNo(3, 1);
    api::setFloorNo(4, 1);
    api::setFloorNo(5, 1);
    api::setFloorNo(6, 1);
    api::setFloorNo(7, 1);
    api::setFloorNo(8, 1);
    api::setFloorNo(9, 1);

    try {
        api::makeFlexible(1);
        FAIL() << "Expected buildingModeler::InvalidOperationException";
    }
    catch (const buildingModeler::InvalidOperationException& e) {
        std::string expected = "Floor number 1 is already flexible.";
        std::string actual = e.what();
        EXPECT_EQ(expected, actual);
    }
    catch (...) {
        FAIL() << "Expected buildingModeler::InvalidOperationException";
    }
}

TEST_F(RigidDiaphragmTests, FloorNotFoundConfineFail) {
    typedef buildingModeler::BuildingModelerAPI api;

    // add joints
    api::addJoint(1, { 0, 0, 0 });
    api::addJoint(2, { 2, 0, 0 });
    api::addJoint(3, { 7, 0, 0 });
    api::addJoint(4, { 0, 2, 0 });
    api::addJoint(5, { 2, 2, 0 });
    api::addJoint(6, { 7, 2, 0 });
    api::addJoint(7, { 0, 7, 0 });
    api::addJoint(8, { 2, 7, 0 });
    api::addJoint(9, { 7, 7, 0 });

    // add masses
    api::addTranslationalMass(1, { 0.0, 10.0, 0.0 });
    api::addTranslationalMass(2, { 0.0, 10.0, 0.0 });
    api::addTranslationalMass(3, { 0.0, 10.0, 0.0 });
    api::addTranslationalMass(4, { 0.0, 10.0, 0.0 });
    api::addTranslationalMass(5, { 0.0, 10.0, 0.0 });
    api::addTranslationalMass(6, { 0.0, 10.0, 0.0 });
    api::addTranslationalMass(7, { 0.0, 10.0, 0.0 });
    api::addTranslationalMass(8, { 0.0, 10.0, 0.0 });
    api::addTranslationalMass(9, { 0.0, 10.0, 0.0 });

    // add floors
    api::addFloor(1, 0.0);

    // assign nodes to floor
    api::setFloorNo(1, 1);
    api::setFloorNo(2, 1);
    api::setFloorNo(3, 1);
    api::setFloorNo(4, 1);
    api::setFloorNo(5, 1);
    api::setFloorNo(6, 1);
    api::setFloorNo(7, 1);
    api::setFloorNo(8, 1);
    api::setFloorNo(9, 1);

    // make rigid
    api::makeRigid(1, 10);

    try {
        api::confineFloorMassOnDiaphragmNode(2, true);
        FAIL() << "Expected buildingModeler::EntityNotFoundException";
    }
    catch (const buildingModeler::EntityNotFoundException& e) {
        std::string expected = "Floor number 2 does not exist.";
        std::string actual = e.what();
        EXPECT_EQ(expected, actual);
    }
    catch (...) {
        FAIL() << "Expected buildingModeler::EntityNotFoundException";
    }
}

TEST_F(RigidDiaphragmTests, FlexibleFloorNotConfinedFail) {
    typedef buildingModeler::BuildingModelerAPI api;

    // add joints
    api::addJoint(1, { 0, 0, 0 });
    api::addJoint(2, { 2, 0, 0 });
    api::addJoint(3, { 7, 0, 0 });
    api::addJoint(4, { 0, 2, 0 });
    api::addJoint(5, { 2, 2, 0 });
    api::addJoint(6, { 7, 2, 0 });
    api::addJoint(7, { 0, 7, 0 });
    api::addJoint(8, { 2, 7, 0 });
    api::addJoint(9, { 7, 7, 0 });

    // add masses
    api::addTranslationalMass(1, { 0.0, 10.0, 0.0 });
    api::addTranslationalMass(2, { 0.0, 10.0, 0.0 });
    api::addTranslationalMass(3, { 0.0, 10.0, 0.0 });
    api::addTranslationalMass(4, { 0.0, 10.0, 0.0 });
    api::addTranslationalMass(5, { 0.0, 10.0, 0.0 });
    api::addTranslationalMass(6, { 0.0, 10.0, 0.0 });
    api::addTranslationalMass(7, { 0.0, 10.0, 0.0 });
    api::addTranslationalMass(8, { 0.0, 10.0, 0.0 });
    api::addTranslationalMass(9, { 0.0, 10.0, 0.0 });

    // add floors
    api::addFloor(1, 0.0);

    // assign nodes to floor
    api::setFloorNo(1, 1);
    api::setFloorNo(2, 1);
    api::setFloorNo(3, 1);
    api::setFloorNo(4, 1);
    api::setFloorNo(5, 1);
    api::setFloorNo(6, 1);
    api::setFloorNo(7, 1);
    api::setFloorNo(8, 1);
    api::setFloorNo(9, 1);

    try {
        api::confineFloorMassOnDiaphragmNode(1, true);
        FAIL() << "Expected buildingModeler::InvalidOperationException";
    }
    catch (const buildingModeler::InvalidOperationException& e) {
        std::string expected = "Floor mass can be confined into diaphragm node only in rigid floors. Floor 1 is not rigid.";
        std::string actual = e.what();
        EXPECT_EQ(expected, actual);
    }
    catch (...) {
        FAIL() << "Expected buildingModeler::InvalidOperationException";
    }
}
