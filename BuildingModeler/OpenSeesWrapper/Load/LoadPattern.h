#pragma once

#include "../OpenseesObject.h"
#include "Load.h"

namespace opensees
{
	enum class TimeSeriesType
	{
		CONSTANT,
		LINEAR
	};

	class LoadPattern : public OpenseesObject
	{
	protected:
		int m_patternTag;
		TimeSeriesType m_timeSeriesType;
		std::vector<std::shared_ptr<Load>> m_loadVector;

		LoadPattern(int patternTag, TimeSeriesType timeSeriesType) : m_patternTag(patternTag), m_timeSeriesType(timeSeriesType) {}

	public:
		LoadPattern() = delete;
		~LoadPattern() {}

		int getPatternTag() const;
		TimeSeriesType getTimeSeriesType() const;
		const std::vector<std::shared_ptr<Load>>& getLoadVector() const;
		void addLoad(std::shared_ptr<Load> load);
		std::string getOpenseesCommand() const override;
	};
}
