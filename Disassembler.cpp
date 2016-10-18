#include "Disassembler.h"

#include <iostream>
#include <fstream>

using namespace std;

Disassembler::Disassembler(string x)
{
    inFileName =x;
    getData();
}

void Disassembler::getData()
{
    inFile.open(inFileName.c_str(), ios_base::in | ios_base::binary);
    
    // If the file is open (this will evaluate to false if the file could not be found)
    if(inFile.is_open())
    {   
        while(!inFile.eof())
        {
            instWord = 0;
            // Decode the instruction read from the file
            for (int i = 0; i < 4; i++)
            {
                if (!inFile.eof())
                {
                    inFile.get(c);
                    unsigned int temp = (unsigned int)((unsigned char)c);
                    instWord = instWord | (temp << (8 * i));
                }
                else
                    break;
            }
            if (!inFile.eof())
            {
                // instfile vector that stores all the binary instructions
                instfile.push_back(instWord);                
            }
        }   
    }
    else
        cerr << "Error in opening disassembler file.\n";
    
    for (int i = 0 ; i < (int)(instfile.size()); i++)
	{    
        if (i<(int)(instfile.size())-1)
		{
			isSub = ((instfile[i] >> 26 == 0) && ((instfile[i + 1] & 0x3F) == 0x22) && (instfile[i - 1] >> 26 == 0x08));
			isAddi = ((instfile[i + 1] >> 26 == 0) && ((instfile[i + 1] & 0x3F) == 0x22)) && (instfile[i] >> 26 == 0x08);
			temp_rd = (instfile[i + 1] >> 11) & 0x1f;
			temp_rt = (instfile[i + 1] >> 21) & 0x1f;
			pLi = ((instfile[i] >> 26 == 0x0F) && instfile[i + 1] >> 26 == 0x0D);
		}
		else
			pLi=false;
		if (i>0)
			pLi2 = ((instfile[i - 1] >> 26 == 0x0F) && (instfile[i] >> 26 == 0x0D));
		else
			pLi2 = false;
		printLi = (pLi || pLi2);
		printSubi = isSub;

		x.dissassemble(instfile[i],temp_rd,temp_rt,printSubi,isAddi,printLi); // calling the disassebling process
		inst.push_back(x);   
    }
}

void Disassembler::display(string path, Simulator& simulator)
{
    Memory memory;
    ofstream outfile;
    outfile.open(path.c_str());
    if (outfile.is_open()) {
        //set vector label to the size of the instructions
        labels.resize(instfile.size() + 1);
		
		int counter = 0; //used in assigning numbers to the labels
        
        for (int i = 0; i < (int)(instfile.size()); i++)
        {
            address =  (instfile[i] & 0x3FFFFFF) << 2; // extracting the address
            jumpIndex= (address - 0x00400000)/4; //the index of the label
            branchIndex = i + 1 + getSImm(i);
            
            //if j or jal, store the labels based on it's address
            if ( (instfile[i] >> 26) == 2 || (instfile[i] >> 26) == 3 )
            {
                //check if the inserted label wasn't inserted by the branch instruction, to avoid duplication
                if (labels[jumpIndex] == "")
                    labels[jumpIndex]= "label_"+ to_string(counter++) + ": ";
            }
            else
                if ( (instfile[i] >> 26) == 0x04 || (instfile[i] >> 26) == 0x05 )
                    labels[branchIndex] = "label_"+ to_string(counter++) + ": ";
        }
        outfile << ".data\n.word ";
        for (int addr = 0x10010000; addr < 0x10010000 + 8192; addr += 4)
        {
            outfile << "0x";
            outfile << hex << simulator.memory.loadWord(addr) << ", ";
        }
        outfile << '\n';
        
        outfile<<".text"<<endl;
        
        for (int i = 0 ; i < (int)(labels.size()); i++)
		{    
            //Printing labels
            if (labels[i] !="" )
                outfile <<endl<<labels[i] << "          " << endl;
            
            if (i < (int)(inst.size()))
			{
				if (inst[i].getAssemblyInstruction() != "")
				{
					// printing j and jal with label
					if (inst[i].getOpcode() == 2 || inst[i].getOpcode() == 3)
					{
						jumpIndex = ((inst[i].getAddress() << 2) - 0x00400000) / 4;
						long len = labels[jumpIndex].length() - 1;
						string jString = labels[jumpIndex];
						outfile << endl << "          " << inst[i].getAssemblyInstruction() << jString.erase(abs(len - 1)) << endl << endl; //erase is used to erase the ":"
					}
					else
					if (inst[i].getOpcode() == 0x04 || inst[i].getOpcode() == 0x05)
					{
						//printing beq and bne with label
						branchIndex = i + 1 + getSImm(i);
						long len1 = labels[branchIndex].length() - 1;
						string brString = labels[branchIndex];
						outfile << "          " << inst[i].getAssemblyInstruction() << brString.erase(len1 - 1) << endl;
					}
					else
						outfile << "          " << inst[i].getAssemblyInstruction() << endl;
				}
			}
        }
    }
    else
        cerr <<"Error in opening disassembler output file.\n";
}


int Disassembler:: getSImm(int i)
{
    int imm = (instfile[i] & 0xFFFF);
    return ((imm & 0x8000) ? (0xFFFF0000 | imm) : imm);
}

void Disassembler::simulatorData(vector<Instruction>& x)
{
    x = inst;
}
