#include "RectangularPatch.h"

using namespace std;
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

string RectangularPatch::getOpenseesCommand() const
{
	string command;

	command = "patch rect " + to_string(m_materialTag) + " " + to_string(m_numDivY) + " " + to_string(m_numDivZ) + " " + to_string(m_yI)
			   + " " + to_string(m_zI) + " " + to_string(m_yJ) + " " + to_string(m_zJ) + "\n";

	return command;
}