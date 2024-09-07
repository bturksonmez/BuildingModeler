#pragma once

#include "../OpenseesObject.h"
#include "NodalLoad.h"
#include "ElementLoad.h"

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
		std::string m_loadingName;
		TimeSeriesType m_timeSeriesType;
		std::vector<std::shared_ptr<Load>> m_loadVector;
		static int counter;

	public:
		LoadPattern(std::string loadingName, TimeSeriesType timeSeriesType) : m_loadingName(loadingName), m_timeSeriesType(timeSeriesType) { m_patternTag = counter++; }
		LoadPattern() = delete;
		~LoadPattern() {}

		int getPatternTag() const;
		std::string getLoadingName() const;
		TimeSeriesType getTimeSeriesType() const;
		const std::vector<std::shared_ptr<Load>>& getLoadVector() const;
		void addLoad(std::shared_ptr<Load> load);
		static void resetCounter();

		std::string getOpenseesCommand() const override;
	};
}
