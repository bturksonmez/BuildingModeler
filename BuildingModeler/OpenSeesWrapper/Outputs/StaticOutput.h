#pragma once

#include "Output.h"

namespace opensees
{
	class StaticOutput : public Output
	{
	private:
		std::unordered_map<int, std::vector<std::vector<double>>> m_eleForceOutput;

		void retrieveNodeDisplacements();
		void retrieveElementForces();

	public:
		StaticOutput(std::string loadTag);
		StaticOutput() = delete;
		~StaticOutput() {}

		const std::unordered_map<int, std::vector<std::vector<double>>>& getElementForce() const;

		void retrieveOutput() override;
	};
}