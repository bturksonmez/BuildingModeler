#pragma once

#include "Vector2.h"
#include "Vector3.h"

namespace utility
{
	class LineUtilities
	{
	public:
		static bool intersectsVector3(const Vector3& pA, const Vector3& pB, const Vector3& qA, const Vector3& qB, Vector3& intersection);
	};
}