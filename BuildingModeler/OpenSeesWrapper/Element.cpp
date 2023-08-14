#include "Element.h"

using namespace std;
using namespace opensees;

int Element::getElementTag() const
{
	return m_elementTag;
}

ElementType Element::getElementType() const
{
	return m_elementType;
}