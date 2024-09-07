#pragma once

#include <memory>
#include <vector>
#include <string>

namespace physicalModel
{
	enum class LoadCaseType
	{
		STATIC,
		MODAL
	};

	class LoadCase
	{
	protected:
		bool m_active = false;
		std::string m_loadCaseTag;
		LoadCaseType m_loadCaseType;

		LoadCase(std::string loadCaseTag);

	public:
		
		LoadCase() = delete;
		virtual ~LoadCase() {}

		void setActive(bool active);
		virtual void clear() {};

		bool isActive() const;
		std::string getLoadCaseTag() const;
		LoadCaseType getLoadCaseType() const;
	};
}