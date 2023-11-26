#pragma once

#include "../OpenseesObject.h"
#include "../Materials/Material.h"

namespace opensees
{
	enum class SectionType
	{
		ELASTIC,
		FIBER,
		ELASTICMEMBRANEPLATE
	};

	class Section : public OpenseesObject
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
			: m_sectionTag(sectionTag), m_material(material), m_A(A), m_Iyy(Iyy), m_Izz(Izz), m_J(J)  {}

	public:
		Section() = delete;
		~Section() {}

		int getSectionTag() const;
		SectionType getSectionType() const;
		const std::shared_ptr<Material> getMaterial() const;
		double getA() const;
		double getIyy() const;
		double getIzz() const;
		double getJ() const;
	};
}

