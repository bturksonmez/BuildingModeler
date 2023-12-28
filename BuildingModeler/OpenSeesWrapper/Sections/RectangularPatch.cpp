#include "RectangularPatch.h"

using namespace opensees;

RectangularPatch::RectangularPatch(int materialTag, int numDivY, int numDivZ, double yI, double zI, double yJ, double zJ) : Patch(materialTag)
{
	m_numDivY = numDivY;
	m_numDivZ = numDivZ;
	m_yI = yI;
	m_zI = zI;
	m_yJ = yJ;
	m_zJ = zJ;

	m_patchType = PatchType::Rect;
}

int RectangularPatch::getNumDivY() const
{
	return m_numDivY;
}

int RectangularPatch::getNumDivZ() const
{
	return m_numDivZ;
}

double RectangularPatch::getYI() const
{
	return m_yI;
}

double RectangularPatch::getZI() const
{
	return m_zI;
}

double RectangularPatch::getYJ() const
{
	return m_yJ;
}

double RectangularPatch::getZJ() const
{
	return m_zJ;
}

std::string RectangularPatch::getOpenseesCommand() const
{
	std::string command;

	command = "patch rect " + std::to_string(m_materialTag) + " " + std::to_string(m_numDivY) + " " + std::to_string(m_numDivZ) + " " + std::to_string(m_yI)
			   + " " + std::to_string(m_zI) + " " + std::to_string(m_yJ) + " " + std::to_string(m_zJ) + "\n";

	return command;
}