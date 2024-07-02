#pragma once

#include "Output.h"

namespace opensees
{
	class ModalOutput : public Output
	{
	private:
		std::vector<double> m_periods;
		std::vector<int> m_masterNodeTags;

		void retrievePeriods();
		void retrieveModeShapes();

	public:
		ModalOutput(std::string loadTag, std::vector<int> masterNodeTags);
		ModalOutput() = delete;
		~ModalOutput() {}

		const std::vector<double>& getPeriods() const;

		void retrieveOutput() override;
	};
}