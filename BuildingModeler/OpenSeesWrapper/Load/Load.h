#pragma once

#include "../OpenseesObject.h"

namespace opensees
{
	enum class LoadType
	{
		NODAL_LOAD,
		ELEMENT_LOAD
	};

	class Load : public OpenseesObject
	{
	protected:
		LoadType m_loadType;
		std::vector<double> m_loadVector;

		Load(std::vector<double> loadVector) : m_loadVector(loadVector) {}

	public:
		Load() = delete;
		~Load() {}

		LoadType getLoadType() const;
		const std::vector<double>& getLoadVector() const;

		Load& operator*=(const double c);
	};
}

