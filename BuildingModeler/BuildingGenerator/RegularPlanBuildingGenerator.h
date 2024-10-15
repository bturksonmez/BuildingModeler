#pragma once

#include <random>
#include <vector>

#include "IBuildingGenerator.h"

// To do: this whole class is implemented for symmetry for both axis for plan, but symmetry only for x axis for structural members

namespace buildingGenerator
{
	class RegularPlanBuildingGenerator : public IBuildingGenerator
	{
	private:
		Parameters m_parameters;
		std::unique_ptr<loadingGenerator::ILoadingGenerator> m_loading;
		long long m_seed;
		std::default_random_engine m_generator;

		void validateInput();
		void generateBuildingPlan(json& buildingInfo);
		void generateFloors(int numberOfStories, double firstStoreyHeight, double storeyHeight);
		void generateJoints(json& buildingInfo);
		void generateMaterials(double E, double shearWallCrackedSectionModifier);
		void generateShearWalls(json& buildingInfo);
		void generateSlabs(json& buildingInfo);
		void generateColumns(json& buildingInfo);
		void generateBeams(json& buildingInfo);
		void meshAreaElements();
		void applyModelingPreferences(json& buildingInfo);
		void analyze(json& buildingInfo);
		void analyzeGravity(json& buildingInfo);
		void fetchResultsForGravityAnalysis(json& buildingInfo);
		void fetchAxialLoadDistribution(std::string analysisName, json& buildingInfo);
		void fetchBeamDisplacementDistribution(std::string analysisName, int floorNumber, json& buildingInfo);

		std::vector<std::vector<int>> getShearWallArrangementInLongitudinalDir(int numOfBaysLongDir, int numOfBaysPerpDir, std::vector<double> bayWidthsLongDir, std::vector<double> bayWidthsPerpDir, double thickness, double& shearWallRatio);
		std::vector<std::vector<int>> findSubsetsOfVector(const std::vector<int>& vec);

	public:
		RegularPlanBuildingGenerator(const Parameters& parameters, std::unique_ptr<loadingGenerator::ILoadingGenerator> loading);
		RegularPlanBuildingGenerator() = delete;
		~RegularPlanBuildingGenerator() = default;

		json generateAndAnalyze() override;
		void createModelFromJsonAndAnalyze(json& buildingInfo) override;
	};
}