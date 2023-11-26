#pragma once

#include "../OpenseesObject.h"

namespace opensees
{
	enum class LoadType
	{
		NODALLOAD,
		ELEMENTLOAD
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
	};
}

