#pragma once

#include <memory>

#include "../Materials/Material.h"

namespace physicalModel
{
	enum class SectionType
	{
		ELASTIC1D,
		ELASTIC2D,
		FIBER1D,
	};

	class Section
	{
	protected:
		int m_sectionTag;
		SectionType m_sectionType;
		std::shared_ptr<Material> m_material;
		double m_A;
		double m_Iyy;
		double m_Izz;
		double m_J;

		Section(int sectionTag, std::shared_ptr<Material> material, double A = 0, double Iyy = 0, double Izz = 0, double J = 0)
			: m_sectionTag(sectionTag), m_material(material), m_A(A), m_Iyy(Iyy), m_Izz(Izz), m_J(J) {}

	public:
		Section() = delete;
		Section(Section&& other) = default;
		Section& operator=(Section&& other) = default;
		virtual ~Section() {}

		int getSectionTag() const;
		SectionType getSectionType() const;
		const std::shared_ptr<Material> getMaterial() const;
		double getA() const;
		double getIyy() const;
		double getIzz() const;
		double getJ() const;
	};
}


