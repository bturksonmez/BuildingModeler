#pragma once

#include <string>

namespace physicalModel
{
	class ILineElementOutputFetcher
	{
	public:
		virtual double calculateAxialForce(std::string analysisTag, size_t timeStep, bool atIJoint = true) = 0;
		virtual double calculateShearForceY(std::string analysisTag, size_t timeStep, bool atIJoint = true) = 0;
		virtual double calculateShearForceZ(std::string analysisTag, size_t timeStep, bool atIJoint = true) = 0;
		virtual double calculateMomentYY(std::string analysisTag, size_t timeStep, bool atIJoint = true) = 0;
		virtual double calculateMomentZZ(std::string analysisTag, size_t timeStep, bool atIJoint = true) = 0;
		virtual double calculateTorsion(std::string analysisTag, size_t timeStep, bool atIJoint = true) = 0;
		virtual double calculateDR(std::string analysisTag, size_t timeStep, bool fromIJoint = true) = 0;
		virtual double calculateChordRotationYY(std::string analysisTag, size_t timeStep, bool atIJoint = true) = 0;
		virtual double calculateChordRotationZZ(std::string analysisTag, size_t timeStep, bool atIJoint = true) = 0;
		virtual double calculateDisplacement(std::string analysisTag, size_t timeStep, size_t segmentNode, size_t dof) = 0;
	};
}
