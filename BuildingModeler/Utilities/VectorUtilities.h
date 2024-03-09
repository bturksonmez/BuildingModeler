#pragma once

#include "Vector2.h"
#include "Vector3.h"

namespace utility
{
	class VectorUtilities
	{
	public:
		static bool intersectsVector3(const Vector3& pA, const Vector3& pB, const Vector3& qA, const Vector3& qB, Vector3& intersection);
		static double dotProduct(const Vector3& vecA, const Vector3& vecB);
		static double getAngleBtw(const Vector3& vecA, const Vector3& vecB);
	};
}