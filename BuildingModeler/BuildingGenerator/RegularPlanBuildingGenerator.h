#pragma once

#include <random>
#include <vector>
#include <optional>

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
		static long long counter;

		void validateInput();
		void generateBuildingPlan(json& buildingInfo);
		void generateFloors(int numberOfStories, double firstStoreyHeight, double storeyHeight);
		void generateJoints(json& buildingInfo, const std::vector<std::vector<std::vector<double>>>& shearWallArrangement, std::unordered_map<int, std::pair<int, int>>& shearWallJointMap);
		void generateMaterials(double E, double shearWallCrackedSectionModifier);
		void generateShearWalls(const std::vector<std::vector<std::vector<double>>>& shearWallArrangement, std::unordered_map<int, std::pair<int, int>>& shearWallJointMap, double thickness, json& buildingInfo);
		void generateSlabMasses(double thickness, std::unordered_map<int, std::pair<int, int>>& shearWallJointMap, json& buildingInfo);
		void generateColumns(const std::vector<std::vector<int>>& modifiedShearWallArrangement, double outerColumnWidth, double outerColumnDepth, double innerColumnWidth, double innerColumnDepth, double columnCrackedSectionModifier, json& buildingInfo);
		void generateBeams(double width, double minDepth, double maxDepth, double minLength, double maxLength, double beamCrackedSectionModifier, std::unordered_map<int, std::pair<int, int>>& shearWallJointMap, json& buildingInfo);
		void meshAreaElements();
		void applyModelingPreferences(json& buildingInfo);
		std::unordered_map<std::string, bool> analyze();
		void fetchResultsForGravityAnalysis(json& buildingInfo);
		void fetchResultsForEarthquakeAnalysis(json& buildingInfo);
		void fetchResultsForModalAnalysis(json& buildingInfo);
		void fetchAxialLoadDistribution(std::string analysisName, json& buildingInfo);
		void fetchBaseShearInXDirDistribution(std::string analysisName, json& buildingInfo);
		void fetchMomentInXDirDistribution(std::string analysisName, json& buildingInfo);
		void fetchBeamDisplacementDistribution(std::string analysisName, int floorNumber, json& buildingInfo);
		void fetchDriftRatioDistribution(std::string analysisName, json& buildingInfo);
		void fetchFundamentalPeriodInGivenDirection(std::string analysisName, json& buildingInfo);

		std::vector<std::vector<std::vector<double>>> getShearWallArrangement(int numOfBaysLongDir, int numOfBaysPerpDir, std::vector<double> bayWidthsLongDir, std::vector<double> bayWidthsPerpDir, double thickness, double& shearWallRatioX, double& shearWallRatioY);
		std::vector<std::vector<double>> getPerimeterShearWallLocations(int numOfBays, std::vector<double> bayWidths, double thickness, double buildingArea);
		std::vector<std::vector<int>> findSubsetsOfVector(const std::vector<int>& vec);

	public:
		RegularPlanBuildingGenerator(const Parameters& parameters, std::unique_ptr<loadingGenerator::ILoadingGenerator> loading);
		RegularPlanBuildingGenerator() = delete;
		~RegularPlanBuildingGenerator() = default;

		json generateAndAnalyze(std::optional<long long> seed) override;
		bool createModelFromJsonAndAnalyze(json& buildingInfo) override;
	};
}