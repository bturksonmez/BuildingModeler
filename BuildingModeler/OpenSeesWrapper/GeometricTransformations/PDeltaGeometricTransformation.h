#pragma once

#include "GeometricTransformation.h"

namespace opensees
{
	class PDeltaGeometricTransformation : public GeometricTransformation
	{
	public:
		PDeltaGeometricTransformation(int transfTag, std::vector<int> m_vecXZ);
		PDeltaGeometricTransformation() = delete;
		~PDeltaGeometricTransformation() {}

		std::string getOpenseesCommand() const override;
	};
}

