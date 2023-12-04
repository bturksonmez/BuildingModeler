#include "ShearWallElement.h"

using namespace std;
using namespace physicalModel;

ShearWallElement::ShearWallElement(int elementTag, std::vector<int> jointTags, std::shared_ptr<Section> section, AreaElementFormulation areaElementFormulation)
	: AreaElement(elementTag, jointTags, section, areaElementFormulation)
{
	m_areaElementType = AreaElementType::SHEARWALL;
}