#include "LinearGeometricTransformation.h"

using namespace opensees;

LinearGeometricTransformation::LinearGeometricTransformation(int transfTag, std::vector<int> m_vecXZ)
	: GeometricTransformation(transfTag, m_vecXZ)
{
	m_transfType = GeometricTransformationType::LINEAR;
}

std::string LinearGeometricTransformation::getOpenseesCommand() const
{
	std::string command;

	command = "geomTransf Linear " + std::to_string(m_transfTag);

	for (auto val : m_vecXZ)
		command += (" " + std::to_string(val));

	command += ("\n");

	return command;
}