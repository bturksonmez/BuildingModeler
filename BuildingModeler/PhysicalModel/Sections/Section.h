#pragma once

#include <memory>
#include <optional>

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
		std::optional<double> m_A;
		std::optional<double> m_Iyy;
		std::optional<double> m_Izz;
		std::optional<double> m_J;

		Section(int sectionTag, std::shared_ptr<Material> material) : m_sectionTag(sectionTag), m_material(material){}

	public:
		Section() = delete;
		Section(Section&& other) = default;
		Section& operator=(Section&& other) = default;
		virtual ~Section() {}

		int getSectionTag() const;
		SectionType getSectionType() const;
		const std::shared_ptr<Material> getMaterial() const;
		std::optional<double> getA() const;
		std::optional<double> getIyy() const;
		std::optional<double> getIzz() const;
		std::optional<double> getJ() const;
	};
}


