#include "ShellElement.h"

using namespace opensees;

ShellElement::ShellElement(int elementTag, std::vector<int> nodeTags, std::shared_ptr<Section> section, ShellElementType type = ShellElementType::DKGQ)
	: QuadrilateralElement(elementTag, nodeTags, section)
{
	m_shellElementType = type;
}

ShellElementType ShellElement::getShellElementType() const
{
    return m_shellElementType;
}

std::string ShellElement::getOpenseesCommand() const
{
    std::string command;

    std::string shellType;
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

	command = "element " + shellType + " " + std::to_string(m_elementTag) + " " + std::to_string(m_nodeTags[0]) + " " + std::to_string(m_nodeTags[1])
        + " " + std::to_string(m_nodeTags[2]) + " " + std::to_string(m_nodeTags[3]) + " " + std::to_string(m_section->getSectionTag());

	command += ("\n");

	return command;
}
