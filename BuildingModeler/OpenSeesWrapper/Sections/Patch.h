#pragma once

#include "../Object.h"

namespace opensees
{
	enum class PatchType
	{
		Quad,
		Rect,
		Circ
	};

	class Patch : public Object
	{
	protected:
		int m_materialTag;
		PatchType m_patchType;

		Patch(int materialTag) : m_materialTag(materialTag) {}

	public:
		Patch() = delete;
		~Patch() {}

		int getMaterialTag() const;
		PatchType getPatchType() const;
	};
}
