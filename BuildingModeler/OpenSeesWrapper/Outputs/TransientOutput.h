#pragma once

#include "Output.h"

namespace opensees
{
	class TransientOutput : public Output
	{
	private:
		std::unordered_map<int, std::vector<std::vector<double>>> m_eleForceOutput;

	public:
		TransientOutput(std::string loadTag);
		TransientOutput() = delete;
		~TransientOutput() {}

		const std::unordered_map<int, std::vector<std::vector<double>>>& getElementForce() const;

		void readOutput() override;
	};
}