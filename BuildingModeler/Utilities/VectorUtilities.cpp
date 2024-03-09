#include "VectorUtilities.h"

namespace utility
{
	bool VectorUtilities::intersectsVector3(const Vector3 & pA, const Vector3 & pB, const Vector3 & qA, const Vector3 & qB, Vector3 & intersection)
    {
        double eps = 1e-10;
        Vector3 d1 = pB - pA;
        Vector3 d2 = qB - qA;
    
        auto denominator = (d1.x - d1.y * d2.x / d2.y);
        if (std::abs(denominator) < 1e-10) {
            return false;
        }
    
        auto s = 0.0;// (qA.x - qA.y * d2.x / d2.y - pA.x + pA.y * d2.x / d2.y) / denominator;
        auto t = 0.5;// (pA.x + s * d1.x - qA.x) / d2.x;
    
        auto corrector = pA.z + s * d1.z - qA.z - t * d2.z;
        if (std::abs(corrector) > 1e-10) {
            return false;
        }
    
        // Check if the intersection point lies within the line segments
        if (s > -eps && s < 1.0 + eps && t > -eps && t < 1.0 + eps) {
            intersection.x = qA.x + t * d2.x;
            intersection.y = qA.y + t * d2.y;
            intersection.z = qA.z + t * d2.z;
    
            return true;
        }
    
        // Intersection point is outside the line segments
        return false;
    }

    double VectorUtilities::dotProduct(const Vector3& vecA, const Vector3& vecB)
    {
        return vecA.x * vecB.x + vecA.y * vecB.y + vecA.z * vecB.z;
    }

    double VectorUtilities::getAngleBtw(const Vector3& vecA, const Vector3& vecB)
    {
        return std::acos(dotProduct(vecA, vecB) / (vecA.norm2() * vecB.norm2()));
    }
}