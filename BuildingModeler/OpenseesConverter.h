#pragma once

#include "PhysicalModel/Building.h"
#include "OpenSeesWrapper/OpenseesModel.h"
#include "OpenSeesWrapper/Utilities/TagGenerator.h"

namespace buildingModeler
{
    class OpenseesConverter
    {
    private:
        static bool nodeExists(int nodeTag);
        static bool materialExists(int materialTag);
        static bool sectionExists(int sectionTag);
        static bool beamColumnElementExists(int elementTag);
        static std::vector<int> getNodesForLineElement(const physicalModel::LineElement* element);

    public:
        static void toNodeMassConstraint(const physicalModel::Joint* joint);
        static void toMaterial(const physicalModel::Material* material);
        static void toSection(const physicalModel::Section* section);
        static void includePDeltaEffects(bool includePDeltaEffects);
        static void toBeamColumnElement(physicalModel::LineElement* element);
        static void toQuadrilateralElement(physicalModel::AreaElement* element);
    };
}
