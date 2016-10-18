#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <string>

using namespace std;

class Instruction
{
    
public:
    Instruction();
    void assemble(const string &, const string &);
    void dissassemble(unsigned int, int, int,bool, bool, bool);
    enum InstructionFormat { R_FORMAT, I_FORMAT, J_FORMAT };
    InstructionFormat getFormat() const;
    int getOpcode() const;
    int getRs() const;
    int getRt() const;
    int getRd() const;
    int getShamt() const;
    int getFunc() const;
    int getImm() const;
    int getSignedImm() const;
    int getAddress() const;
    string getInitialAssemblyInstruction() const;
    string getAssemblyInstruction() const;
    string getBinaryInstruction() const;
private:
    string initialAssemblyInstruction;
    string assemblyInstruction;
    string binaryInstruction;
    string pseudo;
    InstructionFormat format;
    int opcode;
    int rs;
    int rt;
    int rd;
    int shamt;
    int func;
    int imm;
    int liImm;
    int signedImm;
    int addiImm;
    int address;
    void setAssembleyInstruction(int,int,bool,bool,bool);
    string registerToName(int) const;
    string toBinary(int, int, bool) const;
    string twosCompliment(string, int) const;
    int convertToRegisterName(string) const;
    void setJFormat(string, int &);
    void setRFormat(string, int &);
    void setSyscall();
    void setShiftInstruction(string, int &);
    void setJrInstruction(string, int &);
    void setNormalRFormat(string, int &);
    void setIFormat(string, int &);
    void setLui(int &);
    void setBranchInstruction(string, int &);
    void setMemoryInstruction(string, int &);
    void setNormalIFormat(string, int &);
    int hexaToDecimal(const string &, int);
};

#endif
