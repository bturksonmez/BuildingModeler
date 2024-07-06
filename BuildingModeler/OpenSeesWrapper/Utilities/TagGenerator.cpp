#include "TagGenerator.h"

using namespace opensees::utilities;

void TagGenerator::resetTags()
{
	m_nodeTag = 100001;
	m_columnTag = 100001;
	m_beamTag = 300001;
	m_slabTag = 400001;
	m_shearWallTag = 500001;
}

TagGenerator& TagGenerator::getInstance()
{
	static TagGenerator instance;
	return instance;
}

int TagGenerator::getNextNodeTag()
{
	return m_nodeTag++;
}

int TagGenerator::getNextColumnTag()
{
	return m_columnTag++;
}

int TagGenerator::getNextBeamTag()
{
	return m_beamTag++;
}

int TagGenerator::getNextSlabTag()
{
	return m_slabTag++;
}

int TagGenerator::getNextShearWallTag()
{
	return m_shearWallTag++;
}