#include <gtest/gtest.h>
#include "../BuildingModeler/BuildingModelerAPI.h"
#include "../BuildingModeler/Utilities/Vector3.h"

class QuadMesherTests : public ::testing::Test {
protected:
    void TearDown() override {
        typedef buildingModeler::BuildingModelerAPI api;

        api::clear();
    }
};

TEST_F(QuadMesherTests, ElementNotFoundFail) {
    typedef buildingModeler::BuildingModelerAPI api;

    // add quad joints
    api::addJoint(1, { 1, 0, 0 });
    api::addJoint(2, { 2, 0, 0 });
    api::addJoint(3, { 2, 1, 0 });
    api::addJoint(4, { 1, 1, 0 });

    // add material
    api::addElasticMaterial(1, 20, 20, 2);

    // add section
    api::addElasticSection2D(1, 1, 2.0);

    try {
        api::meshAreaElement(1, -1, -1);
        FAIL() << "Expected buildingModeler::EntityNotFoundException";
    }
    catch (const buildingModeler::EntityNotFoundException& e) {
        std::string expected = "Area element with tag 1 does not exist.";
        std::string actual = e.what();
        EXPECT_EQ(expected, actual);
    }
    catch (...) {
        FAIL() << "Expected buildingModeler::EntityNotFoundException";
    }
}

TEST_F(QuadMesherTests, NoSurroundingElementsWithNullN1Fail) {
    typedef buildingModeler::BuildingModelerAPI api;

    // add quad joints
    api::addJoint(1, { 1, 0, 0 });
    api::addJoint(2, { 2, 0, 0 });
    api::addJoint(3, { 2, 1, 0 });
    api::addJoint(4, { 1, 1, 0 });

    // add material
    api::addElasticMaterial(1, 20, 20, 2);

    // add section
    api::addElasticSection2D(1, 1, 2.0);

    // add element
    api::addShearWall(1, { 1, 2, 3, 4 }, 1, physicalModel::AreaElementFormulation::LINEAR);

    try {
        api::meshAreaElement(1);
        FAIL() << "Expected buildingModeler::InvalidInputException";
    }
    catch (const buildingModeler::InvalidInputException& e) {
        std::string expected = "There must be surrounding elements on the opposite sides when n1 is not assigned.";
        std::string actual = e.what();
        EXPECT_EQ(expected, actual);
    }
    catch (...) {
        FAIL() << "Expected buildingModeler::InvalidInputException";
    }
}

TEST_F(QuadMesherTests, UnbalancedSurroundingElementsWithNullN1Fail) {
    typedef buildingModeler::BuildingModelerAPI api;

    // add quad joints
    api::addJoint(1, { 1, 0, 0 });
    api::addJoint(2, { 2, 0, 0 });
    api::addJoint(3, { 3, 1, 0 });
    api::addJoint(4, { 1, 1, 0 });

    // add material
    api::addElasticMaterial(1, 20, 20, 2);

    // add section 2D
    api::addElasticSection2D(1, 1, 2.0);

    // add area element
    api::addShearWall(1, { 1, 2, 3, 4 }, 1, physicalModel::AreaElementFormulation::LINEAR);

    // add section 1D
    api::addElasticSection1D(2, 1, 0.2, 0.02, 0.02, 0.04);

    // add line element
    api::addBeam(1, { 1, 2 }, 2, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);

    // set segment ratios
    api::setSegmentRatios(1, { 1.0 / 3.0, 1.0 / 3.0, 1.0 / 3.0 });

    // update connectivity between line and area elements
    api::updateAreaElementProperties();

    try {
        api::meshAreaElement(1, std::nullopt, 2);
        FAIL() << "Expected buildingModeler::InvalidInputException";
    }
    catch (const buildingModeler::InvalidInputException& e) {
        std::string expected = "There must be surrounding elements on the opposite sides when n1 is not assigned.";
        std::string actual = e.what();
        EXPECT_EQ(expected, actual);
    }
    catch (...) {
        FAIL() << "Expected buildingModeler::InvalidInputException";
    }
}

TEST_F(QuadMesherTests, NoSurroundingElementsWithMinusN1Fail) {
    typedef buildingModeler::BuildingModelerAPI api;

    // add quad joints
    api::addJoint(1, { 1, 0, 0 });
    api::addJoint(2, { 2, 0, 0 });
    api::addJoint(3, { 2, 1, 0 });
    api::addJoint(4, { 1, 1, 0 });

    // add material
    api::addElasticMaterial(1, 20, 20, 2);

    // add section
    api::addElasticSection2D(1, 1, 2.0);

    // add element
    api::addShearWall(1, { 1, 2, 3, 4 }, 1, physicalModel::AreaElementFormulation::LINEAR);

    try {
        api::meshAreaElement(1, -1);
        FAIL() << "Expected buildingModeler::InvalidInputException";
    }
    catch (const buildingModeler::InvalidInputException& e) {
        std::string expected = "n1 value cannot be less than 1.";
        std::string actual = e.what();
        EXPECT_EQ(expected, actual);
    }
    catch (...) {
        FAIL() << "Expected buildingModeler::InvalidInputException";
    }
}

TEST_F(QuadMesherTests, SurroundingElementsWithN1Fail) {
    typedef buildingModeler::BuildingModelerAPI api;

    // add quad joints
    api::addJoint(1, { 1, 0, 0 });
    api::addJoint(2, { 2, 0, 0 });
    api::addJoint(3, { 3, 1, 0 });
    api::addJoint(4, { 1, 1, 0 });

    // add material
    api::addElasticMaterial(1, 20, 20, 2);

    // add section 2D
    api::addElasticSection2D(1, 1, 2.0);

    // add area element
    api::addShearWall(1, { 1, 2, 3, 4 }, 1, physicalModel::AreaElementFormulation::LINEAR);

    // add section 1D
    api::addElasticSection1D(2, 1, 0.2, 0.02, 0.02, 0.04);

    // add line element
    api::addBeam(1, { 1, 2 }, 2, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(3, { 3, 4 }, 2, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);

    // set segment ratios
    api::setSegmentRatios(1, { 1.0 / 3.0, 1.0 / 3.0, 1.0 / 3.0 });
    api::setSegmentRatios(3, { 1.0 / 3.0, 1.0 / 3.0, 1.0 / 3.0 });

    // update connectivity between line and area elements
    api::updateAreaElementProperties();

    try {
        api::meshAreaElement(1, 2, 2);
        FAIL() << "Expected buildingModeler::InvalidInputException";
    }
    catch (const buildingModeler::InvalidInputException& e) {
        std::string expected = "n1 cannot be assigned while there are surrounding elements in that direction.";
        std::string actual = e.what();
        EXPECT_EQ(expected, actual);
    }
    catch (...) {
        FAIL() << "Expected buildingModeler::InvalidInputException";
    }
}

TEST_F(QuadMesherTests, UnbalancedSurroundingElementSegmentsFail) {
    typedef buildingModeler::BuildingModelerAPI api;

    // add quad joints
    api::addJoint(1, { 1, 0, 0 });
    api::addJoint(2, { 2, 0, 0 });
    api::addJoint(3, { 3, 1, 0 });
    api::addJoint(4, { 1, 1, 0 });

    // add material
    api::addElasticMaterial(1, 20, 20, 2);

    // add section 2D
    api::addElasticSection2D(1, 1, 2.0);

    // add area element
    api::addShearWall(1, { 1, 2, 3, 4 }, 1, physicalModel::AreaElementFormulation::LINEAR);

    // add section 1D
    api::addElasticSection1D(2, 1, 0.2, 0.02, 0.02, 0.04);

    // add line element
    api::addBeam(1, { 1, 2 }, 2, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(3, { 3, 4 }, 2, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);

    // set segment ratios
    api::setSegmentRatios(1, { 1.0 / 3.0, 1.0 / 3.0, 1.0 / 3.0 });
    api::setSegmentRatios(3, { 0.5, 0.5 });

    // update connectivity between line and area elements
    api::updateAreaElementProperties();

    try {
        api::meshAreaElement(1, std::nullopt, 2);
        FAIL() << "Expected buildingModeler::InvalidInputException";
    }
    catch (const buildingModeler::InvalidInputException& e) {
        std::string expected = "Surrounding elements on the opposite sides must have the same number of elements.";
        std::string actual = e.what();
        EXPECT_EQ(expected, actual);
    }
    catch (...) {
        FAIL() << "Expected buildingModeler::InvalidInputException";
    }
}

TEST_F(QuadMesherTests, MeshWithN1nN2Success) {
    typedef buildingModeler::BuildingModelerAPI api;

    // add quad joints
    api::addJoint(1, { 1, 0, 0 });
    api::addJoint(2, { 2, 0, 0 });
    api::addJoint(3, { 3, 1, 0 });
    api::addJoint(4, { 1, 1, 0 });

    // add material
    api::addElasticMaterial(1, 20, 20, 2);

    // add section
    api::addElasticSection2D(1, 1, 2.0);

    // add element
    api::addShearWall(1, { 1, 2, 3, 4 }, 1, physicalModel::AreaElementFormulation::LINEAR);

    //mesh the element
    api::meshAreaElement(1, 3, 2);

    // create analytical model
    physicalModel::Building::getInstance().toAnalyticalModel();

    // retrieve node coords
    auto nodeCoords = api::getNodeCoordinatesOfAreaElement(1);

    // expected values
    std::vector<std::vector<utility::Vector3>> expectedCoords{ { {1.0, 0.0, 0.0}, {1.33333333, 0.0, 0.0}, {1.5, 0.5, 0.0}, {1.0, 0.5, 0.0} },
                                                               { {1.33333333, 0.0, 0.0}, {1.66666667, 0.0, 0.0}, {2.0, 0.5, 0.0}, {1.5, 0.5, 0.0} },
                                                               { {1.66666667, 0.0, 0.0}, {2.0, 0.0, 0.0}, {2.5, 0.5, 0.0}, {2.0, 0.5, 0.0} },
                                                               { {1.0, 0.5, 0.0}, {1.5, 0.5, 0.0}, {1.66666667, 1.0, 0.0}, {1.0, 1.0, 0.0} },
                                                               { {1.5, 0.5, 0.0}, {2.0, 0.5, 0.0}, {2.33333333, 1.0, 0.0}, {1.66666667, 1.0, 0.0} },
                                                               { {2.0, 0.5, 0.0}, {2.5, 0.5, 0.0}, {3.0, 1.0, 0.0}, {2.33333333, 1.0, 0.0} } };

    // ensure the vectors have the same length
    ASSERT_EQ(nodeCoords.size(), expectedCoords.size()) << "Vectors differ in size.";

    // floating-point comparison tolerance
    const double epsilon = 1e-6;

    for (size_t i = 0; i < nodeCoords.size(); ++i) {

        for (size_t j = 0; j < nodeCoords[i].size(); ++j) {
            EXPECT_NEAR(expectedCoords[i][j].x, nodeCoords[i][j].x, epsilon) << "X coordinate mismatch at index " << i << "," << j;
            EXPECT_NEAR(expectedCoords[i][j].y, nodeCoords[i][j].y, epsilon) << "Y coordinate mismatch at index " << i << "," << j;
            EXPECT_NEAR(expectedCoords[i][j].z, nodeCoords[i][j].z, epsilon) << "Z coordinate mismatch at index " << i << "," << j;
        }
    }
}

TEST_F(QuadMesherTests, NoMeshSuccess) {
    typedef buildingModeler::BuildingModelerAPI api;

    // add quad joints
    api::addJoint(1, { 1, 0, 0 });
    api::addJoint(2, { 2, 0, 0 });
    api::addJoint(3, { 3, 1, 0 });
    api::addJoint(4, { 1, 1, 0 });

    // add material
    api::addElasticMaterial(1, 20, 20, 2);

    // add section
    api::addElasticSection2D(1, 1, 2.0);

    // add element
    api::addShearWall(1, { 1, 2, 3, 4 }, 1, physicalModel::AreaElementFormulation::LINEAR);

    //mesh the element
    api::meshAreaElement(1, 1, 1);

    // create analytical model
    physicalModel::Building::getInstance().toAnalyticalModel();

    // retrieve node coords
    auto nodeCoords = api::getNodeCoordinatesOfAreaElement(1);

    // expected values
    std::vector<std::vector<utility::Vector3>> expectedCoords{ { {1.0, 0.0, 0.0}, { 2, 0, 0 }, { 3, 1, 0 }, { 1, 1, 0 } } };

    // ensure the vectors have the same length
    ASSERT_EQ(nodeCoords.size(), expectedCoords.size()) << "Vectors differ in size.";

    // floating-point comparison tolerance
    const double epsilon = 1e-6;

    for (size_t i = 0; i < nodeCoords.size(); ++i) {

        for (size_t j = 0; j < nodeCoords[i].size(); ++j) {
            EXPECT_NEAR(expectedCoords[i][j].x, nodeCoords[i][j].x, epsilon) << "X coordinate mismatch at index " << i << "," << j;
            EXPECT_NEAR(expectedCoords[i][j].y, nodeCoords[i][j].y, epsilon) << "Y coordinate mismatch at index " << i << "," << j;
            EXPECT_NEAR(expectedCoords[i][j].z, nodeCoords[i][j].z, epsilon) << "Z coordinate mismatch at index " << i << "," << j;
        }
    }
}

TEST_F(QuadMesherTests, MeshWith4SurroundingElementsSuccess) {
    typedef buildingModeler::BuildingModelerAPI api;

    // add quad joints
    api::addJoint(1, { 1, 0, 0 });
    api::addJoint(2, { 2, 0, 0 });
    api::addJoint(3, { 3, 1, 0 });
    api::addJoint(4, { 1, 1, 0 });

    // add material
    api::addElasticMaterial(1, 20, 20, 2);

    // add section 2D
    api::addElasticSection2D(1, 1, 2.0);

    // add area element
    api::addShearWall(1, { 1, 2, 3, 4 }, 1, physicalModel::AreaElementFormulation::LINEAR);

    // add section 1D
    api::addElasticSection1D(2, 1, 0.2, 0.02, 0.02, 0.04);

    // add line element
    api::addBeam(1, { 1, 2 }, 2, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(2, { 2, 3 }, 2, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(3, { 3, 4 }, 2, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(4, { 4, 1 }, 2, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);

    // set segment ratios
    api::setSegmentRatios(1, { 1.0 / 3.0, 1.0 / 3.0, 1.0 / 3.0 });
    api::setSegmentRatios(3, { 1.0 / 3.0, 1.0 / 3.0, 1.0 / 3.0 });
    api::setSegmentRatios(2, { 0.5, 0.5 });
    api::setSegmentRatios(4, { 0.5, 0.5 });

    // update connectivity between line and area elements
    api::updateAreaElementProperties();

    //mesh the element
    api::meshAreaElement(1);

    // create analytical model
    physicalModel::Building::getInstance().toAnalyticalModel();

    // retrieve node coords
    auto nodeCoords = api::getNodeCoordinatesOfAreaElement(1);

    // expected values
    std::vector<std::vector<utility::Vector3>> expectedCoords{ { {1.0, 0.0, 0.0}, {1.33333333, 0.0, 0.0}, {1.5, 0.5, 0.0}, {1.0, 0.5, 0.0} },
                                                               { {1.33333333, 0.0, 0.0}, {1.66666667, 0.0, 0.0}, {2.0, 0.5, 0.0}, {1.5, 0.5, 0.0} },
                                                               { {1.66666667, 0.0, 0.0}, {2.0, 0.0, 0.0}, {2.5, 0.5, 0.0}, {2.0, 0.5, 0.0} },
                                                               { {1.0, 0.5, 0.0}, {1.5, 0.5, 0.0}, {1.66666667, 1.0, 0.0}, {1.0, 1.0, 0.0} },
                                                               { {1.5, 0.5, 0.0}, {2.0, 0.5, 0.0}, {2.33333333, 1.0, 0.0}, {1.66666667, 1.0, 0.0} },
                                                               { {2.0, 0.5, 0.0}, {2.5, 0.5, 0.0}, {3.0, 1.0, 0.0}, {2.33333333, 1.0, 0.0} } };

    // ensure the vectors have the same length
    ASSERT_EQ(nodeCoords.size(), expectedCoords.size()) << "Vectors differ in size.";

    // floating-point comparison tolerance
    const double epsilon = 1e-6;

    for (size_t i = 0; i < nodeCoords.size(); ++i) {

        for (size_t j = 0; j < nodeCoords[i].size(); ++j) {
            EXPECT_NEAR(expectedCoords[i][j].x, nodeCoords[i][j].x, epsilon) << "X coordinate mismatch at index " << i << "," << j;
            EXPECT_NEAR(expectedCoords[i][j].y, nodeCoords[i][j].y, epsilon) << "Y coordinate mismatch at index " << i << "," << j;
            EXPECT_NEAR(expectedCoords[i][j].z, nodeCoords[i][j].z, epsilon) << "Z coordinate mismatch at index " << i << "," << j;
        }
    }
}

TEST_F(QuadMesherTests, MeshWith2SurroundingElementsN1Success) {
    typedef buildingModeler::BuildingModelerAPI api;

    // add quad joints
    api::addJoint(1, { 1, 0, 0 });
    api::addJoint(2, { 2, 0, 0 });
    api::addJoint(3, { 3, 1, 0 });
    api::addJoint(4, { 1, 1, 0 });

    // add material
    api::addElasticMaterial(1, 20, 20, 2);

    // add section 2D
    api::addElasticSection2D(1, 1, 2.0);

    // add area element
    api::addShearWall(1, { 1, 2, 3, 4 }, 1, physicalModel::AreaElementFormulation::LINEAR);

    // add section 1D
    api::addElasticSection1D(2, 1, 0.2, 0.02, 0.02, 0.04);

    // add line element
    api::addBeam(2, { 2, 3 }, 2, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(4, { 4, 1 }, 2, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);

    // set segment ratios
    api::setSegmentRatios(2, { 0.5, 0.5 });
    api::setSegmentRatios(4, { 0.5, 0.5 });

    // update connectivity between line and area elements
    api::updateAreaElementProperties();

    //mesh the element
    api::meshAreaElement(1, 3);

    // create analytical model
    physicalModel::Building::getInstance().toAnalyticalModel();

    // retrieve node coords
    auto nodeCoords = api::getNodeCoordinatesOfAreaElement(1);

    // expected values
    std::vector<std::vector<utility::Vector3>> expectedCoords{ { {1.0, 0.0, 0.0}, {1.33333333, 0.0, 0.0}, {1.5, 0.5, 0.0}, {1.0, 0.5, 0.0} },
                                                               { {1.33333333, 0.0, 0.0}, {1.66666667, 0.0, 0.0}, {2.0, 0.5, 0.0}, {1.5, 0.5, 0.0} },
                                                               { {1.66666667, 0.0, 0.0}, {2.0, 0.0, 0.0}, {2.5, 0.5, 0.0}, {2.0, 0.5, 0.0} },
                                                               { {1.0, 0.5, 0.0}, {1.5, 0.5, 0.0}, {1.66666667, 1.0, 0.0}, {1.0, 1.0, 0.0} },
                                                               { {1.5, 0.5, 0.0}, {2.0, 0.5, 0.0}, {2.33333333, 1.0, 0.0}, {1.66666667, 1.0, 0.0} },
                                                               { {2.0, 0.5, 0.0}, {2.5, 0.5, 0.0}, {3.0, 1.0, 0.0}, {2.33333333, 1.0, 0.0} } };

    // ensure the vectors have the same length
    ASSERT_EQ(nodeCoords.size(), expectedCoords.size()) << "Vectors differ in size.";

    // floating-point comparison tolerance
    const double epsilon = 1e-6;

    for (size_t i = 0; i < nodeCoords.size(); ++i) {

        for (size_t j = 0; j < nodeCoords[i].size(); ++j) {
            EXPECT_NEAR(expectedCoords[i][j].x, nodeCoords[i][j].x, epsilon) << "X coordinate mismatch at index " << i << "," << j;
            EXPECT_NEAR(expectedCoords[i][j].y, nodeCoords[i][j].y, epsilon) << "Y coordinate mismatch at index " << i << "," << j;
            EXPECT_NEAR(expectedCoords[i][j].z, nodeCoords[i][j].z, epsilon) << "Z coordinate mismatch at index " << i << "," << j;
        }
    }
}

TEST_F(QuadMesherTests, MeshWith2SurroundingElementsN2Success) {
    typedef buildingModeler::BuildingModelerAPI api;

    // add quad joints
    api::addJoint(1, { 1, 0, 0 });
    api::addJoint(2, { 2, 0, 0 });
    api::addJoint(3, { 3, 1, 0 });
    api::addJoint(4, { 1, 1, 0 });

    // add material
    api::addElasticMaterial(1, 20, 20, 2);

    // add section 2D
    api::addElasticSection2D(1, 1, 2.0);

    // add area element
    api::addShearWall(1, { 1, 2, 3, 4 }, 1, physicalModel::AreaElementFormulation::LINEAR);

    // add section 1D
    api::addElasticSection1D(2, 1, 0.2, 0.02, 0.02, 0.04);

    // add line element
    api::addBeam(1, { 1, 2 }, 2, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(3, { 3, 4 }, 2, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);

    // set segment ratios
    api::setSegmentRatios(1, { 1.0 / 3.0, 1.0 / 3.0, 1.0 / 3.0 });
    api::setSegmentRatios(3, { 1.0 / 3.0, 1.0 / 3.0, 1.0 / 3.0 });

    // update connectivity between line and area elements
    api::updateAreaElementProperties();

    //mesh the element
    api::meshAreaElement(1, std::nullopt, 2);

    // create analytical model
    physicalModel::Building::getInstance().toAnalyticalModel();

    // retrieve node coords
    auto nodeCoords = api::getNodeCoordinatesOfAreaElement(1);

    // expected values
    std::vector<std::vector<utility::Vector3>> expectedCoords{ { {1.0, 0.0, 0.0}, {1.33333333, 0.0, 0.0}, {1.5, 0.5, 0.0}, {1.0, 0.5, 0.0} },
                                                               { {1.33333333, 0.0, 0.0}, {1.66666667, 0.0, 0.0}, {2.0, 0.5, 0.0}, {1.5, 0.5, 0.0} },
                                                               { {1.66666667, 0.0, 0.0}, {2.0, 0.0, 0.0}, {2.5, 0.5, 0.0}, {2.0, 0.5, 0.0} },
                                                               { {1.0, 0.5, 0.0}, {1.5, 0.5, 0.0}, {1.66666667, 1.0, 0.0}, {1.0, 1.0, 0.0} },
                                                               { {1.5, 0.5, 0.0}, {2.0, 0.5, 0.0}, {2.33333333, 1.0, 0.0}, {1.66666667, 1.0, 0.0} },
                                                               { {2.0, 0.5, 0.0}, {2.5, 0.5, 0.0}, {3.0, 1.0, 0.0}, {2.33333333, 1.0, 0.0} } };

    // ensure the vectors have the same length
    ASSERT_EQ(nodeCoords.size(), expectedCoords.size()) << "Vectors differ in size.";

    // floating-point comparison tolerance
    const double epsilon = 1e-6;

    for (size_t i = 0; i < nodeCoords.size(); ++i) {

        for (size_t j = 0; j < nodeCoords[i].size(); ++j) {
            EXPECT_NEAR(expectedCoords[i][j].x, nodeCoords[i][j].x, epsilon) << "X coordinate mismatch at index " << i << "," << j;
            EXPECT_NEAR(expectedCoords[i][j].y, nodeCoords[i][j].y, epsilon) << "Y coordinate mismatch at index " << i << "," << j;
            EXPECT_NEAR(expectedCoords[i][j].z, nodeCoords[i][j].z, epsilon) << "Z coordinate mismatch at index " << i << "," << j;
        }
    }
}

TEST_F(QuadMesherTests, MeshWithN1nN2TwoElementsSuccess) {
    typedef buildingModeler::BuildingModelerAPI api;

    // add quad joints
    api::addJoint(1, { 1, 0, 0 });
    api::addJoint(2, { 2, 0, 0 });
    api::addJoint(3, { 2, 1, 0 });
    api::addJoint(4, { 1, 1, 0 });
    api::addJoint(5, { 3, 0, 0 });
    api::addJoint(6, { 3, 1, 0 });

    // add material
    api::addElasticMaterial(1, 20, 20, 2);

    // add section 2D
    api::addElasticSection2D(1, 1, 2.0);

    // add area element
    api::addShearWall(1, { 1, 2, 3, 4 }, 1, physicalModel::AreaElementFormulation::LINEAR);
    api::addShearWall(2, { 2, 5, 6, 3 }, 1, physicalModel::AreaElementFormulation::LINEAR);

    //mesh the element
    api::meshAreaElement(1, 2, 2);
    api::meshAreaElement(2, 2, 2);

    // create analytical model
    physicalModel::Building::getInstance().toAnalyticalModel();

    // retrieve node coords
    auto nodeCoords = api::getNodeCoordinatesOfAreaElement(2);

    // expected values
    std::vector<std::vector<utility::Vector3>> expectedCoords{ { {2.0, 0.0, 0.0}, {2.5, 0.0, 0.0}, {2.5, 0.5, 0.0}, {2.0, 0.5, 0.0} },
                                                               { {2.5, 0.0, 0.0}, {3.0, 0.0, 0.0}, {3.0, 0.5, 0.0}, {2.5, 0.5, 0.0} },
                                                               { {2.0, 0.5, 0.0}, {2.5, 0.5, 0.0}, {2.5, 1.0, 0.0}, {2.0, 1.0, 0.0} },
                                                               { {2.5, 0.5, 0.0}, {3.0, 0.5, 0.0}, {3.0, 1.0, 0.0}, {2.5, 1.0, 0.0} } };

    // ensure the vectors have the same length
    ASSERT_EQ(nodeCoords.size(), expectedCoords.size()) << "Vectors differ in size.";
    
    // floating-point comparison tolerance
    const double epsilon = 1e-6;
    
    for (size_t i = 0; i < nodeCoords.size(); ++i) {
    
        for (size_t j = 0; j < nodeCoords[i].size(); ++j) {
            EXPECT_NEAR(expectedCoords[i][j].x, nodeCoords[i][j].x, epsilon) << "X coordinate mismatch at index " << i << "," << j;
            EXPECT_NEAR(expectedCoords[i][j].y, nodeCoords[i][j].y, epsilon) << "Y coordinate mismatch at index " << i << "," << j;
            EXPECT_NEAR(expectedCoords[i][j].z, nodeCoords[i][j].z, epsilon) << "Z coordinate mismatch at index " << i << "," << j;
        }
    }
}

TEST_F(QuadMesherTests, MeshWithN1nN2FiveElementsSuccess) {
    typedef buildingModeler::BuildingModelerAPI api;

    // add quad joints
    api::addJoint(1, { 0, 0, 0 });
    api::addJoint(2, { 3, 0, 0 });
    api::addJoint(3, { 6, 0, 0 });
    api::addJoint(4, { 0, 2, 0 });
    api::addJoint(5, { 3, 2, 0 });
    api::addJoint(6, { 6, 2, 0 });
    api::addJoint(7, { 0, 4, 0 });
    api::addJoint(8, { 3, 4, 0 });
    api::addJoint(9, { 6, 4, 0 });
    api::addJoint(10, { 9, 2, 0 });
    api::addJoint(11, { 9, 4, 0 });

    // add material
    api::addElasticMaterial(1, 20, 20, 2);

    // add section 2D
    api::addElasticSection2D(1, 1, 2.0);

    // add area element
    api::addShearWall(1, { 1, 2, 5, 4 }, 1, physicalModel::AreaElementFormulation::LINEAR);
    api::addShearWall(2, { 2, 3, 6, 5 }, 1, physicalModel::AreaElementFormulation::LINEAR);
    api::addShearWall(3, { 4, 5, 8, 7 }, 1, physicalModel::AreaElementFormulation::LINEAR);
    api::addShearWall(4, { 5, 6, 9, 8 }, 1, physicalModel::AreaElementFormulation::LINEAR);
    api::addShearWall(5, { 6, 10, 11, 9 }, 1, physicalModel::AreaElementFormulation::LINEAR);

    //mesh the element
    api::meshAreaElement(1, 3, 2);
    api::meshAreaElement(2, 3, 2);
    api::meshAreaElement(3, 3, 2);
    api::meshAreaElement(4, 3, 2);
    api::meshAreaElement(5, 3, 2);

    // create analytical model
    physicalModel::Building::getInstance().toAnalyticalModel();

    // retrieve node coords
    auto nodeCoords1 = api::getNodeCoordinatesOfAreaElement(1);
    auto nodeCoords2 = api::getNodeCoordinatesOfAreaElement(2);
    auto nodeCoords3 = api::getNodeCoordinatesOfAreaElement(3);
    auto nodeCoords4 = api::getNodeCoordinatesOfAreaElement(4);
    auto nodeCoords5 = api::getNodeCoordinatesOfAreaElement(5);

    // expected values
    std::vector<std::vector<utility::Vector3>> expectedCoords1{ { {0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {1.0, 1.0, 0.0}, {0.0, 1.0, 0.0} },
                                                                { {1.0, 0.0, 0.0}, {2.0, 0.0, 0.0}, {2.0, 1.0, 0.0}, {1.0, 1.0, 0.0} },
                                                                { {2.0, 0.0, 0.0}, {3.0, 0.0, 0.0}, {3.0, 1.0, 0.0}, {2.0, 1.0, 0.0} },
                                                                { {0.0, 1.0, 0.0}, {1.0, 1.0, 0.0}, {1.0, 2.0, 0.0}, {0.0, 2.0, 0.0} }, 
                                                                { {1.0, 1.0, 0.0}, {2.0, 1.0, 0.0}, {2.0, 2.0, 0.0}, {1.0, 2.0, 0.0} },
                                                                { {2.0, 1.0, 0.0}, {3.0, 1.0, 0.0}, {3.0, 2.0, 0.0}, {2.0, 2.0, 0.0} } };

    std::vector<std::vector<utility::Vector3>> expectedCoords2{ { {3.0, 0.0, 0.0}, {4.0, 0.0, 0.0}, {4.0, 1.0, 0.0}, {3.0, 1.0, 0.0} },
                                                                { {4.0, 0.0, 0.0}, {5.0, 0.0, 0.0}, {5.0, 1.0, 0.0}, {4.0, 1.0, 0.0} },
                                                                { {5.0, 0.0, 0.0}, {6.0, 0.0, 0.0}, {6.0, 1.0, 0.0}, {5.0, 1.0, 0.0} },
                                                                { {3.0, 1.0, 0.0}, {4.0, 1.0, 0.0}, {4.0, 2.0, 0.0}, {3.0, 2.0, 0.0} },
                                                                { {4.0, 1.0, 0.0}, {5.0, 1.0, 0.0}, {5.0, 2.0, 0.0}, {4.0, 2.0, 0.0} },
                                                                { {5.0, 1.0, 0.0}, {6.0, 1.0, 0.0}, {6.0, 2.0, 0.0}, {5.0, 2.0, 0.0} } };

    std::vector<std::vector<utility::Vector3>> expectedCoords3{ { {0.0, 2.0, 0.0}, {1.0, 2.0, 0.0}, {1.0, 3.0, 0.0}, {0.0, 3.0, 0.0} },
                                                                { {1.0, 2.0, 0.0}, {2.0, 2.0, 0.0}, {2.0, 3.0, 0.0}, {1.0, 3.0, 0.0} },
                                                                { {2.0, 2.0, 0.0}, {3.0, 2.0, 0.0}, {3.0, 3.0, 0.0}, {2.0, 3.0, 0.0} },
                                                                { {0.0, 3.0, 0.0}, {1.0, 3.0, 0.0}, {1.0, 4.0, 0.0}, {0.0, 4.0, 0.0} },
                                                                { {1.0, 3.0, 0.0}, {2.0, 3.0, 0.0}, {2.0, 4.0, 0.0}, {1.0, 4.0, 0.0} },
                                                                { {2.0, 3.0, 0.0}, {3.0, 3.0, 0.0}, {3.0, 4.0, 0.0}, {2.0, 4.0, 0.0} } };

    std::vector<std::vector<utility::Vector3>> expectedCoords4{ { {3.0, 2.0, 0.0}, {4.0, 2.0, 0.0}, {4.0, 3.0, 0.0}, {3.0, 3.0, 0.0} },
                                                                { {4.0, 2.0, 0.0}, {5.0, 2.0, 0.0}, {5.0, 3.0, 0.0}, {4.0, 3.0, 0.0} },
                                                                { {5.0, 2.0, 0.0}, {6.0, 2.0, 0.0}, {6.0, 3.0, 0.0}, {5.0, 3.0, 0.0} },
                                                                { {3.0, 3.0, 0.0}, {4.0, 3.0, 0.0}, {4.0, 4.0, 0.0}, {3.0, 4.0, 0.0} },
                                                                { {4.0, 3.0, 0.0}, {5.0, 3.0, 0.0}, {5.0, 4.0, 0.0}, {4.0, 4.0, 0.0} },
                                                                { {5.0, 3.0, 0.0}, {6.0, 3.0, 0.0}, {6.0, 4.0, 0.0}, {5.0, 4.0, 0.0} } };

    std::vector<std::vector<utility::Vector3>> expectedCoords5{ { {6.0, 2.0, 0.0}, {7.0, 2.0, 0.0}, {7.0, 3.0, 0.0}, {6.0, 3.0, 0.0} },
                                                                { {7.0, 2.0, 0.0}, {8.0, 2.0, 0.0}, {8.0, 3.0, 0.0}, {7.0, 3.0, 0.0} },
                                                                { {8.0, 2.0, 0.0}, {9.0, 2.0, 0.0}, {9.0, 3.0, 0.0}, {8.0, 3.0, 0.0} },
                                                                { {6.0, 3.0, 0.0}, {7.0, 3.0, 0.0}, {7.0, 4.0, 0.0}, {6.0, 4.0, 0.0} },
                                                                { {7.0, 3.0, 0.0}, {8.0, 3.0, 0.0}, {8.0, 4.0, 0.0}, {7.0, 4.0, 0.0} },
                                                                { {8.0, 3.0, 0.0}, {9.0, 3.0, 0.0}, {9.0, 4.0, 0.0}, {8.0, 4.0, 0.0} } };

    // ensure the vectors have the same length
    ASSERT_EQ(nodeCoords1.size(), expectedCoords1.size()) << "Vectors differ in size.";
    ASSERT_EQ(nodeCoords2.size(), expectedCoords2.size()) << "Vectors differ in size.";
    ASSERT_EQ(nodeCoords3.size(), expectedCoords3.size()) << "Vectors differ in size.";
    ASSERT_EQ(nodeCoords4.size(), expectedCoords4.size()) << "Vectors differ in size.";
    ASSERT_EQ(nodeCoords5.size(), expectedCoords5.size()) << "Vectors differ in size.";

    // floating-point comparison tolerance
    const double epsilon = 1e-6;

    for (size_t i = 0; i < nodeCoords1.size(); ++i) {
        for (size_t j = 0; j < nodeCoords1[i].size(); ++j) {
            EXPECT_NEAR(expectedCoords1[i][j].x, nodeCoords1[i][j].x, epsilon) << "X coordinate mismatch at index " << i << "," << j;
            EXPECT_NEAR(expectedCoords1[i][j].y, nodeCoords1[i][j].y, epsilon) << "Y coordinate mismatch at index " << i << "," << j;
            EXPECT_NEAR(expectedCoords1[i][j].z, nodeCoords1[i][j].z, epsilon) << "Z coordinate mismatch at index " << i << "," << j;
        }
    }

    for (size_t i = 0; i < nodeCoords2.size(); ++i) {
        for (size_t j = 0; j < nodeCoords2[i].size(); ++j) {
            EXPECT_NEAR(expectedCoords2[i][j].x, nodeCoords2[i][j].x, epsilon) << "X coordinate mismatch at index " << i << "," << j;
            EXPECT_NEAR(expectedCoords2[i][j].y, nodeCoords2[i][j].y, epsilon) << "Y coordinate mismatch at index " << i << "," << j;
            EXPECT_NEAR(expectedCoords2[i][j].z, nodeCoords2[i][j].z, epsilon) << "Z coordinate mismatch at index " << i << "," << j;
        }
    }

    for (size_t i = 0; i < nodeCoords3.size(); ++i) {
        for (size_t j = 0; j < nodeCoords3[i].size(); ++j) {
            EXPECT_NEAR(expectedCoords3[i][j].x, nodeCoords3[i][j].x, epsilon) << "X coordinate mismatch at index " << i << "," << j;
            EXPECT_NEAR(expectedCoords3[i][j].y, nodeCoords3[i][j].y, epsilon) << "Y coordinate mismatch at index " << i << "," << j;
            EXPECT_NEAR(expectedCoords3[i][j].z, nodeCoords3[i][j].z, epsilon) << "Z coordinate mismatch at index " << i << "," << j;
        }
    }

    for (size_t i = 0; i < nodeCoords4.size(); ++i) {
        for (size_t j = 0; j < nodeCoords4[i].size(); ++j) {
            EXPECT_NEAR(expectedCoords4[i][j].x, nodeCoords4[i][j].x, epsilon) << "X coordinate mismatch at index " << i << "," << j;
            EXPECT_NEAR(expectedCoords4[i][j].y, nodeCoords4[i][j].y, epsilon) << "Y coordinate mismatch at index " << i << "," << j;
            EXPECT_NEAR(expectedCoords4[i][j].z, nodeCoords4[i][j].z, epsilon) << "Z coordinate mismatch at index " << i << "," << j;
        }
    }

    for (size_t i = 0; i < nodeCoords5.size(); ++i) {
        for (size_t j = 0; j < nodeCoords5[i].size(); ++j) {
            EXPECT_NEAR(expectedCoords5[i][j].x, nodeCoords5[i][j].x, epsilon) << "X coordinate mismatch at index " << i << "," << j;
            EXPECT_NEAR(expectedCoords5[i][j].y, nodeCoords5[i][j].y, epsilon) << "Y coordinate mismatch at index " << i << "," << j;
            EXPECT_NEAR(expectedCoords5[i][j].z, nodeCoords5[i][j].z, epsilon) << "Z coordinate mismatch at index " << i << "," << j;
        }
    }
}

TEST_F(QuadMesherTests, MeshWithN1nN2FiveElementsWithConstraintSuccess) {
    typedef buildingModeler::BuildingModelerAPI api;

    // add quad joints
    api::addJoint(1, { 0, 0, 0 });
    api::addJoint(2, { 3, 0, 0 });
    api::addJoint(3, { 6, 0, 0 });
    api::addJoint(4, { 0, 2, 0 });
    api::addJoint(5, { 3, 2, 0 });
    api::addJoint(6, { 6, 2, 0 });
    api::addJoint(7, { 0, 4, 0 });
    api::addJoint(8, { 3, 4, 0 });
    api::addJoint(9, { 6, 4, 0 });
    api::addJoint(10, { 9, 2, 0 });
    api::addJoint(11, { 9, 4, 0 });

    // add constraint
    api::setConstraintVector(1, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(2, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(3, { 1, 1, 1, 0, 0, 0 });
    api::setConstraintVector(4, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(7, { 1, 1, 1, 0, 0, 0 });

    // add material
    api::addElasticMaterial(1, 20, 20, 2);

    // add section 2D
    api::addElasticSection2D(1, 1, 2.0);

    // add area element
    api::addShearWall(1, { 1, 2, 5, 4 }, 1, physicalModel::AreaElementFormulation::LINEAR);
    api::addShearWall(2, { 2, 3, 6, 5 }, 1, physicalModel::AreaElementFormulation::LINEAR);
    api::addShearWall(3, { 4, 5, 8, 7 }, 1, physicalModel::AreaElementFormulation::LINEAR);
    api::addShearWall(4, { 5, 6, 9, 8 }, 1, physicalModel::AreaElementFormulation::LINEAR);
    api::addShearWall(5, { 6, 10, 11, 9 }, 1, physicalModel::AreaElementFormulation::LINEAR);

    //mesh the element
    api::meshAreaElement(1, 3, 2);
    api::meshAreaElement(2, 3, 2);
    api::meshAreaElement(3, 3, 2);
    api::meshAreaElement(4, 3, 2);
    api::meshAreaElement(5, 3, 2);

    // create analytical model
    physicalModel::Building::getInstance().toAnalyticalModel();

    // retrieve node coords
    auto nodeCoords1 = api::getNodeCoordinatesOfAreaElement(1);
    auto nodeCoords2 = api::getNodeCoordinatesOfAreaElement(2);
    auto nodeCoords3 = api::getNodeCoordinatesOfAreaElement(3);
    auto nodeCoords4 = api::getNodeCoordinatesOfAreaElement(4);
    auto nodeCoords5 = api::getNodeCoordinatesOfAreaElement(5);

    // expected values
    std::vector<std::vector<utility::Vector3>> expectedCoords1{ { {0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {1.0, 1.0, 0.0}, {0.0, 1.0, 0.0} },
                                                                { {1.0, 0.0, 0.0}, {2.0, 0.0, 0.0}, {2.0, 1.0, 0.0}, {1.0, 1.0, 0.0} },
                                                                { {2.0, 0.0, 0.0}, {3.0, 0.0, 0.0}, {3.0, 1.0, 0.0}, {2.0, 1.0, 0.0} },
                                                                { {0.0, 1.0, 0.0}, {1.0, 1.0, 0.0}, {1.0, 2.0, 0.0}, {0.0, 2.0, 0.0} },
                                                                { {1.0, 1.0, 0.0}, {2.0, 1.0, 0.0}, {2.0, 2.0, 0.0}, {1.0, 2.0, 0.0} },
                                                                { {2.0, 1.0, 0.0}, {3.0, 1.0, 0.0}, {3.0, 2.0, 0.0}, {2.0, 2.0, 0.0} } };

    std::vector<std::vector<utility::Vector3>> expectedCoords2{ { {3.0, 0.0, 0.0}, {4.0, 0.0, 0.0}, {4.0, 1.0, 0.0}, {3.0, 1.0, 0.0} },
                                                                { {4.0, 0.0, 0.0}, {5.0, 0.0, 0.0}, {5.0, 1.0, 0.0}, {4.0, 1.0, 0.0} },
                                                                { {5.0, 0.0, 0.0}, {6.0, 0.0, 0.0}, {6.0, 1.0, 0.0}, {5.0, 1.0, 0.0} },
                                                                { {3.0, 1.0, 0.0}, {4.0, 1.0, 0.0}, {4.0, 2.0, 0.0}, {3.0, 2.0, 0.0} },
                                                                { {4.0, 1.0, 0.0}, {5.0, 1.0, 0.0}, {5.0, 2.0, 0.0}, {4.0, 2.0, 0.0} },
                                                                { {5.0, 1.0, 0.0}, {6.0, 1.0, 0.0}, {6.0, 2.0, 0.0}, {5.0, 2.0, 0.0} } };

    std::vector<std::vector<utility::Vector3>> expectedCoords3{ { {0.0, 2.0, 0.0}, {1.0, 2.0, 0.0}, {1.0, 3.0, 0.0}, {0.0, 3.0, 0.0} },
                                                                { {1.0, 2.0, 0.0}, {2.0, 2.0, 0.0}, {2.0, 3.0, 0.0}, {1.0, 3.0, 0.0} },
                                                                { {2.0, 2.0, 0.0}, {3.0, 2.0, 0.0}, {3.0, 3.0, 0.0}, {2.0, 3.0, 0.0} },
                                                                { {0.0, 3.0, 0.0}, {1.0, 3.0, 0.0}, {1.0, 4.0, 0.0}, {0.0, 4.0, 0.0} },
                                                                { {1.0, 3.0, 0.0}, {2.0, 3.0, 0.0}, {2.0, 4.0, 0.0}, {1.0, 4.0, 0.0} },
                                                                { {2.0, 3.0, 0.0}, {3.0, 3.0, 0.0}, {3.0, 4.0, 0.0}, {2.0, 4.0, 0.0} } };

    std::vector<std::vector<utility::Vector3>> expectedCoords4{ { {3.0, 2.0, 0.0}, {4.0, 2.0, 0.0}, {4.0, 3.0, 0.0}, {3.0, 3.0, 0.0} },
                                                                { {4.0, 2.0, 0.0}, {5.0, 2.0, 0.0}, {5.0, 3.0, 0.0}, {4.0, 3.0, 0.0} },
                                                                { {5.0, 2.0, 0.0}, {6.0, 2.0, 0.0}, {6.0, 3.0, 0.0}, {5.0, 3.0, 0.0} },
                                                                { {3.0, 3.0, 0.0}, {4.0, 3.0, 0.0}, {4.0, 4.0, 0.0}, {3.0, 4.0, 0.0} },
                                                                { {4.0, 3.0, 0.0}, {5.0, 3.0, 0.0}, {5.0, 4.0, 0.0}, {4.0, 4.0, 0.0} },
                                                                { {5.0, 3.0, 0.0}, {6.0, 3.0, 0.0}, {6.0, 4.0, 0.0}, {5.0, 4.0, 0.0} } };

    std::vector<std::vector<utility::Vector3>> expectedCoords5{ { {6.0, 2.0, 0.0}, {7.0, 2.0, 0.0}, {7.0, 3.0, 0.0}, {6.0, 3.0, 0.0} },
                                                                { {7.0, 2.0, 0.0}, {8.0, 2.0, 0.0}, {8.0, 3.0, 0.0}, {7.0, 3.0, 0.0} },
                                                                { {8.0, 2.0, 0.0}, {9.0, 2.0, 0.0}, {9.0, 3.0, 0.0}, {8.0, 3.0, 0.0} },
                                                                { {6.0, 3.0, 0.0}, {7.0, 3.0, 0.0}, {7.0, 4.0, 0.0}, {6.0, 4.0, 0.0} },
                                                                { {7.0, 3.0, 0.0}, {8.0, 3.0, 0.0}, {8.0, 4.0, 0.0}, {7.0, 4.0, 0.0} },
                                                                { {8.0, 3.0, 0.0}, {9.0, 3.0, 0.0}, {9.0, 4.0, 0.0}, {8.0, 4.0, 0.0} } };

    // ensure the vectors have the same length
    ASSERT_EQ(nodeCoords1.size(), expectedCoords1.size()) << "Vectors differ in size.";
    ASSERT_EQ(nodeCoords2.size(), expectedCoords2.size()) << "Vectors differ in size.";
    ASSERT_EQ(nodeCoords3.size(), expectedCoords3.size()) << "Vectors differ in size.";
    ASSERT_EQ(nodeCoords4.size(), expectedCoords4.size()) << "Vectors differ in size.";
    ASSERT_EQ(nodeCoords5.size(), expectedCoords5.size()) << "Vectors differ in size.";

    // floating-point comparison tolerance
    const double epsilon = 1e-6;

    for (size_t i = 0; i < nodeCoords1.size(); ++i) {
        for (size_t j = 0; j < nodeCoords1[i].size(); ++j) {
            EXPECT_NEAR(expectedCoords1[i][j].x, nodeCoords1[i][j].x, epsilon) << "X coordinate mismatch at index " << i << "," << j;
            EXPECT_NEAR(expectedCoords1[i][j].y, nodeCoords1[i][j].y, epsilon) << "Y coordinate mismatch at index " << i << "," << j;
            EXPECT_NEAR(expectedCoords1[i][j].z, nodeCoords1[i][j].z, epsilon) << "Z coordinate mismatch at index " << i << "," << j;
        }
    }

    for (size_t i = 0; i < nodeCoords2.size(); ++i) {
        for (size_t j = 0; j < nodeCoords2[i].size(); ++j) {
            EXPECT_NEAR(expectedCoords2[i][j].x, nodeCoords2[i][j].x, epsilon) << "X coordinate mismatch at index " << i << "," << j;
            EXPECT_NEAR(expectedCoords2[i][j].y, nodeCoords2[i][j].y, epsilon) << "Y coordinate mismatch at index " << i << "," << j;
            EXPECT_NEAR(expectedCoords2[i][j].z, nodeCoords2[i][j].z, epsilon) << "Z coordinate mismatch at index " << i << "," << j;
        }
    }

    for (size_t i = 0; i < nodeCoords3.size(); ++i) {
        for (size_t j = 0; j < nodeCoords3[i].size(); ++j) {
            EXPECT_NEAR(expectedCoords3[i][j].x, nodeCoords3[i][j].x, epsilon) << "X coordinate mismatch at index " << i << "," << j;
            EXPECT_NEAR(expectedCoords3[i][j].y, nodeCoords3[i][j].y, epsilon) << "Y coordinate mismatch at index " << i << "," << j;
            EXPECT_NEAR(expectedCoords3[i][j].z, nodeCoords3[i][j].z, epsilon) << "Z coordinate mismatch at index " << i << "," << j;
        }
    }

    for (size_t i = 0; i < nodeCoords4.size(); ++i) {
        for (size_t j = 0; j < nodeCoords4[i].size(); ++j) {
            EXPECT_NEAR(expectedCoords4[i][j].x, nodeCoords4[i][j].x, epsilon) << "X coordinate mismatch at index " << i << "," << j;
            EXPECT_NEAR(expectedCoords4[i][j].y, nodeCoords4[i][j].y, epsilon) << "Y coordinate mismatch at index " << i << "," << j;
            EXPECT_NEAR(expectedCoords4[i][j].z, nodeCoords4[i][j].z, epsilon) << "Z coordinate mismatch at index " << i << "," << j;
        }
    }

    for (size_t i = 0; i < nodeCoords5.size(); ++i) {
        for (size_t j = 0; j < nodeCoords5[i].size(); ++j) {
            EXPECT_NEAR(expectedCoords5[i][j].x, nodeCoords5[i][j].x, epsilon) << "X coordinate mismatch at index " << i << "," << j;
            EXPECT_NEAR(expectedCoords5[i][j].y, nodeCoords5[i][j].y, epsilon) << "Y coordinate mismatch at index " << i << "," << j;
            EXPECT_NEAR(expectedCoords5[i][j].z, nodeCoords5[i][j].z, epsilon) << "Z coordinate mismatch at index " << i << "," << j;
        }
    }

    // retrieve constraint vectors
    auto constraints12 = api::getConstraintVectorForNodesBetween(1, 2);
    auto constraints23 = api::getConstraintVectorForNodesBetween(2, 3);
    auto constraints14 = api::getConstraintVectorForNodesBetween(1, 4);
    auto constraints47 = api::getConstraintVectorForNodesBetween(4, 7);
    auto constraints78 = api::getConstraintVectorForNodesBetween(7, 8);
    auto constraints13 = api::getConstraintVectorForNodesBetween(1, 3);
    auto constraints1011 = api::getConstraintVectorForNodesBetween(10, 11);

    // expected constraint vectors
    auto expectedFixed = std::vector<int>{ 1, 1, 1, 1, 1, 1 };
    auto expectedPinned = std::vector<int>{ 1, 1, 1, 0, 0, 0 };
    auto expectedFree = std::vector<int>{};

    // Joints 1 & 2
    for (auto vec : constraints12) {
        ASSERT_EQ(expectedFixed, vec);
    }

    // Joints 2 & 3
    ASSERT_EQ(expectedFixed, constraints23[0]);
    for (int i = 1; i < constraints23.size() - 1; ++i) {
        ASSERT_EQ(expectedFree, constraints23[i]);
    }
    ASSERT_EQ(expectedPinned, constraints23[constraints23.size() - 1]);

    // Joints 1 & 4
    for (auto vec : constraints14) {
        ASSERT_EQ(expectedFixed, vec);
    }

    // Joints 4 & 7
    ASSERT_EQ(expectedFixed, constraints47[0]);
    for (int i = 1; i < constraints47.size() - 1; ++i) {
        ASSERT_EQ(expectedFree, constraints47[i]);
    }
    ASSERT_EQ(expectedPinned, constraints47[constraints47.size() - 1]);

    // Joints 7 & 8
    ASSERT_EQ(expectedPinned, constraints78[0]);
    for (int i = 1; i < constraints78.size(); ++i) {
        ASSERT_EQ(expectedFree, constraints78[i]);
    }

    // Joints 1 & 3
    ASSERT_EQ(2, constraints13.size());
    ASSERT_EQ(expectedFixed, constraints13[0]);
    ASSERT_EQ(expectedPinned, constraints13[1]);

    // Joints 10 & 11
    for (int i = 0; i < constraints1011.size(); ++i) {
        ASSERT_EQ(expectedFree, constraints1011[i]);
    }
}

TEST_F(QuadMesherTests, MeshWithSurroundingElementsFiveElementsSuccess) {
    typedef buildingModeler::BuildingModelerAPI api;

    // add quad joints
    api::addJoint(1, { 0, 0, 0 });
    api::addJoint(2, { 3, 0, 0 });
    api::addJoint(3, { 6, 0, 0 });
    api::addJoint(4, { 0, 2, 0 });
    api::addJoint(5, { 3, 2, 0 });
    api::addJoint(6, { 6, 2, 0 });
    api::addJoint(7, { 0, 4, 0 });
    api::addJoint(8, { 3, 4, 0 });
    api::addJoint(9, { 6, 4, 0 });
    api::addJoint(10, { 9, 2, 0 });
    api::addJoint(11, { 9, 4, 0 });

    // add material
    api::addElasticMaterial(1, 20, 20, 2);

    // add section 2D
    api::addElasticSection2D(1, 1, 2.0);

    // add area element
    api::addShearWall(1, { 1, 2, 5, 4 }, 1, physicalModel::AreaElementFormulation::LINEAR);
    api::addShearWall(2, { 2, 3, 6, 5 }, 1, physicalModel::AreaElementFormulation::LINEAR);
    api::addShearWall(3, { 4, 5, 8, 7 }, 1, physicalModel::AreaElementFormulation::LINEAR);
    api::addShearWall(4, { 5, 6, 9, 8 }, 1, physicalModel::AreaElementFormulation::LINEAR);
    api::addShearWall(5, { 6, 10, 11, 9 }, 1, physicalModel::AreaElementFormulation::LINEAR);

    // add section 1D
    api::addElasticSection1D(2, 1, 0.2, 0.02, 0.02, 0.04);

    // add line element
    api::addBeam(1, { 1, 2 }, 2, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(2, { 2, 3 }, 2, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(3, { 4, 5 }, 2, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(4, { 5, 6 }, 2, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(5, { 6, 10 }, 2, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(6, { 7, 8 }, 2, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(7, { 8, 9 }, 2, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(8, { 9, 11 }, 2, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(9, { 1, 4 }, 2, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(10, { 4, 7 }, 2, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(11, { 2, 5 }, 2, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(12, { 5, 8 }, 2, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(13, { 3, 6 }, 2, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(14, { 6, 9 }, 2, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);
    api::addBeam(15, { 10, 11 }, 2, physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI);

    // set segment ratios
    api::setSegmentRatios(1, { 1.0 / 3.0, 1.0 / 3.0, 1.0 / 3.0 });
    api::setSegmentRatios(2, { 1.0 / 3.0, 1.0 / 3.0, 1.0 / 3.0 });
    api::setSegmentRatios(3, { 1.0 / 3.0, 1.0 / 3.0, 1.0 / 3.0 });
    api::setSegmentRatios(4, { 1.0 / 3.0, 1.0 / 3.0, 1.0 / 3.0 });
    api::setSegmentRatios(5, { 1.0 / 3.0, 1.0 / 3.0, 1.0 / 3.0 });
    api::setSegmentRatios(6, { 1.0 / 3.0, 1.0 / 3.0, 1.0 / 3.0 });
    api::setSegmentRatios(7, { 1.0 / 3.0, 1.0 / 3.0, 1.0 / 3.0 });
    api::setSegmentRatios(8, { 1.0 / 3.0, 1.0 / 3.0, 1.0 / 3.0 });
    api::setSegmentRatios(9, { 0.5, 0.5});
    api::setSegmentRatios(10, { 0.5, 0.5 });
    api::setSegmentRatios(11, { 0.5, 0.5 });
    api::setSegmentRatios(12, { 0.5, 0.5 });
    api::setSegmentRatios(13, { 0.5, 0.5 });
    api::setSegmentRatios(14, { 0.5, 0.5 });
    api::setSegmentRatios(15, { 0.5, 0.5 });

    // update connectivity between line and area elements
    api::updateAreaElementProperties();

    //mesh the element
    api::meshAreaElement(1);
    api::meshAreaElement(2);
    api::meshAreaElement(3);
    api::meshAreaElement(4);
    api::meshAreaElement(5);

    // create analytical model
    physicalModel::Building::getInstance().toAnalyticalModel();

    // retrieve node coords
    auto nodeCoords1 = api::getNodeCoordinatesOfAreaElement(1);
    auto nodeCoords2 = api::getNodeCoordinatesOfAreaElement(2);
    auto nodeCoords3 = api::getNodeCoordinatesOfAreaElement(3);
    auto nodeCoords4 = api::getNodeCoordinatesOfAreaElement(4);
    auto nodeCoords5 = api::getNodeCoordinatesOfAreaElement(5);

    // expected values
    std::vector<std::vector<utility::Vector3>> expectedCoords1{ { {0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {1.0, 1.0, 0.0}, {0.0, 1.0, 0.0} },
                                                                { {1.0, 0.0, 0.0}, {2.0, 0.0, 0.0}, {2.0, 1.0, 0.0}, {1.0, 1.0, 0.0} },
                                                                { {2.0, 0.0, 0.0}, {3.0, 0.0, 0.0}, {3.0, 1.0, 0.0}, {2.0, 1.0, 0.0} },
                                                                { {0.0, 1.0, 0.0}, {1.0, 1.0, 0.0}, {1.0, 2.0, 0.0}, {0.0, 2.0, 0.0} },
                                                                { {1.0, 1.0, 0.0}, {2.0, 1.0, 0.0}, {2.0, 2.0, 0.0}, {1.0, 2.0, 0.0} },
                                                                { {2.0, 1.0, 0.0}, {3.0, 1.0, 0.0}, {3.0, 2.0, 0.0}, {2.0, 2.0, 0.0} } };

    std::vector<std::vector<utility::Vector3>> expectedCoords2{ { {3.0, 0.0, 0.0}, {4.0, 0.0, 0.0}, {4.0, 1.0, 0.0}, {3.0, 1.0, 0.0} },
                                                                { {4.0, 0.0, 0.0}, {5.0, 0.0, 0.0}, {5.0, 1.0, 0.0}, {4.0, 1.0, 0.0} },
                                                                { {5.0, 0.0, 0.0}, {6.0, 0.0, 0.0}, {6.0, 1.0, 0.0}, {5.0, 1.0, 0.0} },
                                                                { {3.0, 1.0, 0.0}, {4.0, 1.0, 0.0}, {4.0, 2.0, 0.0}, {3.0, 2.0, 0.0} },
                                                                { {4.0, 1.0, 0.0}, {5.0, 1.0, 0.0}, {5.0, 2.0, 0.0}, {4.0, 2.0, 0.0} },
                                                                { {5.0, 1.0, 0.0}, {6.0, 1.0, 0.0}, {6.0, 2.0, 0.0}, {5.0, 2.0, 0.0} } };

    std::vector<std::vector<utility::Vector3>> expectedCoords3{ { {0.0, 2.0, 0.0}, {1.0, 2.0, 0.0}, {1.0, 3.0, 0.0}, {0.0, 3.0, 0.0} },
                                                                { {1.0, 2.0, 0.0}, {2.0, 2.0, 0.0}, {2.0, 3.0, 0.0}, {1.0, 3.0, 0.0} },
                                                                { {2.0, 2.0, 0.0}, {3.0, 2.0, 0.0}, {3.0, 3.0, 0.0}, {2.0, 3.0, 0.0} },
                                                                { {0.0, 3.0, 0.0}, {1.0, 3.0, 0.0}, {1.0, 4.0, 0.0}, {0.0, 4.0, 0.0} },
                                                                { {1.0, 3.0, 0.0}, {2.0, 3.0, 0.0}, {2.0, 4.0, 0.0}, {1.0, 4.0, 0.0} },
                                                                { {2.0, 3.0, 0.0}, {3.0, 3.0, 0.0}, {3.0, 4.0, 0.0}, {2.0, 4.0, 0.0} } };

    std::vector<std::vector<utility::Vector3>> expectedCoords4{ { {3.0, 2.0, 0.0}, {4.0, 2.0, 0.0}, {4.0, 3.0, 0.0}, {3.0, 3.0, 0.0} },
                                                                { {4.0, 2.0, 0.0}, {5.0, 2.0, 0.0}, {5.0, 3.0, 0.0}, {4.0, 3.0, 0.0} },
                                                                { {5.0, 2.0, 0.0}, {6.0, 2.0, 0.0}, {6.0, 3.0, 0.0}, {5.0, 3.0, 0.0} },
                                                                { {3.0, 3.0, 0.0}, {4.0, 3.0, 0.0}, {4.0, 4.0, 0.0}, {3.0, 4.0, 0.0} },
                                                                { {4.0, 3.0, 0.0}, {5.0, 3.0, 0.0}, {5.0, 4.0, 0.0}, {4.0, 4.0, 0.0} },
                                                                { {5.0, 3.0, 0.0}, {6.0, 3.0, 0.0}, {6.0, 4.0, 0.0}, {5.0, 4.0, 0.0} } };

    std::vector<std::vector<utility::Vector3>> expectedCoords5{ { {6.0, 2.0, 0.0}, {7.0, 2.0, 0.0}, {7.0, 3.0, 0.0}, {6.0, 3.0, 0.0} },
                                                                { {7.0, 2.0, 0.0}, {8.0, 2.0, 0.0}, {8.0, 3.0, 0.0}, {7.0, 3.0, 0.0} },
                                                                { {8.0, 2.0, 0.0}, {9.0, 2.0, 0.0}, {9.0, 3.0, 0.0}, {8.0, 3.0, 0.0} },
                                                                { {6.0, 3.0, 0.0}, {7.0, 3.0, 0.0}, {7.0, 4.0, 0.0}, {6.0, 4.0, 0.0} },
                                                                { {7.0, 3.0, 0.0}, {8.0, 3.0, 0.0}, {8.0, 4.0, 0.0}, {7.0, 4.0, 0.0} },
                                                                { {8.0, 3.0, 0.0}, {9.0, 3.0, 0.0}, {9.0, 4.0, 0.0}, {8.0, 4.0, 0.0} } };

    // ensure the vectors have the same length
    ASSERT_EQ(nodeCoords1.size(), expectedCoords1.size()) << "Vectors differ in size.";
    ASSERT_EQ(nodeCoords2.size(), expectedCoords2.size()) << "Vectors differ in size.";
    ASSERT_EQ(nodeCoords3.size(), expectedCoords3.size()) << "Vectors differ in size.";
    ASSERT_EQ(nodeCoords4.size(), expectedCoords4.size()) << "Vectors differ in size.";
    ASSERT_EQ(nodeCoords5.size(), expectedCoords5.size()) << "Vectors differ in size.";

    // floating-point comparison tolerance
    const double epsilon = 1e-6;

    for (size_t i = 0; i < nodeCoords1.size(); ++i) {
        for (size_t j = 0; j < nodeCoords1[i].size(); ++j) {
            EXPECT_NEAR(expectedCoords1[i][j].x, nodeCoords1[i][j].x, epsilon) << "X coordinate mismatch at index " << i << "," << j;
            EXPECT_NEAR(expectedCoords1[i][j].y, nodeCoords1[i][j].y, epsilon) << "Y coordinate mismatch at index " << i << "," << j;
            EXPECT_NEAR(expectedCoords1[i][j].z, nodeCoords1[i][j].z, epsilon) << "Z coordinate mismatch at index " << i << "," << j;
        }
    }

    for (size_t i = 0; i < nodeCoords2.size(); ++i) {
        for (size_t j = 0; j < nodeCoords2[i].size(); ++j) {
            EXPECT_NEAR(expectedCoords2[i][j].x, nodeCoords2[i][j].x, epsilon) << "X coordinate mismatch at index " << i << "," << j;
            EXPECT_NEAR(expectedCoords2[i][j].y, nodeCoords2[i][j].y, epsilon) << "Y coordinate mismatch at index " << i << "," << j;
            EXPECT_NEAR(expectedCoords2[i][j].z, nodeCoords2[i][j].z, epsilon) << "Z coordinate mismatch at index " << i << "," << j;
        }
    }

    for (size_t i = 0; i < nodeCoords3.size(); ++i) {
        for (size_t j = 0; j < nodeCoords3[i].size(); ++j) {
            EXPECT_NEAR(expectedCoords3[i][j].x, nodeCoords3[i][j].x, epsilon) << "X coordinate mismatch at index " << i << "," << j;
            EXPECT_NEAR(expectedCoords3[i][j].y, nodeCoords3[i][j].y, epsilon) << "Y coordinate mismatch at index " << i << "," << j;
            EXPECT_NEAR(expectedCoords3[i][j].z, nodeCoords3[i][j].z, epsilon) << "Z coordinate mismatch at index " << i << "," << j;
        }
    }

    for (size_t i = 0; i < nodeCoords4.size(); ++i) {
        for (size_t j = 0; j < nodeCoords4[i].size(); ++j) {
            EXPECT_NEAR(expectedCoords4[i][j].x, nodeCoords4[i][j].x, epsilon) << "X coordinate mismatch at index " << i << "," << j;
            EXPECT_NEAR(expectedCoords4[i][j].y, nodeCoords4[i][j].y, epsilon) << "Y coordinate mismatch at index " << i << "," << j;
            EXPECT_NEAR(expectedCoords4[i][j].z, nodeCoords4[i][j].z, epsilon) << "Z coordinate mismatch at index " << i << "," << j;
        }
    }

    for (size_t i = 0; i < nodeCoords5.size(); ++i) {
        for (size_t j = 0; j < nodeCoords5[i].size(); ++j) {
            EXPECT_NEAR(expectedCoords5[i][j].x, nodeCoords5[i][j].x, epsilon) << "X coordinate mismatch at index " << i << "," << j;
            EXPECT_NEAR(expectedCoords5[i][j].y, nodeCoords5[i][j].y, epsilon) << "Y coordinate mismatch at index " << i << "," << j;
            EXPECT_NEAR(expectedCoords5[i][j].z, nodeCoords5[i][j].z, epsilon) << "Z coordinate mismatch at index " << i << "," << j;
        }
    }
}

TEST_F(QuadMesherTests, MeshWithN1nN2TwoElementsFail) {
    typedef buildingModeler::BuildingModelerAPI api;

    // add quad joints
    api::addJoint(1, { 1, 0, 0 });
    api::addJoint(2, { 2, 0, 0 });
    api::addJoint(3, { 2, 1, 0 });
    api::addJoint(4, { 1, 1, 0 });
    api::addJoint(5, { 3, 0, 0 });
    api::addJoint(6, { 3, 1, 0 });

    // add material
    api::addElasticMaterial(1, 20, 20, 2);

    // add section 2D
    api::addElasticSection2D(1, 1, 2.0);

    // add area element
    api::addShearWall(1, { 1, 2, 3, 4 }, 1, physicalModel::AreaElementFormulation::LINEAR);
    api::addShearWall(2, { 2, 5, 6, 3 }, 1, physicalModel::AreaElementFormulation::LINEAR);

    // update connectivity between line and area elements
    api::updateAreaElementProperties();

    //mesh the element
    api::meshAreaElement(1, 2, 2);
    api::meshAreaElement(2, 2, 3);

    try {
        physicalModel::Building::getInstance().toAnalyticalModel();
        FAIL() << "Expected buildingModeler::InvalidInputException";
    }
    catch (const buildingModeler::InvalidInputException& e) {
        std::string expected = "Area elements sharing an edge must have the same mesh size along that edge!.";
        std::string actual = e.what();
        EXPECT_EQ(expected, actual);
    }
    catch (...) {
        FAIL() << "Expected buildingModeler::InvalidInputException";
    }
}