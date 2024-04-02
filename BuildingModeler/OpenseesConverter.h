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
        static bool quadrilateralElementExists(int elementTag);
        static bool loadPatternExists(std::string loadingName);
        static std::vector<int> createNodesForLineElement(const physicalModel::LineElement* element);
        static std::vector<int> createNodesBetweenTwoJoints(int jointTagA, int jointTagB, int numberOfIntervals);
        static std::vector<std::vector<int>> createNodesForMesh(const std::vector<int>& nodesIJ, const std::vector<int>& nodesLK, const std::vector<int>& nodesIL, const std::vector<int>& nodesJK);
        static void createMeshForQuadElement(physicalModel::AreaElement* element, const std::vector<std::vector<int>>& nodes);
        static std::tuple<utility::Vector3, utility::Vector3, utility::Vector3> getLocalCoordinateSystem(std::vector<int> lineIJ, std::vector<int> lineIL);

    public:
        static void toNodeMassConstraint(const physicalModel::Joint* joint);
        static void toMaterial(const physicalModel::Material* material);
        static void toSection(const physicalModel::Section* section);
        static void includePDeltaEffects(bool includePDeltaEffects);
        static void toBeamColumnElement(physicalModel::LineElement* element);
        static void toQuadrilateralElement(physicalModel::AreaElement* element);
        static void toRigidDiaphragm(physicalModel::Floor* floor);
        static void toLoadPatternFromLoadCase(physicalModel::LoadCase* loadCase);
        static void toLoadPatternFromLoadCombination(physicalModel::LoadCombination* loadCombination);
    };
}
