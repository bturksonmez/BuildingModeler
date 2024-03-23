#include <iostream>
#include "BuildingModelerAPI.h"
#include "Utilities/VectorUtilities.h"

using namespace std;

int main()
{
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

    api::setConstraintVector(1, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(2, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(7, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(8, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(9, { 1, 1, 1, 1, 1, 1 });
    api::setConstraintVector(11, { 1, 1, 1, 1, 1, 1 });

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

    api::includeMassFromMembers(false);

    // create analytical model
    api::createInputFile();

	return 0;
}

