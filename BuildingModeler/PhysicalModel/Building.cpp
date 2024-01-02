#include "Building.h"

using namespace physicalModel;

Building& Building::getInstance()
{
    static Building instance;
    return instance;
}