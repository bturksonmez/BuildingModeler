#pragma once

#include "../Object.h"

namespace opensees
{
	enum class LoadType
	{
		NODALLOAD,
		ELEMENTLOAD
	};

	class Load : public Object
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

