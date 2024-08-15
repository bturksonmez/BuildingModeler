#pragma once

#include <string>

namespace physicalModel
{
	class ILineElementOutputFetcher
	{
	public:
		virtual double calculateForceX(std::string analysisTag, bool atIJoint, size_t timeStep) = 0;
		virtual double calculateForceY(std::string analysisTag, bool atIJoint, size_t timeStep) = 0;
		virtual double calculateForceZ(std::string analysisTag, bool atIJoint, size_t timeStep) = 0;
		virtual double calculateMomentXX(std::string analysisTag, bool atIJoint, size_t timeStep) = 0;
		virtual double calculateMomentYY(std::string analysisTag, bool atIJoint, size_t timeStep) = 0;
		virtual double calculateMomentZZ(std::string analysisTag, bool atIJoint, size_t timeStep) = 0;
		virtual double calculateDR(std::string analysisTag, bool fromIJoint, size_t timeStep) = 0;
		virtual double calculateChordRotation(std::string analysisTag, bool fromIJoint, size_t timeStep) = 0;
		virtual double calculateDisplacement(std::string analysisTag, size_t segmentNode, size_t dof, size_t timeStep) = 0;
	};
}
