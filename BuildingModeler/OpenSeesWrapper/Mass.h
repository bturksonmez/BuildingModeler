#pragma once

#include "OpenseesObject.h"
#include "../Utilities/Vector3.h"

namespace opensees
{
	class Mass : public OpenseesObject
	{
	protected:
		int m_nodeTag;
		utility::Vector3 m_massTranslational;
		utility::Vector3 m_massRotational;

	public:
		Mass(int nodeTag, utility::Vector3 m_massTranslational = {}, utility::Vector3 m_massRotational = {});
		Mass() = delete;
		~Mass() {}

		int getNodeTag() const;
		const utility::Vector3& getTranslationalMass() const;
		const utility::Vector3& getRotationalMass() const;

		std::string getOpenseesCommand() const override;
	};
}
