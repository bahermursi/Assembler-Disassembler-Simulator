#ifndef __Mars__Disassembler__
#define __Mars__Disassembler__

#include "Instruction.h"
#include "Simulator.h"
#include "Memory.h"

#include <fstream>
#include <vector>
#include <string>

using namespace std;

class Disassembler{
    
public:
	Disassembler(string);
    void getData();
	void display(string, Simulator&);
    int getOpcode(int) const;
    int getSImm(int);
	void simulatorData(vector<Instruction>&);
private:
    char c;
    unsigned int instWord;
    ifstream inFile;
    string inFileName;
    vector<Instruction> inst;
    vector<unsigned int> instfile;
    vector<string>labels;
    unsigned int address;
    int jumpIndex;
    int branchIndex;
    Instruction x;
    int temp_rd;
    int temp_rt;
    bool isAddi;
    bool isSub;
    bool isSubu;
    int opcode;
    bool  pLi;
    bool pLi2;
    bool  printLi;
    bool printSubi;
    bool printSubi2;
    Simulator tempSimulator;
};
#endif
