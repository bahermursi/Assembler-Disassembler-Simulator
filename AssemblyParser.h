#ifndef ASSEMBLYPARSER_H
#define ASSEMBLYPARSER_H

#include "Memory.h"
#include "Instruction.h"

#include <QTextBrowser>

#include <string>
#include <map>
#include <vector>

using namespace std;

class AssemblyParser
{

private:

	string assemblyFile, textDumpFile, memoryDumpFile;

	map<string, unsigned int>labels;

    void getLabels(QTextBrowser *);

    void parseAssembly(Memory &, vector<Instruction> &, QTextBrowser *);

    bool isInstruction(const string &) const;

    int wordHexaToDecimal(const string &) const;
	
    short halfHexaToDecimal(const string &) const;

    char byteHexaToDecimal(const string &) const;

    void toChar(const string &, vector<char> &) const;

	char specialCharacter(char) const;

public:

    AssemblyParser(const string & assemblyCodeFile, const string & textDumpFile, const string & memoryDumpFile, Memory & memory, vector<Instruction> & instructionVector, QTextBrowser *);

};

#endif
