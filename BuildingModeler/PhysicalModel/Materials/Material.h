#pragma once

namespace physicalModel
{
	enum class MaterialType
	{
		ELASTIC,
		CONCRETE_KENT_PARK_NOTENSION,
		CONCRETE_KENT_PARK,
		CONCRETE_MANDER,
		CONCRETE_CHANG_MANDER,
		STEEL_BILINEAR,
		STEEL_GMP
	};

	class Material
	{
	protected:
		int m_materialTag;
		MaterialType m_materialType;
		double m_E;
		double m_G;
		double m_rho;

		Material(int materialTag, double E, double G, double rho) : m_materialTag(materialTag), m_E(E), m_G(G), m_rho(rho) {}

	public:
		Material() = delete;
		Material(Material&& other) = default;
		Material& operator=(Material&& other) = default;
		virtual ~Material() {}

		int getMaterialTag() const;
		MaterialType getMaterialType() const;
		double getRho() const;
		double getE() const;
		double getG() const;
	};
}

