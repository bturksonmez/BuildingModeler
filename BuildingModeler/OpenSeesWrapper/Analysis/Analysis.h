#pragma once

#include "../OpenseesObject.h"

namespace opensees
{
	enum class AnalysisType
	{
		STATIC,
		MODAL
	};

	class Analysis : public OpenseesObject
	{
	protected:
		size_t m_analysisID;
		std::string m_sourceModelName;
		AnalysisType m_analysisType;
		static size_t counter;

		Analysis(std::string sourceModelName);

	public:
		Analysis() = delete;
		~Analysis() {}

		size_t getAnalysisID() const;
		std::string getSourceModelName() const;
		AnalysisType getAnalysisType() const;
		
		struct AnalysisHash
		{
			size_t operator()(const std::shared_ptr<Analysis>& ptr) const
			{
				return std::hash<size_t>()(ptr->getAnalysisID());
			}
		};

		struct AnalysisEqual
		{
			bool operator()(const std::shared_ptr<Analysis>& lhs, const std::shared_ptr<Analysis>& rhs) const
			{
				return lhs->getAnalysisID() == rhs->getAnalysisID();
			}
		};
	};
}