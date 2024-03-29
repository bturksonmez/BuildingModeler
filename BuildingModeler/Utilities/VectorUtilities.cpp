#include "VectorUtilities.h"

namespace utility
{
	bool VectorUtilities::intersectsVector2(const Vector2& pA, const Vector2& pB, const Vector2& qA, const Vector2& qB, Vector2& intersection)
    {
        auto ma = calculateSlope(pA, pB);
        auto mb = calculateSlope(qA, qB);

        auto ba = pA.y - ma * pA.x;
        auto bb = qA.y - mb * qA.x;

        if (ma == std::numeric_limits<double>::max()) {
            
            auto y = mb * pA.x + bb;
            if (y <= pB.y && y >= pA.y) {
                intersection.x = pA.x;
                intersection.y = y;
                return true;
            }

            return false;
        }
        else if (mb == std::numeric_limits<double>::max()) {

            auto y = ma * qA.x + ba;
            if (y <= qB.y && y >= qA.y) {
                intersection.x = qA.x;
                intersection.y = y;
                return true;
            }

            return false;
        }
        else if (ma == mb) {
            return false;
        }
        else {
            auto x = (bb - ba) / (ma - mb);
            auto y = ma * x + ba;

            if (isBetween(pA, pB, { x, y }) && isBetween(qA, qB, { x, y })) {
                intersection.x = x;
                intersection.y = y;
                return true;
            }

            return false;
        }
    }

    double VectorUtilities::dotProduct(const Vector3& vecA, const Vector3& vecB)
    {
        return vecA.x * vecB.x + vecA.y * vecB.y + vecA.z * vecB.z;
    }

    Vector3 VectorUtilities::crossProduct(const Vector2& vecA, const Vector2& vecB)
    {
        auto z = vecA.x * vecB.y - vecA.y * vecB.x;

        return Vector3{ 0.0, 0.0, z };
    }

    Vector3 VectorUtilities::crossProduct(const Vector3& vecA, const Vector3& vecB)
    {
        auto x = vecA.y * vecB.z - vecA.z * vecB.y;
        auto y = vecA.z * vecB.x - vecA.x * vecB.z;
        auto z = vecA.x * vecB.y - vecA.y * vecB.x;

        return Vector3{ x, y, z };
    }

    double VectorUtilities::getAngleBtw(const Vector3& vecA, const Vector3& vecB)
    {
        return std::acos(dotProduct(vecA, vecB) / (vecA.norm2() * vecB.norm2()));
    }

    double VectorUtilities::calculateSlope(const Vector2& pA, const Vector2& pB)
    {
        if (pB.x - pA.x == 0) {
            return std::numeric_limits<double>::max(); // Avoid division by zero
        }

        return (pB.y - pA.y) / (pB.x - pA.x);
    }

    Vector2 VectorUtilities::projectVectorOn2DLocalBasis(const Vector3& vec, const Vector3& u, const Vector3& v)
    {
        utility::Vector2 projectedVec;

        projectedVec.x = utility::VectorUtilities::dotProduct(vec, u);
        projectedVec.y = utility::VectorUtilities::dotProduct(vec, v);

        return projectedVec;
    }

    bool VectorUtilities::isBetween(const Vector2& pA, const Vector2& pB, const Vector2& point)
    {
        if (point.x >= std::min(pA.x, pB.x) - 1e-10 && point.x <= std::max(pA.x, pB.x) + 1e-10
            && point.y >= std::min(pA.y, pB.y) - 1e-10 && point.y <= std::max(pA.y, pB.y) + 1e-10) {
            return true;
        }

        return false;
    }
}