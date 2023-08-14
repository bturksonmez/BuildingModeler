#pragma once

#include <iostream>
#include <vector>

namespace opensees
{
	enum ElementType
	{
		Frame,
		Area
	};

	class Element
	{
	protected:
		int m_elementTag;
		ElementType m_elementType;

		Element(int elementTag) : m_elementTag(elementTag) {}

	public:
		virtual ~Element() {}

		int getElementTag() const;
		ElementType getElementType() const;

		virtual std::string getOpenseesCommand() const = 0;
	};
}

