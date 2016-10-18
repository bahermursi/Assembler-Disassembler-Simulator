#include "Simulator.h"
#include "GLOBALS.h"

#include <QString>

#include <iostream>
#include <iomanip>

using namespace std;

Simulator::Simulator(){}

Simulator:: ~Simulator(){}

void Simulator::add(Instruction* instruction, QTextBrowser *textBrowser)
{
    cpu.registers[instruction->getRd()] = cpu.registers[instruction->getRs()] + cpu.registers[instruction->getRt()];
    if((cpu.registers[instruction->getRs()] < 0 && cpu.registers[instruction->getRt()] < 0 && cpu.registers[instruction->getRd()] > 0 ) || (cpu.registers[instruction->getRs()] > 0 && cpu.registers[instruction->getRt()] > 0 && cpu.registers[instruction->getRd()] < 0 ))
    {
        textBrowser->append("Overflow.\n");
        TERMINATE = true;
    }
}

void Simulator::addu(Instruction* instruction)
{
    cpu.registers[instruction->getRd()] = cpu.registers[instruction->getRs()] + cpu.registers[instruction->getRt()];
}

void Simulator::addi(Instruction* instruction)
{
    // R[rt] = R[rs] + SignExtImm
    cpu.registers[instruction->getRt()] = cpu.registers[instruction->getRs()] + instruction->getSignedImm();
}

void Simulator:: addiu(Instruction* instruction)
{
    // change sign extension R[rt] = R[rs] + SignExtImm
    
    cpu.registers[instruction->getRt()] = cpu.registers[instruction->getRs()] + instruction->getSignedImm();
                      
}

void Simulator::sub(Instruction* instruction)
{
    //R[rd] = R[rs] - R[rt]
    cpu.registers[instruction->getRd()] = cpu.registers[instruction->getRs()] - cpu.registers[instruction->getRt()];
}

void Simulator:: oring(Instruction* instruction)
{
    //R[rd] = R[rs] | R[rt]
    cpu.registers[instruction->getRd()]= cpu.registers[instruction->getRs()] | cpu.registers[instruction->getRt()];
}

void Simulator:: ori(Instruction* instruction)
{
    // missing zero extension immediate  R[rd] = R[rs] | R[rt]
    cpu.registers[instruction->getRt()]= cpu.registers[instruction->getRs()] | instruction->getImm();
}
void Simulator:: sll(Instruction* instruction)
{
    //R[rd] = R[rt] << shamt
    cpu.registers[instruction->getRd()] = cpu.registers[instruction->getRt()] << instruction->getShamt();
}

void Simulator:: srl(Instruction* instruction)
{
    //R[rd] = R[rt] >> shamt
    cpu.registers[instruction->getRd()] = (unsigned int)(cpu.registers[instruction->getRt()]) >> instruction->getShamt();
}

void Simulator:: anding(Instruction* instruction)
{
    //R[rd] = R[rs] & R[rt]
    cpu.registers[instruction->getRd()] = cpu.registers[instruction->getRs()] & cpu.registers[instruction->getRt()];
    
}
void Simulator:: andi(Instruction* instruction)
{
    //R[rt] = R[rs] & ZeroExtImm
    cpu.registers[instruction->getRt()] = cpu.registers[instruction->getRs()] & instruction->getImm();
    
}

void Simulator:: xori(Instruction* instruction)
{
    cpu.registers[instruction->getRt()] = cpu.registers[instruction->getRs()] ^ instruction->getImm();
}
void Simulator:: xoring(Instruction* instruction)
{
    cpu.registers[instruction->getRd()] = cpu.registers[instruction->getRs()] ^ cpu.registers[instruction->getRt()];
}

void Simulator:: lw(Instruction* instruction)
{
    //R[rt] = M[R[rs]+SignExtImm]
    int address = cpu.registers[instruction->getRs()] + instruction->getSignedImm();
    cpu.registers[instruction->getRt()]=memory.loadWord(address);
}

void Simulator:: sw(Instruction* instruction)
{
    //M[R[rs]+SignExtImm] = R[rt]
    int address = cpu.registers[instruction->getRs()] + instruction->getSignedImm();
    memory.storeWord(address,cpu.registers[instruction->getRt()]);
    
}
void Simulator::sb(Instruction* instruction)
{
    //M[R[rs]+SignExtImm](7:0) =R[rt](7:0)
    int address = cpu.registers[instruction->getRs()] + instruction->getSignedImm();
   memory.storeByte(address, cpu.registers[instruction->getRt()]);
    
}
void Simulator::lb(Instruction* instruction)
{
    int address = cpu.registers[instruction->getRs()] + instruction->getSignedImm();
    cpu.registers[instruction->getRt()]=memory.loadByte(address);
}
void Simulator::lh(Instruction* instruction)
{
    int address = cpu.registers[instruction->getRs()] + instruction->getSignedImm();
    cpu.registers[instruction->getRt()]=memory.loadHalf(address);
}
void Simulator::sh(Instruction* instruction)
{
    //M[R[rs]+SignExtImm](15:0) =R[rt](15:0)
    int address = cpu.registers[instruction->getRs()] + instruction->getSignedImm();
    memory.storeHalf(address, cpu.registers[instruction->getRt()]);
}
void Simulator::beq(Instruction* instruction)
{
    //if(R[rs]==R[rt]) ,PC=PC+4+BranchAddr
    if(cpu.registers[instruction->getRs()]==cpu.registers[instruction->getRt()])
        cpu.programCounter=cpu.programCounter+(instruction->getSignedImm()*4);
    
}
void Simulator::bne(Instruction* instruction)
{
     //if(R[rs]!=R[rt]) ,PC=PC+4+BranchAddr
    if(cpu.registers[instruction->getRs()]!=cpu.registers[instruction->getRt()])
        cpu.programCounter=cpu.programCounter+(instruction->getSignedImm()*4);
    
}
void Simulator::slt(Instruction* instruction)
{
    //R[rd] = (R[rs] < R[rt]) ? 1 : 0
    if(cpu.registers[instruction->getRs()] < cpu.registers[instruction->getRt()])
        cpu.registers[instruction->getRd()]=1;
    else
        cpu.registers[instruction->getRd()]=0;
}
void Simulator::slti(Instruction* instruction)
{
    ////R[rd] = (R[rs] < SignExtImm) ? 1 : 0
    if(cpu.registers[instruction->getRs()] < instruction->getSignedImm())
        cpu.registers[instruction->getRd()]=1;
    else
        cpu.registers[instruction->getRd()]=0;
    
}
void Simulator::j(Instruction* instruction)
{
    //PC=JumpAddr
    cpu.programCounter = (cpu.programCounter & 0xf0000000)|(instruction->getAddress() << 2);
}
void Simulator::jr(Instruction* instruction)
{
    //PC=R[rs]
    cpu.programCounter=cpu.registers[instruction->getRs()];
    
}

void Simulator::jal(Instruction* instruction)
{
    //R[31]=PC+8;PC=JumpAddr
    
    cpu.registers[31]=cpu.programCounter;
    cpu.programCounter=(cpu.programCounter & 0xf0000000)|(instruction->getAddress() << 2);
    
}

void Simulator::lui(Instruction* instruction)
{
    //R[rt] = {imm, 16’b0}
    cpu.registers[instruction->getRt()]=(instruction->getImm()<<16);
}

void Simulator::syscall(QTextBrowser *textBrowser)
{
	int address = cpu.registers[4];
	char t;
    QString text;
	switch (cpu.registers[2])
	{
		//print integer
		case 1:
            text = QString::number(cpu.registers[4]);
            textBrowser->insertPlainText(text);
            textBrowser->moveCursor(QTextCursor::End);
			break;
		// print character
		case 11:
            text = "";
            text += (char)(cpu.registers[4] & 0xff);
            textBrowser->insertPlainText(text);
            textBrowser->moveCursor(QTextCursor::End);
			break;
		//print string
		case 4:
			while((t = memory.loadByte(address)) != 0)
			{
                text = "";
                text += t;
                textBrowser->insertPlainText(text);
				address++;
			}
			break;
		//terminate execution
		case 10:
			TERMINATE = true;
			break;
	}
}

void Simulator::run(Instruction *instruction, QTextBrowser *textBrowser)
{
	if (instruction->getOpcode() == 0)
	{
		switch (instruction->getFunc())
		{
		case 0x08:
			jr(instruction);
			break;
		case 0x20:
            add(instruction, textBrowser);
			break;
		case 0x21:
			addu(instruction);
			break;
		case 0x22:
			sub(instruction);
			break;
		case 0x2A:
			slt(instruction);
			break;
		case 0x00:
			sll(instruction);
			break;
		case 0x02:
			srl(instruction);
			break;
		case 0x24:
			anding(instruction);
			break;
		case 0x25:
			oring(instruction);
			break;
		case 0x26:
			xoring(instruction);
			break;
		case 0x0C:
            syscall(textBrowser);
			break;
		default:
            cerr << "Error in extracting R- Format in simulator.\n";
		}
	}
	else if (instruction->getOpcode() != 0 && instruction->getOpcode() != 2 && instruction->getOpcode() != 3)
	{
		switch (instruction->getOpcode())
		{
		case 0x08:
			addi(instruction);
			break;
		case 0x09:
			addiu(instruction);
			break;
		case 0x0C:
			andi(instruction);
			break;
		case 0x23:
			lw(instruction);
			break;
		case 0x2B:
			sw(instruction);
			break;
		case 0x20:
			lb(instruction);
			break;
		case 0x28:
			sb(instruction);
			break;
		case 0x21:
			lh(instruction);
			break;
		case 0x29:
			sh(instruction);
			break;
		case 0x0A:
			slti(instruction);
			break;
		case 0x0D:
			ori(instruction);
			break;
		case 0x0E:
			xori(instruction);
			break;
		case 0x0F:
			lui(instruction);
			break;
		case 0x04:
			beq(instruction);
			break;
		case 0x05:
			bne(instruction);
			break;
		default:
            cerr << "Error in extracting I- Format in simulator.\n";
			break;
		}
	}
	else if (instruction->getOpcode() == 2 || instruction->getOpcode() == 3)
	{
		switch (instruction->getOpcode())
		{
		case 0x02:
			j(instruction);
			break;
		case 0x03:
			jal(instruction);
			break;
		default:
            cerr << "Error in extracting J- Format in simulator.\n";
			break;
		}
	}

}

void Simulator::simulate(const string & registerUpdateFile, QTextBrowser *textBrowser)
{
    outfile.open(registerUpdateFile.c_str());
    outfile << "Initial State:\n";
    displayRegister(textBrowser);
    int i;
    do
    {
        i=(cpu.programCounter-0x00400000)/4;
        cpu.programCounter+=4;
        if (i>=(int)(program.size()))
            break;
        run(&program[i], textBrowser);
		cpu.registers[0] = 0;
        outfile << program[i].getInitialAssemblyInstruction() << '\n';
        displayRegister(textBrowser);
    } while(!TERMINATE);
}

string Simulator::initializeName(int i)
{
   
    switch(i)
    {
        case 0:
            registerName[0]="$zero";
            break;
        case 1:
            registerName[1]="$at";
            break;
        case 2:
            registerName[2]="$v0";
            break;
        case 3:
            registerName[3]="$v1";
            break;
        case 4:
            registerName[4]="$a0";
            break;
        case 5:
            registerName[5]="$a1";
            break;
        case 6:
            registerName[6]="$a2";
            break;
        case 7:
            registerName[7]="$a3";
            break;
        case 8:
            registerName[8]="$t0";
            break;
        case 9:
            registerName[9]="$t1";
            break;
        case 10:
            registerName[10]="$t2";
            break;
        case 11:
            registerName[11]="$t3";
            break;
        case 12:
            registerName[12]="$t4";
            break;
        case 13:
            registerName[13]="$t5";
            break;
        case 14:
            registerName[14]="$t6";
            break;
        case 15:
            registerName[15]="$t7";
            break;
        case 16:
            registerName[16]="$s0";
            break;
        case 17:
            registerName[17]="$s1";
            break;
        case 18:
            registerName[18]="$s2";
            break;
        case 19:
            registerName[19]="$s3";
            break;
        case 20:
            registerName[20]="$s4";
            break;
        case 21:
            registerName[21]="$s5";
            break;
        case 22:
            registerName[22]="$s6";
            break;
        case 23:
            registerName[23]="$s7";
            break;
        case 24:
            registerName[24]="$t8";
            break;
        case 25:
            registerName[25]="$t9";
            break;
        case 26:
            registerName[26]="$k0";
            break;
        case 27:
            registerName[27]="$k1";
            break;
        case 28:
            registerName[28]="$gp";
            break;
        case 29:
            registerName[29]="$sp";
            break;
        case 30:
            registerName[30]="$fp";
            break;
        case 31:
            registerName[31]="$ra";
            break;
    }
    return registerName[i];
}

void Simulator::displayRegister(QTextBrowser *textBrowser)
{
    if(outfile.is_open())
    {
        outfile<<"Name"<<setw(35)<<"Number"<<setw(35)<<"Value"<<endl;
        for(int i=0; i<32; i++)
            outfile<< initializeName(i)<<setw(35)<<i<<setw(35)<<cpu.registers[i]<<endl;
        outfile << '\n';
    }
    else
        textBrowser->append("Error in opening output file.\n");
}
