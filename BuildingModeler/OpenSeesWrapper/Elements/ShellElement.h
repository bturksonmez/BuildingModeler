#pragma once

#include "QuadrilateralElement.h"

namespace opensees
{
    enum class ShellElementType
	{
		DKGQ,
        NLDKGQ,
		MITC4
	};
    
	class ShellElement : public QuadrilateralElement
	{
    private:
        ShellElementType m_shellElementType;

	public:
		ShellElement(int elementTag, std::vector<std::shared_ptr<Node>> nodes, std::shared_ptr<Section> section, ShellElementType type);
		ShellElement() = delete;
		~ShellElement() {}

        ShellElementType getShellElementType() const;
		std::string getOpenseesCommand() const override;
	};
}