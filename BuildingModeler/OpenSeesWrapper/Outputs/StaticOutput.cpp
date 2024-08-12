#include "StaticOutput.h"
#include "../OpenseesModel.h"

using namespace opensees;

StaticOutput::StaticOutput(std::string loadTag) : Output(loadTag)
{
	m_outputType = OutputType::STATIC;
}

const std::unordered_map<int, std::vector<std::vector<double>>>& StaticOutput::getElementForce() const
{
	return m_eleForceOutput;
}

void StaticOutput::retrieveOutput()
{
	// To do: these methods will be updated based on multiple stage analysis not just fsor analyze 1 command in OpenSees
	retrieveNodeDisplacements();
	retrieveElementForces();
}

void StaticOutput::retrieveNodeDisplacements()
{
	std::string outputFile = "nodeDisp.out";
	auto displacements = utilities::DataReader::readContinuosLine<double>(outputFile);

	auto nodeTags = OpenseesModel::getInstance().getOutputNodes();
	for (const auto tag : nodeTags) {
		m_nodeDispOutput[tag].resize(1);
	}

	for (int i = 1; i < displacements.size(); ++i) {
		
		int nodeInd = (i - 1) / 6;
		m_nodeDispOutput[nodeTags[nodeInd]][0].push_back(displacements[i]);
	}
}

void StaticOutput::retrieveElementForces()
{
	std::string outputFile = "eleForce.out";
	auto forces = utilities::DataReader::readContinuosLine<double>(outputFile);

	auto eleTags = OpenseesModel::getInstance().getOutputElements();
	for (const auto tag : eleTags) {
		m_eleForceOutput[tag.first].resize(1);
	}

	int forceInd = 1;
	for (auto it = eleTags.begin(); it != eleTags.end(); it++) {

		// To do: update here for zero length elements
		int numOfForces;
		if (it->second == opensees::ElementType::BEAMCOLUMN) {
			numOfForces = 12;
		}
		else {
			numOfForces = 24;
		}

		for (int i = 0; i < numOfForces; ++i) {
			m_eleForceOutput[it->first][0].push_back(forces[forceInd]);
			forceInd++;
		}
	}
}