#include "Mass.h"

using namespace opensees;
using namespace utility;

Mass::Mass(int nodeTag, Vector3 massTranslational,Vector3 massRotational) : 
	m_nodeTag(nodeTag), m_massTranslational(massTranslational), m_massRotational(massRotational)
{
}

void Mass::addTranslationalMass(utility::Vector3 massTranslational)
{
	m_massTranslational = m_massTranslational + massTranslational;
}

void Mass::addRotationalMass(utility::Vector3 massRotational)
{
	m_massRotational = m_massRotational + massRotational;
}

int Mass::getNodeTag() const
{
	return m_nodeTag;
}

const Vector3& Mass::getTranslationalMass() const
{
	return m_massTranslational;
}

const Vector3& Mass::getRotationalMass() const
{
	return m_massRotational;
}

std::string Mass::getOpenseesCommand() const
{
	std::string command;

	command = "mass " + std::to_string(m_nodeTag);

	command += (" " + std::to_string(m_massTranslational.x));
	command += (" " + std::to_string(m_massTranslational.y));
	command += (" " + std::to_string(m_massTranslational.z));

	command += (" " + std::to_string(m_massRotational.x));
	command += (" " + std::to_string(m_massRotational.y));
	command += (" " + std::to_string(m_massRotational.z));

	command += ("\n");

	return command;
}