#include "ModalAnalysis.h"

using namespace opensees;

ModalAnalysis::ModalAnalysis(std::string sourceModelName, std::string analysisName, std::shared_ptr<Output> output, size_t numberOfModes, std::vector<int> masterNodeTags) : Analysis(sourceModelName, analysisName, output)
{
	m_analysisType = opensees::AnalysisType::MODAL;
	m_numberOfModes = numberOfModes;
	m_masterNodeTags = masterNodeTags;
}

size_t ModalAnalysis::getNumberOfModes() const
{
	return m_numberOfModes;
}

const std::vector<int>& ModalAnalysis::getMasterNodeTags() const
{
	return m_masterNodeTags;
}

std::string ModalAnalysis::getOpenseesCommand() const
{
	std::string command;

	command = "source " + m_sourceModelName + ".tcl\n";
	command += "\n";

	command += "remove recorders\n";
	command += "\n";

	command += "#Eigenvector recorder for dof 1\n";
	command += "for { set k 1 } { $k <= " + std::to_string(m_numberOfModes) + " } { incr k } {\n";
	command += "\trecorder Node -file[format \"mode1%i.out\" $k] -node ";
	for (const auto& nodeTag : m_masterNodeTags) {
		command += (std::to_string(nodeTag) + " ");
	}
	command += "-dof 1 \"eigen $k\"\n";
	command += "}\n";
	command += "\n";

	command += "#Eigenvector recorder for dof 2\n";
	command += "for { set k 1 } { $k <= " + std::to_string(m_numberOfModes) + " } { incr k } {\n";
	command += "\trecorder Node -file[format \"mode2%i.out\" $k] -node ";
	for (const auto& nodeTag : m_masterNodeTags) {
		command += (std::to_string(nodeTag) + " ");
	}
	command += "-dof 2 \"eigen $k\"\n";
	command += "}\n";
	command += "\n";

	command += "#Eigenvector recorder for dof 6\n";
	command += "for { set k 1 } { $k <= " + std::to_string(m_numberOfModes) + " } { incr k } {\n";
	command += "\trecorder Node -file[format \"mode6%i.out\" $k] -node ";
	for (const auto& nodeTag : m_masterNodeTags) {
		command += (std::to_string(nodeTag) + " ");
	}
	command += "-dof 6 \"eigen $k\"\n";
	command += "}\n";
	command += "\n";

	command += "#Analysis\n";
	command += "set lambdaVal [eigen " + std::to_string(m_numberOfModes) + "]\n";
	command += "\n";

	command += "#Storing Lambda Values\n";
	command += "set lambda \"Lambda.txt\"\n";
	command += "set lambdaFile [open $lambda \"w\"]\n";
	command += "foreach l $lambdaVal {\n";
	command += "\tputs $lambdaFile \"$l\"\n";
	command += "}\n";
	command += "close $lambdaFile\n";

	command += "\n";
	command += "record\n";
	command += "wipe";

	return command;
}