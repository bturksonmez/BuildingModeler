#pragma once

#include <string>

namespace physicalModel
{
	class IShearWallOutputFetcher
	{
	public:
		virtual double calculateAxialForceGlobalZ(std::string analysisTag, size_t timeStep, bool atBottom) = 0;
		virtual double calculateShearForceGlobalX(std::string analysisTag, size_t timeStep, bool atBottom) = 0;
		virtual double calculateShearForceGlobalY(std::string analysisTag, size_t timeStep, bool atBottom) = 0;
		virtual double calculateMomentGlobalXX(std::string analysisTag, size_t timeStep, bool atBottom) = 0;
		virtual double calculateMomentGlobalYY(std::string analysisTag, size_t timeStep, bool atBottom) = 0;
		virtual double calculateDR(std::string analysisTag, size_t dof, size_t timeStep, bool fromBottom) = 0;
		virtual double calculateChordRotation(std::string analysisTag, size_t dof, size_t timeStep, bool fromBottom) = 0;
	};
}
