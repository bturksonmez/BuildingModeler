#pragma once

#include "Object.h"

namespace opensees
{
	enum class MaterialType
	{
		Uniaxial,
		Ndim
	};

	class Material : Object
	{
	protected:
		int m_materialTag;
		MaterialType m_materialType;

		Material(int materialTag) : m_materialTag(materialTag) {}

	public:
		Material() = delete;
		~Material() {}

		int getMaterialTag() const;
		MaterialType getMaterialType() const;
	};
}

