#pragma once

#include <vector>

#include "ILoadingGenerator.h"

namespace loadingGenerator
{
	class ModalAnalysisGenerator : public ILoadingGenerator
	{
	public:
		ModalAnalysisGenerator();
		~ModalAnalysisGenerator() = default;

		void load(json& buildingInfo) override;
	};
}