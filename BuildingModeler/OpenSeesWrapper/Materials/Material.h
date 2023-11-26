#pragma once

#include "../OpenseesObject.h"

namespace opensees
{
	enum class MaterialType
	{
		ELASTIC,
		CONCRETE01,
		CONCRETE02,
		CONCRETE07,
		CONCRETECM,
		STEEL01,
		STEEL02

	};

	class Material : public OpenseesObject
	{
	protected:
		int m_materialTag;
		MaterialType m_materialType;
		double m_E;
		double m_G;
		double m_rho;

		Material(int materialTag, double E, double G = 0, double rho = 0) : m_materialTag(materialTag), m_E(E), m_G(G), m_rho(rho)  {}

	public:
		Material() = delete;
		~Material() {}

		int getMaterialTag() const;
		MaterialType getMaterialType() const;
		double getRho() const;
		double getE() const;
		double getG() const;
	};
}

