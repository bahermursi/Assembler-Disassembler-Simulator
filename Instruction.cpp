#include "Instruction.h"
#include "GLOBALS.h"

#include <string>
#include <stack>
#include <cmath>
#include <iostream>

using namespace std;

Instruction::Instruction()
{
}

void Instruction::assemble(const string & assemblyInstruction, const string & initialAssemblyInstruction)
{
    this->assemblyInstruction = assemblyInstruction;
    this->initialAssemblyInstruction = initialAssemblyInstruction;
    string instruction = "";
    int idx = 0;
    while (!isalpha(assemblyInstruction[idx]))
        idx++;
    while (assemblyInstruction[idx] != ' ' && assemblyInstruction[idx] != '\t' && assemblyInstruction[idx] != '\r' && idx < (int)(assemblyInstruction.length()))
    {
        instruction += tolower(assemblyInstruction[idx]);
        idx++;
    }
    binaryInstruction = "";
    if (instruction == "j" || instruction == "jal")
        setJFormat(instruction, idx);
    else if (instruction == "add" || instruction == "addu" || instruction == "sub" || instruction == "and" || instruction == "or" || instruction == "xor" || instruction == "slt" || instruction == "jr" || instruction == "sll" || instruction == "srl" || instruction == "syscall")
        setRFormat(instruction, idx);
    else if (instruction == "lui" || instruction == "beq" || instruction == "bne" || instruction == "lw" || instruction == "sw" || instruction == "lb" || instruction == "sb" || instruction == "lh" || instruction == "sh" || instruction == "addi" || instruction == "addiu" || instruction == "andi" || instruction == "ori" || instruction == "xori")
        setIFormat(instruction, idx);
}

void Instruction::dissassemble(unsigned int binaryInstruction, int trd, int trt, bool subi,bool subi2, bool li)
{
    opcode = binaryInstruction >> 26;
    if (opcode == 0)
    {
        func = binaryInstruction & 0x3F;
        shamt = (binaryInstruction >> 6) & 0x1f;
        rd = (binaryInstruction >> 11) & 0x1f;
        rt = (binaryInstruction >> 16) & 0x1f;
        rs = (binaryInstruction >> 21) & 0x1f;
    }
    else if (opcode != 0 && opcode != 2 && opcode != 3 && opcode != 16 && opcode != 17 && opcode != 18 && opcode != 19)
    {
        rt = (binaryInstruction >> 16) & 0x1f;
        rs = (binaryInstruction >> 21) & 0x1f;
        imm = (binaryInstruction & 0x0000FFFF);
        signedImm = (imm & 0x8000) ? (0xFFFF0000 | imm) : imm;
    }
    else if(opcode == 2 || opcode == 3)
    {
        address = (binaryInstruction & 0x3FFFFFF);
    }
    
    int temp_rd=trd;
    int temp_rt=trt;
    bool psubi=subi,pLi=li, psubi2 = subi2;
    setAssembleyInstruction(temp_rd ,temp_rt, psubi, psubi2, pLi);
}


void Instruction::setAssembleyInstruction(int tempd, int tempt,bool subi, bool isAddi, bool li)
{
    
    if (opcode == 0)
    {
        switch (func)
        {
            case 0x08:
                initialAssemblyInstruction = assemblyInstruction = "jr " + registerToName(rs);
                break;
            case 0x20:
                initialAssemblyInstruction = assemblyInstruction = "add " + registerToName(rd) + ", " + registerToName(rs) + ", " + registerToName(rt);
                break;
            case 0x21:
             initialAssemblyInstruction = assemblyInstruction = "addu " + registerToName(rd) + ", " + registerToName(rs) + ", " + registerToName(rt);
                if (rs == 0) {
                    assemblyInstruction = "move " + registerToName(rd) + ", " + registerToName(rt);
                }
                else
                   assemblyInstruction = "addu " + registerToName(rd) + ", " + registerToName(rs) + ", " + registerToName(rt);
                break;
            case 0x22:
             initialAssemblyInstruction = "sub " + registerToName(rd) + ", " + registerToName(rs) + ", " + registerToName(rt);
                //Re-test this part
                if (subi == 1)
                    assemblyInstruction = "subi " + registerToName(rd) + ", " + registerToName(rs) + ", " + to_string(addiImm);
                else
                     assemblyInstruction = "sub " + registerToName(rd) + ", " + registerToName(rs) + ", " + registerToName(rt);
                
                break;
            case 0x23:
                initialAssemblyInstruction = assemblyInstruction = "subu " + registerToName(rd) + ", " + registerToName(rs) + ", " + registerToName(rt);
                break;
            case 0x2A:
                initialAssemblyInstruction = assemblyInstruction = "slt " + registerToName(rd) + ", " + registerToName(rs) + ", " + registerToName(rt);
                break;
            case 0x2B:
                initialAssemblyInstruction = assemblyInstruction = "sltu " + registerToName(rd) + ", " + registerToName(rs) + ", " + registerToName(rt);
                break;
            case 0x00:
                initialAssemblyInstruction = assemblyInstruction = "sll " + registerToName(rd) + ", " + registerToName(rt) + ", " + to_string(shamt);
                break;
            case 0x02:
                initialAssemblyInstruction = assemblyInstruction = "srl " + registerToName(rd) + ", " + registerToName(rt) + ", " + to_string(shamt);
                break;
            case 0x03:
                initialAssemblyInstruction = assemblyInstruction = "sra " + registerToName(rd) + ", " + registerToName(rt) + ", " + to_string(shamt);
                break;
            case 0x24:
                initialAssemblyInstruction = assemblyInstruction = "and " + registerToName(rd) + ", " + registerToName(rs) + ", " + registerToName(rt);
                break;
            case 0x25:
                initialAssemblyInstruction = assemblyInstruction = "or " + registerToName(rd) + ", " + registerToName(rs) + ", " + registerToName(rt);
                break;
            case 0x26:
                initialAssemblyInstruction = assemblyInstruction = "xor " + registerToName(rd) + ", " + registerToName(rs) + ", " + registerToName(rt);
                break;
            case 0x27:
                initialAssemblyInstruction = assemblyInstruction = "nor " + registerToName(rd) + ", " + registerToName(rs) + ", " + registerToName(rt);
                break;
            case 0x0C:
                initialAssemblyInstruction = assemblyInstruction = "syscall";
                break;
            default:
                initialAssemblyInstruction = assemblyInstruction = "Unknown R-Format Instruction";
        }
    }
    else if (opcode != 0 && opcode != 2 && opcode != 3 && opcode != 16 && opcode != 17 && opcode != 18 && opcode != 19)
    {
        switch (opcode)
        {
        	
            case 0x08:
             initialAssemblyInstruction =  "addi " + registerToName(rt) + ", " + registerToName(rs) + ", " + to_string(signedImm);
                if (rs == 0 && isAddi == false)
                    assemblyInstruction = "li " + registerToName(rt) + ", " + to_string(signedImm);
                else //Check on different test cases
                    if (isAddi == 1) {
                        assemblyInstruction ="";
                    }
                    else
                      assemblyInstruction = "addi " + registerToName(rt) + ", " + registerToName(rs) + ", " + to_string(signedImm);
                addiImm = signedImm;
                break;
            case 0x09:
             initialAssemblyInstruction = = "addiu " + registerToName(rt) + ", " + registerToName(rs) + ", " + to_string(signedImm);
                if (rs == 0)
                     assemblyInstruction = "li " + registerToName(rt) + ", " + to_string(signedImm);
                else
                     assemblyInstruction = "addiu " + registerToName(rt) + ", " + registerToName(rs) + ", " + to_string(signedImm);
                break;
            case 0x0C:
                initialAssemblyInstruction = assemblyInstruction = "andi " + registerToName(rt) + ", " + registerToName(rs) + ", " + to_string(imm);
                break;
            case 0x23:
                initialAssemblyInstruction = assemblyInstruction =  "lw " + registerToName(rt) + ", " + to_string(signedImm) + "(" + registerToName(rs) + ")";
                break;
            case 0x2B:
                initialAssemblyInstruction = assemblyInstruction = "sw " + registerToName(rt) + ", " + to_string(signedImm) + "(" + registerToName(rs) + ")";
                break;
            case 0x20:
                initialAssemblyInstruction = assemblyInstruction = "lb " + registerToName(rt) + ", " + to_string(signedImm) + "(" + registerToName(rs) + ")";
                break;
            case 0x28:
                initialAssemblyInstruction = assemblyInstruction = "sb " + registerToName(rt) + ", " + to_string(signedImm) + "(" + registerToName(rs) + ")";
                break;
            case 0x21:
                initialAssemblyInstruction = assemblyInstruction = "lh " + registerToName(rt) + ", " + to_string(signedImm) + "(" + registerToName(rs) + ")";
                break;
            case 0x29:
                initialAssemblyInstruction = assemblyInstruction = "sh " + registerToName(rt) + ", " + to_string(signedImm) + "(" + registerToName(rs) + ")";
                break;
            case 0x0A:
                initialAssemblyInstruction = assemblyInstruction = "slti " + registerToName(rt) + ", " + registerToName(rs) + ", " + to_string(signedImm);
                break;
            case 0x0B:
                initialAssemblyInstruction = assemblyInstruction = "sltiu " + registerToName(rt) + ", " + registerToName(rs) + ", " + to_string(signedImm);
                break;
            case 0x0D:
             initialAssemblyInstruction  = "ori " + registerToName(rt) + ", " + to_string((liImm<<16) + imm);
                pseudo = "ori " + registerToName(rt) + ", " + registerToName(rs) + ", " + to_string(imm);
                if (li == false)
                    assemblyInstruction = pseudo;
                else
                    if(li == true)
                        assemblyInstruction = "li " + registerToName(rt) + ", " + to_string((liImm<<16) + imm);
                break;
            case 0x0E:
                initialAssemblyInstruction = assemblyInstruction = "xori " + registerToName(rt) + ", " + registerToName(rs) + ", " + to_string(imm);
                break;
            case 0x0F:
           initialAssemblyInstruction =  "lui " + registerToName(rt) + ", " + to_string(imm);
                pseudo = "lui " + registerToName(rt) + ", " + to_string(imm);
                if (li == false)
                    assemblyInstruction = pseudo;
                else
                    liImm = imm;
                 assemblyInstruction ="";
                break;
            case 0x04:
                initialAssemblyInstruction = assemblyInstruction = "beq " + registerToName(rs) + ", " + registerToName(rt) + ", " /*+ to_string(signedImm)*/;
                break;
            case 0x05:
                initialAssemblyInstruction = assemblyInstruction = "bne " + registerToName(rs) + ", " + registerToName(rt) + ", "/*+ to_string(signedImm)*/;
                break;
            default:
                initialAssemblyInstruction = assemblyInstruction = "Unknown I-Format Instruction";
                break;
        }
    }
    else if (opcode == 2 || opcode == 3)
    {
        switch (opcode)
        {
            case 0x02:
                initialAssemblyInstruction = assemblyInstruction = "j " ;
                break;
            case 0x03:
                initialAssemblyInstruction =  assemblyInstruction = "jal " ;
                break;
            default:
                initialAssemblyInstruction = assemblyInstruction = "Unknown J-Format Instruction" ;
                break;
        }
    }
}

string Instruction::registerToName(int reg) const
{
    string name;
    switch (reg) {
        case 0:
            name = "$zero";
            break;
        case 1:
            name = "$at";
        case 2:case 3:
            name = "$v" + to_string(abs(reg - 2));
            break;
        case 4: case 5: case 6: case 7:
            name = "$a" + to_string(reg - 4);
            break;
        case 8:case 9:case 10:  case 11: case 12:case 13:case 14:case 15:
            name = "$t" + to_string(reg - 8);
            break;
        case 16:case 17:case 18: case 19: case 20: case 21:case 22:case 23:
            name = "$s" + to_string(reg - 16);
            break;
        case 24: case 25:
            name = "$t" + to_string(reg - 16);
            break;
        case 26:case 27:
            name = "$k" + to_string(reg - 26);
            break;
        case 28:
            name = "$gp";
            break;
        case 29:
            name = "$sp";
            break;
        case 30:
            name = "$fp";
            break;
        case 31:
            name = "$ra";
            break;
            
    }
    return name;
}

string Instruction::toBinary(int n, int size, bool sign) const
{
    n = abs(n);
    stack<int>s;
    string str = "";
    while (n != 0)
    {
        s.push(n % 2);
        n /= 2;
    }
    while ((int)(s.size()) < size)
        s.push(0);
    while (!s.empty())
    {
        str += (s.top() + '0');
        s.pop();
    }
    if (!sign)
        str = twosCompliment(str, size);
    return str;
}

string Instruction::twosCompliment(string binary, int size) const
{
    string answer = "";
    for (int i = 0; i < size; i++)
        answer += '0';
    for (int i = 0; i < (int)(binary.length()); i++)
        binary[i] = (binary[i] == '0') ? '1' : '0';
    char carry = (binary[binary.length() - 1] == '1') ? '1' : '0';
    answer[answer.length() - 1] = (binary[binary.length() - 1] == '1') ? '0' : '1';
    for (int i = binary.length() - 2; i >= 0; i--)
    {
        if ((carry == '1' && binary[i] == '0') || (carry == '0' && binary[i] == '1'))
        {
            answer[i] = '1';
            carry = '0';
        }
        else if (carry == '1' && binary[i] == '1')
            carry = '1';
    }
    return answer;
}

int Instruction::convertToRegisterName(string name) const
{
    if (name == "$zero" || name == "$0")
        return 0;
    else if (name == "$at" || name == "$1")
        return 1;
    else if (name == "$v0" || name == "$2")
        return 2;
    else if (name == "$v1" || name == "$3")
        return 3;
    else if (name == "$a0" || name == "$4")
        return 4;
    else if (name == "$a1" || name == "$5")
        return 5;
    else if (name == "$a2" || name == "$6")
        return 6;
    else if (name == "$a3" || name == "$7")
        return 7;
    else if (name == "$t0" || name == "$8")
        return 8;
    else if (name == "$t1" || name == "$9")
        return 9;
    else if (name == "$t2" || name == "$10")
        return 10;
    else if (name == "$t3" || name == "$11")
        return 11;
    else if (name == "$t4" || name == "$12")
        return 12;
    else if (name == "$t5" || name == "$13")
        return 13;
    else if (name == "$t6" || name == "$14")
        return 14;
    else if (name == "$t7" || name == "$15")
        return 15;
    else if (name == "$s0" || name == "$16")
        return 16;
    else if (name == "$s1" || name == "$17")
        return 17;
    else if (name == "$s2" || name == "$18")
        return 18;
    else if (name == "$s3" || name == "$19")
        return 19;
    else if (name == "$s4" || name == "$20")
        return 20;
    else if (name == "$s5" || name == "$21")
        return 21;
    else if (name == "$s6" || name == "$22")
        return 22;
    else if (name == "$s7" || name == "$23")
        return 23;
    else if (name == "$t8" || name == "$24")
        return 24;
    else if (name == "$t9" || name == "$25")
        return 25;
    else if (name == "$k0" || name == "$26")
        return 26;
    else if (name == "$k1" || name == "$27")
        return 27;
    else if (name == "$gp" || name == "$28")
        return 28;
    else if (name == "$sp" || name == "$29")
        return 29;
    else if (name == "$fp" || name == "$30")
        return 30;
    else if (name == "$ra" || name == "$31")
        return 31;
    else
        return -1;
}

void Instruction::setJFormat(string instruction, int & idx)
{
    format = J_FORMAT;
    opcode = (instruction == "j") ? 2 : 3;
    while (!isdigit(assemblyInstruction[idx]))
        idx++;
    string temp = "";
    while (assemblyInstruction[idx] != ' ' && assemblyInstruction[idx] != '\t' && assemblyInstruction[idx] != '\r' && assemblyInstruction[idx] != '\n' && idx < (int)(assemblyInstruction.length()))
    {
        temp += assemblyInstruction[idx];
        idx++;
    }
    address = ((hexaToDecimal(temp, 8) & 0x0fffffff) >> 2);
    binaryInstruction += toBinary(opcode, 6, true);
    binaryInstruction += toBinary(address, 26, true);
}

void Instruction::setRFormat(string instruction, int & idx)
{
    format = R_FORMAT;
    opcode = 0;
    binaryInstruction += toBinary(opcode, 6, true);
    rs = 0;
    rt = 0;
    rd = 0;
    shamt = 0;
    if (instruction == "syscall")
        setSyscall();
    else if (instruction == "sll" || instruction == "srl")
        setShiftInstruction(instruction, idx);
    else if (instruction == "jr")
        setJrInstruction(instruction, idx);
    else
        setNormalRFormat(instruction, idx);
    binaryInstruction += toBinary(rs, 5, true);
    binaryInstruction += toBinary(rt, 5, true);
    binaryInstruction += toBinary(rd, 5, true);
    binaryInstruction += toBinary(shamt, 5, true);
    binaryInstruction += toBinary(func, 6, true);
}

void Instruction::setSyscall()
{
    func = 12;
}

void Instruction::setShiftInstruction(string instruction, int & idx)
{
    string temp = "";
    while (assemblyInstruction[idx] != '$')
        idx++;
	while (assemblyInstruction[idx] != ',' && assemblyInstruction[idx] != ' ' && assemblyInstruction[idx] != '\t' && assemblyInstruction[idx] != '\r')
    {
        temp += tolower(assemblyInstruction[idx]);
        idx++;
    }
    rd = convertToRegisterName(temp);
    temp = "";
    while (assemblyInstruction[idx] != '$')
        idx++;
    while (assemblyInstruction[idx] != ',' && assemblyInstruction[idx] != ' ' && assemblyInstruction[idx] != '\t' && assemblyInstruction[idx] != '\r')
    {
        temp += tolower(assemblyInstruction[idx]);
        idx++;
    }
    rt = convertToRegisterName(temp);
    temp = "";
    while (!isdigit(assemblyInstruction[idx]))
        idx++;
    while (assemblyInstruction[idx] != ' ' && assemblyInstruction[idx] != '\t' && assemblyInstruction[idx] != '\r' && assemblyInstruction[idx] != '\n' && idx < (int)(assemblyInstruction.length()))
    {
        temp += assemblyInstruction[idx];
        idx++;
    }
    if ((shamt = hexaToDecimal(temp, 2)) > 31)
        TERMINATE = true;
    if (instruction == "sll")
        func = 0;
    else
        func = 2;
}

void Instruction::setJrInstruction(string instruction, int & idx)
{
    string temp = "";
    while (assemblyInstruction[idx] != '$')
        idx++;
    while (assemblyInstruction[idx] != ' ' && assemblyInstruction[idx] != '\t' && assemblyInstruction[idx] != '\r' && assemblyInstruction[idx] != '\n' && idx < (int)(assemblyInstruction.length()))
    {
        temp += assemblyInstruction[idx];
        idx++;
    }
    rs = convertToRegisterName(temp);
    func = 8;
}

void Instruction::setNormalRFormat(string instruction, int & idx)
{
    string temp = "";
    while (assemblyInstruction[idx] != '$')
        idx++;
    while (assemblyInstruction[idx] != ',' && assemblyInstruction[idx] != ' ' && assemblyInstruction[idx] != '\t' && assemblyInstruction[idx] != '\r')
    {
        temp += tolower(assemblyInstruction[idx]);
        idx++;
    }
    rd = convertToRegisterName(temp);
    temp = "";
    while (assemblyInstruction[idx] != '$')
        idx++;
    while (assemblyInstruction[idx] != ',' && assemblyInstruction[idx] != ' ' && assemblyInstruction[idx] != '\t' && assemblyInstruction[idx] != '\r')
    {
        temp += tolower(assemblyInstruction[idx]);
        idx++;
    }
    rs = convertToRegisterName(temp);
    temp = "";
    while (assemblyInstruction[idx] != '$')
        idx++;
    while (assemblyInstruction[idx] != ' ' && assemblyInstruction[idx] != '\t' && assemblyInstruction[idx] != '\r' && assemblyInstruction[idx] != '\n' && idx < (int)(assemblyInstruction.length()))
    {
        temp += tolower(assemblyInstruction[idx]);
        idx++;
    }
    rt = convertToRegisterName(temp);
    if (instruction == "add")
        func = 32;
    else if (instruction == "addu")
        func = 33;
    else if (instruction == "sub")
        func = 34;
    else if (instruction == "and")
        func = 36;
    else if (instruction == "or")
        func = 37;
    else if (instruction == "xor")
        func = 38;
    else if (instruction == "slt")
        func = 42;
}

void Instruction::setIFormat(string instruction, int & idx)
{
    format = I_FORMAT;
    rs = 0;
    rt = 0;
    imm = 0;
    if (instruction == "lui")
        setLui(idx);
    else if (instruction == "beq" || instruction == "bne")
        setBranchInstruction(instruction, idx);
    else if (instruction == "lw" || instruction == "sw" || instruction == "lb" || instruction == "sb" || instruction == "lh" || instruction == "sh")
        setMemoryInstruction(instruction, idx);
    else
        setNormalIFormat(instruction, idx);
    binaryInstruction += toBinary(opcode, 6, true);
    binaryInstruction += toBinary(rs, 5, true);
    binaryInstruction += toBinary(rt, 5, true);
    binaryInstruction += (imm < 0) ? toBinary(imm, 16, false) : toBinary(imm, 16, true);
}

void Instruction::setLui(int & idx)
{
    opcode = 15;
    string temp = "";
    while (assemblyInstruction[idx] != '$')
        idx++;
    while (assemblyInstruction[idx] != ',' && assemblyInstruction[idx] != ' ' && assemblyInstruction[idx] != '\t' && assemblyInstruction[idx] != '\r')
    {
        temp += tolower(assemblyInstruction[idx]);
        idx++;
    }
    rt = convertToRegisterName(temp);
    temp = "";
    while (!isdigit(assemblyInstruction[idx]) && assemblyInstruction[idx] != '-')
        idx++;
    while (assemblyInstruction[idx] != ' ' && assemblyInstruction[idx] != '\t' && assemblyInstruction[idx] != '\r' && assemblyInstruction[idx] != '\n' && idx < (int)(assemblyInstruction.length()))
    {
        temp += assemblyInstruction[idx];
        idx++;
    }
    signedImm = hexaToDecimal(temp, 4);
    imm = signedImm & 0x0000ffff;
}

void Instruction::setBranchInstruction(string instruction, int & idx)
{
    opcode = (instruction == "beq") ? 4 : 5;
    string temp = "";
    while (assemblyInstruction[idx] != '$')
        idx++;
    while (assemblyInstruction[idx] != ',' && assemblyInstruction[idx] != ' ' && assemblyInstruction[idx] != '\t' && assemblyInstruction[idx] != '\r')
    {
        temp += tolower(assemblyInstruction[idx]);
        idx++;
    }
    rs = convertToRegisterName(temp);
    temp = "";
    while (assemblyInstruction[idx] != '$')
        idx++;
    while (assemblyInstruction[idx] != ',' && assemblyInstruction[idx] != ' ' && assemblyInstruction[idx] != '\t' && assemblyInstruction[idx] != '\r')
    {
        temp += tolower(assemblyInstruction[idx]);
        idx++;
    }
    rt = convertToRegisterName(temp);
    temp = "";
    while (!isdigit(assemblyInstruction[idx]) && assemblyInstruction[idx] != '-')
        idx++;
    while (assemblyInstruction[idx] != ' ' && assemblyInstruction[idx] != '\t' && assemblyInstruction[idx] != '\r' && assemblyInstruction[idx] != '\n' && idx < (int)(assemblyInstruction.length()))
    {
        temp += assemblyInstruction[idx];
        idx++;
    }
    signedImm = hexaToDecimal(temp, 4);
    imm = signedImm & 0x0000ffff;
}

void Instruction::setMemoryInstruction(string instruction, int & idx)
{
    opcode = (instruction == "lw") ? 35 : (instruction == "sw") ? 43 : (instruction == "lh") ? 33 : (instruction == "sh") ? 41 : (instruction == "lb") ? 32 : 40;
    string temp = "";
    while (assemblyInstruction[idx] != '$')
        idx++;
    while (assemblyInstruction[idx] != ',' && assemblyInstruction[idx] != ' ' && assemblyInstruction[idx] != '\t' && assemblyInstruction[idx] != '\r')
    {
        temp += tolower(assemblyInstruction[idx]);
        idx++;
    }
    rt = convertToRegisterName(temp);
    temp = "";
    while (!isdigit(assemblyInstruction[idx]) && assemblyInstruction[idx] != '-')
        idx++;
    while (assemblyInstruction[idx] != '(' && assemblyInstruction[idx] != ' ' && assemblyInstruction[idx] != '\t' && assemblyInstruction[idx] != '\r')
    {
        temp += tolower(assemblyInstruction[idx]);
        idx++;
    }
    signedImm = hexaToDecimal(temp, 4);
    imm = signedImm & 0x0000ffff;
    temp = "";
    while (assemblyInstruction[idx] != '$')
        idx++;
    while (assemblyInstruction[idx] != ')' && assemblyInstruction[idx] != ' ' && assemblyInstruction[idx] != '\t' && assemblyInstruction[idx] != '\r')
    {
        temp += tolower(assemblyInstruction[idx]);
        idx++;
    }
    rs = convertToRegisterName(temp);
}

void Instruction::setNormalIFormat(string instruction, int & idx)
{
    opcode = (instruction == "addi") ? 8 : (instruction == "addiu") ? 9 : (instruction == "andi") ? 12 : (instruction == "ori") ? 13 : (instruction == "xori") ? 14 : 10;
    string temp = "";
    while (assemblyInstruction[idx] != '$')
        idx++;
    while (assemblyInstruction[idx] != ',' && assemblyInstruction[idx] != ' ' && assemblyInstruction[idx] != '\t' && assemblyInstruction[idx] != '\r')
    {
        temp += tolower(assemblyInstruction[idx]);
        idx++;
    }
    rt = convertToRegisterName(temp);
    temp = "";
    while (assemblyInstruction[idx] != '$')
        idx++;
    while (assemblyInstruction[idx] != ',' && assemblyInstruction[idx] != ' ' && assemblyInstruction[idx] != '\t' && assemblyInstruction[idx] != '\r')
    {
        temp += tolower(assemblyInstruction[idx]);
        idx++;
    }
    rs = convertToRegisterName(temp);
    temp = "";
    while (!isdigit(assemblyInstruction[idx]) && assemblyInstruction[idx] != '-')
        idx++;
    while (assemblyInstruction[idx] != ' ' && assemblyInstruction[idx] != '\t' && assemblyInstruction[idx] != '\r' && assemblyInstruction[idx] != '\n' && idx < (int)(assemblyInstruction.length()))
    {
        temp += tolower(assemblyInstruction[idx]);
        idx++;
    }
    signedImm = hexaToDecimal(temp, 4);
    imm = signedImm & 0x0000ffff;
}

Instruction::InstructionFormat Instruction::getFormat() const
{
    return format;
}

int Instruction::getOpcode() const
{
    return opcode;
}

int Instruction::getRs() const
{
    return rs;
}

int Instruction::getRt() const
{
    return rt;
}

int Instruction::getRd() const
{
    return rd;
}

int Instruction::getShamt() const
{
    return shamt;
}

int Instruction::getFunc() const
{
    return func;
}

int Instruction::getImm() const
{
    return imm;
}

int Instruction::getSignedImm() const
{
    return signedImm;
}

int Instruction::getAddress() const
{
    return address;
}

string Instruction::getAssemblyInstruction() const
{
    return assemblyInstruction;
}

string Instruction::getInitialAssemblyInstruction() const
{
    return initialAssemblyInstruction;
}

string Instruction::getBinaryInstruction() const
{
    return binaryInstruction;
}

int Instruction::hexaToDecimal(const string & hexa, int n)
{
    if (hexa.length() > 2)
    {
        if (hexa[1] == 'x')
        {
            int ans = 0;
            int i = hexa.length() - 1;
            while (i >= 0 && n > 0)
            {
                if (hexa[i] >= '0' && hexa[i] <= '9')
                    ans += ((hexa[i] - '0') << (4 * (hexa.length() - 1 - i)));
                else if (hexa[i] >= 'a' && hexa[i] <= 'f')
                    ans += ((hexa[i] - 'a' + 10) << (4 * (hexa.length() - 1 - i)));
                else if (hexa[i] >= 'A' && hexa[i] <= 'F')
                    ans += ((hexa[i] - 'A' + 10) << (4 * (hexa.length() - 1 - i)));
                i--;
                n--;
            }
            return ans;
        }
        else
        {
            int y = atoi(hexa.c_str());
            int z;
            z = (y << (n * 4));
            z = (z >> (n * 4));
            return z;
        }
    }
    else
    {
        int y = atoi(hexa.c_str());
        int z;
        z = (y << (n * 4));
        z = (z >> (n * 4));
        return z;
    }
}
