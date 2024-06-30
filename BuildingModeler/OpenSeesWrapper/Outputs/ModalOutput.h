#pragma once

#include "Output.h"

namespace opensees
{
	class ModalOutput : public Output
	{
	private:
		std::unordered_map<int, std::vector<double>> m_periods;
		std::vector<int> m_masterNodeTags;

	public:
		ModalOutput(std::string loadTag, std::vector<int> masterNodeTags);
		ModalOutput() = delete;
		~ModalOutput() {}

		const std::unordered_map<int, std::vector<double>>& getPeriods() const;

		void readOutput() override;
	};
}