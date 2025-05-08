#include "StaticAnalysis.h"
#include "../OpenseesModel.h"

#include <algorithm>

using namespace opensees;

StaticAnalysis::StaticAnalysis(std::string sourceModelName, std::string analysisName, std::shared_ptr<Output> output, std::shared_ptr<LoadPattern> loadPattern) : Analysis(sourceModelName, analysisName, output)
{
	m_analysisType = opensees::AnalysisType::STATIC;
	m_loadPattern = loadPattern;
}

std::shared_ptr<LoadPattern> StaticAnalysis::getLoadPattern() const
{
	return m_loadPattern;
}

std::string StaticAnalysis::getOpenseesCommand() const
{
	std::string command;
	std::string workingDir = std::filesystem::current_path().string();

	command = "source " + workingDir + "/" + m_sourceModelName + ".tcl\n";
	command += "\n";

	command += "remove recorders\n";
	command += "\n";

	auto nodeTags = OpenseesModel::getInstance().getOutputNodes();
	auto minNodeTag = *std::min_element(nodeTags.begin(), nodeTags.end());
	auto maxNodeTag = *std::max_element(nodeTags.begin(), nodeTags.end());
	command += "#Node displacement recorder\n";
	command += "recorder Node -file " + workingDir + "/nodeDisp.out -time -nodeRange " + std::to_string(minNodeTag) + " " + std::to_string(maxNodeTag) + " -dof 1 2 3 4 5 6 disp";
	command += "\n";

	auto eleTags = OpenseesModel::getInstance().getOutputElements();
	auto minEleTag = (eleTags.begin())->first;
	auto maxEleTag = (--eleTags.end())->first;
	command += "#Element force recorder\n";
	command += "recorder Element -file " + workingDir + "/eleForce.out -time -eleRange " + std::to_string(minEleTag) + " " + std::to_string(maxEleTag) + " force";
	command += "\n";

	command += "#Load pattern\n";
	command += m_loadPattern->getOpenseesCommand();
	command += "\n";

	command += "#Analysis\n";
	command += "constraints Transformation\n";
	command += "numberer RCM\n";
	command += "system UmfPack\n";
	command += "test NormDispIncr 1.0e-6 6\n";
	command += "algorithm Linear\n";
	command += "integrator LoadControl 1\n";
	command += "analysis Static\n";
	command += "analyze 1\n";

	command += "wipe";

	return command;
}