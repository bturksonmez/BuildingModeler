#pragma once

#include <iostream>

namespace opensees
{
	enum PatchType
	{
		Quad,
		Rect,
		Circ
	};

	class Patch
	{
	protected:
		int m_materialTag;
		PatchType m_patchType;

		Patch(int materialTag) : m_materialTag(materialTag) {}

	public:
		virtual ~Patch() {}

		int getMaterialTag() const;
		PatchType getPatchType() const;

		virtual std::string getOpenseesCommand() const = 0;
	};
}
