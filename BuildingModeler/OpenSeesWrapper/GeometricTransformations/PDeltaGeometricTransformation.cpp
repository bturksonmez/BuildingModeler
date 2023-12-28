#include "PDeltaGeometricTransformation.h"

using namespace opensees;

PDeltaGeometricTransformation::PDeltaGeometricTransformation(int transfTag, std::vector<int> m_vecXZ)
	: GeometricTransformation(transfTag, m_vecXZ)
{
	m_transfType = GeometricTransformationType::PDELTA;
}

std::string PDeltaGeometricTransformation::getOpenseesCommand() const
{
	std::string command;

	command = "geomTransf PDelta " + std::to_string(m_transfTag);

	for (auto val : m_vecXZ)
		command += (" " + std::to_string(val));

	command += ("\n");

	return command;
}