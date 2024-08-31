#pragma once

#include <limits>

#include "Vector2.h"
#include "Vector3.h"

namespace utility
{
	class VectorUtilities
	{
	public:
		static bool intersectsVector2(const Vector2& pA, const Vector2& pB, const Vector2& qA, const Vector2& qB, Vector2& intersection);
		static double dotProduct(const Vector3& vecA, const Vector3& vecB);
		static Vector3 crossProduct(const Vector2& vecA, const Vector2& vecB);
		static Vector3 crossProduct(const Vector3& vecA, const Vector3& vecB);
		static double getAngleBtw(const Vector3& vecA, const Vector3& vecB);
		static double getSignedAngleBtw(const Vector3& vecA, const Vector3& vecB, const Vector3& referenceAxis);
		static double calculateSlope(const Vector2& pA, const Vector2& pB);
		static double calculateQuadArea(const Vector3& pointI, const Vector3& pointJ, const Vector3& pointK, const Vector3& pointL);
		static Vector2 projectVectorOn2DLocalBasis(const Vector3& vec, const Vector3& u, const Vector3& v);
		static Vector3 normalize(const Vector3& vec);
		static Vector2 normalize(const Vector2& vec);
		static bool isBetween(const Vector2& pA, const Vector2& pB, const Vector2& point);
	};
}