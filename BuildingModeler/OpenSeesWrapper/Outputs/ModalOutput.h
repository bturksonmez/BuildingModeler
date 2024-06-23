#pragma once

#include "Output.h"

namespace opensees
{
	class ModalOutput : public Output
	{
	private:
		std::unordered_map<int, std::vector<double>> m_periods;

	public:
		ModalOutput(std::string loadTag);
		ModalOutput() = delete;
		~ModalOutput() {}

		const std::unordered_map<int, std::vector<double>>& getPeriods() const;
	};
}