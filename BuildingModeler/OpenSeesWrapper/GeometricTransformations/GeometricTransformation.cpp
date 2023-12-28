#include "GeometricTransformation.h"

using namespace opensees;

GeometricTransformation::GeometricTransformation(int transfTag, std::vector<int> vecXZ) : m_transfTag(transfTag), m_vecXZ(vecXZ)
{

}

int GeometricTransformation::getTransfTag() const
{
	return m_transfTag;
}

GeometricTransformationType GeometricTransformation::getTransfType() const
{
	return m_transfType;
}

const std::vector<int>& GeometricTransformation::getVectorXZ() const
{
	return m_vecXZ;
}