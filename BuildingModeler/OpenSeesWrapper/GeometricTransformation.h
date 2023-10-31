#pragma once

#include "Object.h"

namespace opensees
{
	enum class GeometricTransformationType
	{
		LINEAR,
		PDELTA
	};

	class GeometricTransformation : public Object
	{
	protected:
		int m_transfTag;
		GeometricTransformationType m_transfType;
		std::vector<int> m_vecXZ;

		GeometricTransformation(int transfTag, std::vector<int> m_vecXZ);

	public:
		GeometricTransformation() = delete;
		~GeometricTransformation() {}

		int getTransfTag() const;
		GeometricTransformationType getTransfType() const;
		const std::vector<int>& getVectorXZ() const;
	};
}

