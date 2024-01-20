#pragma once

#include "PhysicalModel/Building.h"
#include "OpenSeesWrapper/OpenseesModel.h"

namespace buildingModeler
{
    class OpenseesConverter
    {
    private:
        static bool nodeExists(int nodeTag);
        static bool materialExists(int materialTag);
        static bool sectionExists(int sectionTag);

    public:
        static void toNodeMassConstraint(const physicalModel::Joint* joint);
        static void toMaterial(const physicalModel::Material* material);
        static void toSection(const physicalModel::Section* section);
    };
}
