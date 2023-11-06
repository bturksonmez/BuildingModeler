#include "LinearGeometricTransformation.h"

using namespace std;
using namespace opensees;

LinearGeometricTransformation::LinearGeometricTransformation(int transfTag, vector<int> m_vecXZ)
	: GeometricTransformation(transfTag, m_vecXZ)
{
	m_transfType = GeometricTransformationType::LINEAR;
}

string LinearGeometricTransformation::getOpenseesCommand() const
{
	string command;

	command = "geomTransf Linear " + to_string(m_transfTag);

	for (auto val : m_vecXZ)
		command += (" " + to_string(val));

	command += ("\n");

	return command;
}