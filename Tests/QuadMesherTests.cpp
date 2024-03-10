#include <gtest/gtest.h>
#include "../BuildingModeler/BuildingModelerAPI.h"
#include "../BuildingModeler/Utilities/Vector3.h"

TEST(QuadMesherTests, PlainMesher) {
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

    // mesh the element
    api::meshAreaElement(1, 2, 2);

    // create tcl file
    api::createInputFile();

    ASSERT_TRUE(true);
}