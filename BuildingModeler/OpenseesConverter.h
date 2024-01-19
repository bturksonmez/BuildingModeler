#pragma once

#include "PhysicalModel/Building.h"
#include "OpenSeesWrapper/OpenseesModel.h"

namespace buildingModeler
{
    class OpenseesConverter
    {
    private:
        static bool nodeExists(int nodeTag);

    public:
        static void toNodeAndMass(const physicalModel::Joint* joint);
    };
}
