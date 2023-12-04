#include "ColumnElement.h"

using namespace std;
using namespace physicalModel;

ColumnElement::ColumnElement(int elementTag, std::vector<int> jointTags, std::vector<std::shared_ptr<Section>> sections,
	std::vector<std::shared_ptr<SectionModifiers>> sectionModifiers, LineElementFormulation lineElementFormulation)
	: LineElement(elementTag, jointTags, sections, sectionModifiers, lineElementFormulation)
{
	m_lineElementType = LineElementType::COLUMN;
}