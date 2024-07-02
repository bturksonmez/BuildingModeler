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

TEST_F(StaticAnalysisTests, OneStoryFrameStructureWithNx1Ny1) {
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

    // create analytical model and tcl file
    api::createInputFile();
}