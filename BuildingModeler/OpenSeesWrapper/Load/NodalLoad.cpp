#include "NodalLoad.h"
#include "../Node.h"

using namespace std;
using namespace opensees;

NodalLoad::NodalLoad(shared_ptr<Node> node, vector<double> loadVector) : Load(loadVector)
{
	m_node = node;
	m_loadType = LoadType::NODALLOAD;

	Load* load = this;
	if (auto locked_sp = m_node.lock())
		locked_sp->addNodalLoad(shared_ptr<Load>(load));
}

const shared_ptr<Node> NodalLoad::getNode() const
{
	return m_node.lock();
}

string NodalLoad::getOpenseesCommand() const
{
    string command;

    command = "load " + to_string(m_node.lock()->getNodeTag()) + " " + to_string(m_loadVector[0]) + " " + to_string(m_loadVector[1]) +  " " + to_string(m_loadVector[2]) 
		+ " " + to_string(m_loadVector[3]) + " " + to_string(m_loadVector[4]) + " " + to_string(m_loadVector[5]);

    command += ("\n");

    return command;
}