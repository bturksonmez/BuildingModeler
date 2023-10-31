#pragma once

#include "GeometricTransformation.h"

namespace opensees
{
	class LinearGeometricTransformation : public GeometricTransformation
	{
	public:
		LinearGeometricTransformation(int transfTag, std::vector<int> m_vecXZ);
		LinearGeometricTransformation() = delete;
		~LinearGeometricTransformation() {}

		std::string getOpenseesCommand() const override;
	};
}

