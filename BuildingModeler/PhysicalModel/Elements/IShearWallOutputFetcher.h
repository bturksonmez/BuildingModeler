#pragma once

#include <string>

namespace physicalModel
{
	class IShearWallOutputFetcher
	{
	public:
		virtual double calculateAxialForceGlobalZ(std::string analysisTag, size_t timeStep, size_t edgeNo) = 0;
		virtual double calculateShearForceGlobalX(std::string analysisTag, size_t timeStep, size_t edgeNo) = 0;
		virtual double calculateShearForceGlobalY(std::string analysisTag, size_t timeStep, size_t edgeNo) = 0;
		virtual double calculateMomentGlobalXX(std::string analysisTag, size_t timeStep, size_t edgeNo) = 0;
		virtual double calculateMomentGlobalYY(std::string analysisTag, size_t timeStep, size_t edgeNo) = 0;
		virtual double calculateDR(std::string analysisTag, size_t timeStep, size_t edgeNo) = 0;
		virtual double calculateChordRotationGlobalX(std::string analysisTag, size_t timeStep, size_t edgeNo) = 0;
		virtual double calculateChordRotationGlobalZ(std::string analysisTag, size_t timeStep, size_t edgeNo) = 0;
	};
}
