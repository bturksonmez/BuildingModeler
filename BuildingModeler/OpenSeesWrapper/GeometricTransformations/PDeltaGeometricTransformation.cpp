#include "PDeltaGeometricTransformation.h"

using namespace std;
using namespace opensees;

PDeltaGeometricTransformation::PDeltaGeometricTransformation(int transfTag, vector<int> m_vecXZ)
	: GeometricTransformation(transfTag, m_vecXZ)
{
	m_transfType = GeometricTransformationType::PDELTA;
}

string PDeltaGeometricTransformation::getOpenseesCommand() const
{
	string command;

	command = "geomTransf PDelta " + to_string(m_transfTag);

	for (auto val : m_vecXZ)
		command += (" " + to_string(val));

	command += ("\n");

	return command;
}