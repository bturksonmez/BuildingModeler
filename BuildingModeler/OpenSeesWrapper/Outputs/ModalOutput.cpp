#include "ModalOutput.h"
#include "../OpenseesModel.h"

#include <cmath>

using namespace opensees;

ModalOutput::ModalOutput(std::string loadTag, std::vector<int> masterNodeTags) : Output(loadTag)
{
	m_outputType = OutputType::MODAL;
	m_masterNodeTags = masterNodeTags;
}

const std::vector<std::vector<double>>& ModalOutput::getModeShapeX()
{
	return m_modeShapeX;
}

const std::vector<std::vector<double>>& ModalOutput::getModeShapeY()
{
	return m_modeShapeY;
}

const std::vector<std::vector<double>>& ModalOutput::getModeShapeXY()
{
	return m_modeShapeXY;
}

const std::vector<double>& ModalOutput::getPeriods() const
{
	return m_periods;
}

double ModalOutput::getFundamentalPeriod(size_t dof)
{
	std::vector<double> desiredEigenVector;
	std::vector<double> candidateEigenVector;

	for (int i = 0; i < m_periods.size(); ++i) {

		if (dof == 1) {
			desiredEigenVector = m_modeShapeX[i];
			candidateEigenVector = m_modeShapeY[i];
		}
		else {
			desiredEigenVector = m_modeShapeX[i];
			candidateEigenVector = m_modeShapeY[i];
		}

		if (isMonotonicallyIncreasingEigenVector(desiredEigenVector)) {

			if (isBetterThanCandidate(desiredEigenVector, candidateEigenVector)) {

				return m_periods[i];
			}
		}
	}

	return -1.0;
}

bool ModalOutput::retrieveOutput()
{
	bool periodsRetrieved = retrievePeriods();

	if (!periodsRetrieved) {

		return false;
	}
	retrieveModeShapes();
	retrieveModeShapesAsDisplacements();

	return true;
}

bool ModalOutput::retrievePeriods()
{
	std::string processID = std::to_string(OpenseesModel::getInstance().processID);

	std::string outputFile = "lambda_" + processID + ".out";
	auto lambdas = utilities::DataReader::readContinuosLine<double>(outputFile);

	if (lambdas.empty()) {

		return false;
	}

	for (auto lambda : lambdas) {
		// To do: add PI global
		auto period = 2.0 * 3.141593 / std::pow(lambda, 0.5);
		m_periods.push_back(period);
	}

	return true;
}

void ModalOutput::retrieveModeShapes()
{
	std::string processID = std::to_string(OpenseesModel::getInstance().processID);

	auto numOfModes = m_periods.size();
	
	m_modeShapeX.resize(numOfModes);
	m_modeShapeY.resize(numOfModes);
	m_modeShapeXY.resize(numOfModes);

	for (int i = 0; i < numOfModes; i++)
	{
		auto vecX = utilities::DataReader::readContinuosLine<double>("mode1" + std::to_string(i + 1) + "_" + processID + ".out");
		auto vecY = utilities::DataReader::readContinuosLine<double>("mode2" + std::to_string(i + 1) + "_" + processID + ".out");
		auto vecXY = utilities::DataReader::readContinuosLine<double>("mode6" + std::to_string(i + 1) + "_" + processID + ".out");

		m_modeShapeX[i] = vecX;
		m_modeShapeY[i] = vecY;
		m_modeShapeXY[i] = vecXY;
	}
}

void ModalOutput::retrieveModeShapesAsDisplacements()
{
	std::string processID = std::to_string(OpenseesModel::getInstance().processID);

	auto numOfModes = m_periods.size();

	for (const auto tag : m_masterNodeTags) {
		m_nodeDispOutput[tag].resize(numOfModes);
	}

	for (int i = 0; i < numOfModes; i++)
	{
		auto vecX = utilities::DataReader::readContinuosLine<double>("mode1" + std::to_string(i + 1) + "_" + processID + ".out");
		auto vecY = utilities::DataReader::readContinuosLine<double>("mode2" + std::to_string(i + 1) + "_" + processID + ".out");
		auto vecXY = utilities::DataReader::readContinuosLine<double>("mode6" + std::to_string(i + 1) + "_" + processID + ".out");

		for (int j = 0; j < vecX.size(); ++j) {
			m_nodeDispOutput[m_masterNodeTags[j]][i].push_back(vecX[j]);
			m_nodeDispOutput[m_masterNodeTags[j]][i].push_back(vecY[j]);
			m_nodeDispOutput[m_masterNodeTags[j]][i].push_back(vecXY[j]);
		}
	}
}

bool ModalOutput::isMonotonicallyIncreasingEigenVector(const std::vector<double>& eigenVector)
{
	double sign1 = eigenVector[0] > 0 ? 1 : -1;
	for (int i = 1; i < eigenVector.size(); i++)
		if (eigenVector[i] * sign1 < 0)
			return false;

	for (int i = 1; i < eigenVector.size(); i++)
		if (std::abs(eigenVector[i]) < std::abs(eigenVector[i - 1]))
			return false;

	return true;
}

bool ModalOutput::isBetterThanCandidate(const std::vector<double>& desiredEigenVector, const std::vector<double>& candidateEigenVector)
{
	if (abs(desiredEigenVector[desiredEigenVector.size() - 1]) >= abs(candidateEigenVector[candidateEigenVector.size() - 1]))
		return true;

	double sign2 = candidateEigenVector[0] > 0 ? 1 : -1;
	for (int i = 1; i < candidateEigenVector.size(); i++)
		if (candidateEigenVector[i] * sign2 < 0)
			return true;

	return false;
}