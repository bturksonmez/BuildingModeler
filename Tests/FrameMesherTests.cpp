#include <gtest/gtest.h>
#include "../BuildingModeler/BuildingModelerAPI.h"
#include "../BuildingModeler/Utilities/Vector3.h"

TEST(FrameMesherTests, ElementNotFoundFail) {
    typedef buildingModeler::BuildingModelerAPI api;

    // add line element joints
    api::addJoint(1, { 0, 0, 0 });
    api::addJoint(2, { 10, 0, 0 });

    // add material
    api::addElasticMaterial(1, 20, 20, 2);

    // add section
    api::addElasticSection1D(1, 1, 0.2, 0.02, 0.02, 0.04);

    try {
        api::setSegmentRatios(1, { 0.2, 0.4, 0.2, 0.2 });
        FAIL() << "Expected buildingModeler::EntityNotFoundException";
    }
    catch (const buildingModeler::EntityNotFoundException& e) {
        std::string expected = "Line element with tag 1 does not exist.";
        std::string actual = e.what();
        EXPECT_EQ(expected, actual);
    }
    catch (...) {
        FAIL() << "Expected buildingModeler::EntityNotFoundException";
    }
}

TEST(FrameMesherTests, SegmentRatiosIncorrectFail) {
    typedef buildingModeler::BuildingModelerAPI api;

    // add line element joints
    api::addJoint(1, { 0, 0, 0 });
    api::addJoint(2, { 10, 0, 0 });

    // add material
    api::addElasticMaterial(1, 20, 20, 2);

    // add section
    api::addElasticSection1D(1, 1, 0.2, 0.02, 0.02, 0.04);

    // add element
    api::addBeam(1, { 1, 2 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);

    try {
        api::setSegmentRatios(1, { 0.2, 0.4, 0.2, 0.1 });
        FAIL() << "Expected buildingModeler::InvalidInputException";
    }
    catch (const buildingModeler::InvalidInputException& e) {
        std::string expected = "Segment ratios should sum up to 1.0";
        std::string actual = e.what();
        EXPECT_EQ(expected, actual);
    }
    catch (...) {
        FAIL() << "Expected buildingModeler::InvalidInputException";
    }
}

TEST(FrameMesherTests, MeshSuccess) {
    typedef buildingModeler::BuildingModelerAPI api;

    // add line element joints
    api::addJoint(1, { 0, 0, 0 });
    api::addJoint(2, { 15, 5, 10 });

    // add material
    api::addElasticMaterial(1, 20, 20, 2);

    // add section
    api::addElasticSection1D(1, 1, 0.2, 0.02, 0.02, 0.04);

    // add element
    api::addBeam(1, { 1, 2 }, 1, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);

    // set segment ratios
    api::setSegmentRatios(1, { 0.2, 0.4, 0.2, 0.2 });

    // create analytical model
    physicalModel::Building::getInstance().toAnalyticalModel();

    // retrieve node coords
    auto nodeCoords = api::getNodeCoordinatesOfLineElement(1);

    // expected values
    std::vector<utility::Vector3> expectedCoords{ {0.0, 0.0, 0.0}, {3.0, 1.0, 2.0}, {9.0, 3.0, 6.0}, {12.0, 4.0, 8.0}, { 15.0, 5.0, 10.0 } };

    // ensure the vectors have the same length
    ASSERT_EQ(nodeCoords.size(), expectedCoords.size()) << "Vectors differ in size.";

    // floating-point comparison tolerance
    const double epsilon = 1e-7;

    for (size_t i = 0; i < nodeCoords.size(); ++i) {
        EXPECT_NEAR(expectedCoords[i].x, nodeCoords[i].x, epsilon) << "X coordinate mismatch at index " << i;
        EXPECT_NEAR(expectedCoords[i].y, nodeCoords[i].y, epsilon) << "Y coordinate mismatch at index " << i;
        EXPECT_NEAR(expectedCoords[i].z, nodeCoords[i].z, epsilon) << "Z coordinate mismatch at index " << i;
    }
}