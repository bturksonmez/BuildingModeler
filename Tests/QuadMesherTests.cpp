#include <gtest/gtest.h>
#include "../BuildingModeler/BuildingModelerAPI.h"
#include "../BuildingModeler/Utilities/Vector3.h"

TEST(QuadMesherTests, ElementNotFoundFail) {
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

TEST(QuadMesherTests, NoSurroundingElementsWithNullN1Fail) {
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

TEST(QuadMesherTests, UnbalancedSurroundingElementsWithNullN1Fail) {
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

TEST(QuadMesherTests, NoSurroundingElementsWithMinusN1Fail) {
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

TEST(QuadMesherTests, SurroundingElementsWithN1Fail) {
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

TEST(QuadMesherTests, UnbalancedSurroundingElementSegmentsFail) {
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

TEST(QuadMesherTests, MeshWithN1nN2Success) {
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

TEST(QuadMesherTests, NoMeshSuccess) {
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

TEST(QuadMesherTests, MeshWith4SurroundingElementsSuccess) {
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

TEST(QuadMesherTests, MeshWith2SurroundingElementsN1Success) {
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

TEST(QuadMesherTests, MeshWith2SurroundingElementsN2Success) {
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

TEST(QuadMesherTests, MeshWithN1nN2TwoElementsSuccess) {
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

TEST(QuadMesherTests, MeshWithN1nN2TwoElementsFail) {
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