#include "ColumnElement.h"

using namespace physicalModel;

ColumnElement::ColumnElement(int elementTag, std::vector<int> jointTags, std::shared_ptr<Section> section, LineElementFormulation lineElementFormulation)
	: LineElement(elementTag, jointTags, section, lineElementFormulation)
{
	m_lineElementType = LineElementType::COLUMN;
}