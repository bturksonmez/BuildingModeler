#pragma once

#include <memory>

#include "LoadCase.h"

namespace physicalModel
{
	class ModalLoadCase : public LoadCase
	{
	private:
		size_t m_numberOfModes;

	public:
		ModalLoadCase(std::string loadCaseTag, size_t m_numberOfModes);
		ModalLoadCase() = delete;
		~ModalLoadCase() {}

		size_t getNumberOfModes() const;
	};
}


