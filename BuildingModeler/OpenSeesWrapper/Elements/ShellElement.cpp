#include "ShellElement.h"

using namespace std;
using namespace opensees;

ShellElement::ShellElement(int elementTag, vector<shared_ptr<Node>> nodes, shared_ptr<Section> section, ShellElementType type = ShellElementType::DKGQ) 
	: QuadrilateralElement(elementTag, nodes, section)
{
	m_shellElementType = type;
}

ShellElementType ShellElement::getShellElementType() const
{
    return m_shellElementType;
}

string ShellElement::getOpenseesCommand() const
{
	string command;

    string shellType;
	switch(m_shellElementType) {
        case ShellElementType::DKGQ:
            shellType = "ShellDKGQ";
            break;
        case ShellElementType::NLDKGQ:
            shellType = "ShellNLDKGQ";
            break;
        case ShellElementType::MITC4:
            shellType = "ShellMITC4";
            break;
        default:
            shellType = "ShellDKGQ";
    }

	command = "element " + shellType + " " + to_string(m_elementTag) + " " + to_string(m_nodes[0].lock()->getNodeTag()) + " " + to_string(m_nodes[1].lock()->getNodeTag())
        + " " + to_string(m_nodes[2].lock()->getNodeTag()) + " " + to_string(m_nodes[3].lock()->getNodeTag()) + " " + to_string(m_section->getSectionTag());

	command += ("\n");

	return command;
}
