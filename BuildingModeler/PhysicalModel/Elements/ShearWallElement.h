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

	public:
		ShearWallElement(int elementTag, std::vector<int> jointTags, std::shared_ptr<Section> section, AreaElementFormulation areaElementFormulation);
		ShearWallElement() = delete;
		~ShearWallElement() {}

		double calculateAxialForceGlobalZ(std::string analysisTag, size_t timeStep, size_t edgeNo) override;
		double calculateShearForceGlobalX(std::string analysisTag, size_t timeStep, size_t edgeNo) override;
		double calculateShearForceGlobalY(std::string analysisTag, size_t timeStep, size_t edgeNo) override;
		double calculateMomentGlobalXX(std::string analysisTag, size_t timeStep, size_t edgeNo) override;
		double calculateMomentGlobalYY(std::string analysisTag, size_t timeStep, size_t edgeNo) override;
		double calculateDR(std::string analysisTag, size_t timeStep, size_t edgeNo) override;
		double calculateChordRotationGlobalX(std::string analysisTag, size_t timeStep, size_t edgeNo) override;
		double calculateChordRotationGlobalZ(std::string analysisTag, size_t timeStep, size_t edgeNo) override;
	};
}
