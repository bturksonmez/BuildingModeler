#include "ElementLoad.h"
#include "../Elements/Element.h"

using namespace std;
using namespace opensees;

ElementLoad::ElementLoad(shared_ptr<Element> element, vector<double> loadVector) : Load(loadVector)
{
	m_element = element;
	m_loadType = LoadType::NODALLOAD;

	Load* ele = this;
	if (auto locked_sp = m_element.lock())
		locked_sp->addElementLoad(shared_ptr<Load>(ele));
}

const shared_ptr<Element> ElementLoad::getElement() const
{
	return m_element.lock();
}

string ElementLoad::getOpenseesCommand() const
{
	string command;

	command = "eleLoad -ele " + to_string(m_element.lock()->getElementTag()) + " -type -beamUniform " + to_string(m_loadVector[0]) 
			+ " " + to_string(m_loadVector[1]) + " " + to_string(m_loadVector[2]);

	command += ("\n");

	return command;
}