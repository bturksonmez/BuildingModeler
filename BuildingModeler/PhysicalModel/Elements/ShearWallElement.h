#pragma once

#include "AreaElement.h"
#include "IShearWallOutputFetcher.h"

namespace physicalModel
{
	class ShearWallElement : public AreaElement, public IShearWallOutputFetcher
	{
	private:
		const std::vector<int> getBottomAnalyticalElements() const;
		const std::vector<int> getTopAnalyticalElements() const;
		bool onXdirection() const;
		double calculateMomentInPlaneXX(std::string analysisTag, size_t timeStep, bool atBottom);
		double calculateMomentInPlaneYY(std::string analysisTag, size_t timeStep, bool atBottom);
		double calculateRotation(std::string analysisTag, size_t dofTrans, size_t timeStep, bool fromBottom);

	public:
		ShearWallElement(int elementTag, std::vector<int> jointTags, std::shared_ptr<Section> section, AreaElementFormulation areaElementFormulation);
		ShearWallElement() = delete;
		~ShearWallElement() {}

		double calculateAxialForceGlobalZ(std::string analysisTag, size_t timeStep, bool atBottom) override;
		double calculateShearForceGlobalX(std::string analysisTag, size_t timeStep, bool atBottom) override;
		double calculateShearForceGlobalY(std::string analysisTag, size_t timeStep, bool atBottom) override;
		double calculateMomentGlobalXX(std::string analysisTag, size_t timeStep, bool atBottom) override;
		double calculateMomentGlobalYY(std::string analysisTag, size_t timeStep, bool atBottom) override;
		double calculateMomentGlobalZZ(std::string analysisTag, size_t timeStep, bool atBottom) override;
		double calculateDR(std::string analysisTag, size_t dof, size_t timeStep) override;
		double calculateChordRotation(std::string analysisTag, size_t dofRot, size_t timeStep, bool fromBottom) override;
	};
}
