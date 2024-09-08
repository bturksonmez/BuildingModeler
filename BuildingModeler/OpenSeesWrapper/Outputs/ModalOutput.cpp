#include "ModalOutput.h"

#include <cmath>

using namespace opensees;

ModalOutput::ModalOutput(std::string loadTag, std::vector<int> masterNodeTags) : Output(loadTag)
{
	m_outputType = OutputType::MODAL;
	m_masterNodeTags = masterNodeTags;
}

const std::vector<double>& ModalOutput::getPeriods() const
{
	return m_periods;
}

void ModalOutput::retrieveOutput()
{
	retrievePeriods();
	retrieveModeShapes();
}

void ModalOutput::retrievePeriods()
{
	std::string outputFile = "lambda.out";
	auto lambdas = utilities::DataReader::readContinuosLine<double>(outputFile);

	for (auto lambda : lambdas) {
		// To do: add PI global
		auto period = 2.0 * 3.141593 / std::pow(lambda, 0.5);
		m_periods.push_back(period);
	}
}

void ModalOutput::retrieveModeShapes()
{
	auto numOfModes = m_periods.size();

	for (const auto tag : m_masterNodeTags) {
		m_nodeDispOutput[tag].resize(numOfModes);
	}

	for (int i = 0; i < numOfModes; i++)
	{
		auto vecX = utilities::DataReader::readContinuosLine<double>("mode1" + std::to_string(i + 1) + ".out");
		auto vecY = utilities::DataReader::readContinuosLine<double>("mode2" + std::to_string(i + 1) + ".out");
		auto vecXY = utilities::DataReader::readContinuosLine<double>("mode6" + std::to_string(i + 1) + ".out");

		for (int j = 0; j < vecX.size(); ++j) {
			m_nodeDispOutput[m_masterNodeTags[j]][i].push_back(vecX[j]);
			m_nodeDispOutput[m_masterNodeTags[j]][i].push_back(vecY[j]);
			m_nodeDispOutput[m_masterNodeTags[j]][i].push_back(vecXY[j]);
		}
	}
}