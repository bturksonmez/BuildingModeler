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

		Section(int sectionTag, std::shared_ptr<Material> material) : m_sectionTag(sectionTag), m_material(material){}

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


