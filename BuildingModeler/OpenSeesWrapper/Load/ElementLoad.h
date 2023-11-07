#pragma once

#include "Load.h"

namespace opensees
{
	class Element;

	class ElementLoad : Load
	{
	private:
		std::weak_ptr<Element> m_element;

		ElementLoad(std::shared_ptr<Element> element, std::vector<double> loadVector);

	public:
		ElementLoad() = delete;
		~ElementLoad() {}

		const std::shared_ptr<Element> getElement() const;
		std::string getOpenseesCommand() const override;
	};
}