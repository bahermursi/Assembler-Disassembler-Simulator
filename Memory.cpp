#include "Memory.h"
#include "GLOBALS.h"

#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

Memory::Memory()
{
	currAddress = 0;
	memset(memory, 0, sizeof memory);
}

void Memory::getPointers(QTextBrowser *)
{
    this->textBrowser = textBrowser;
}

char Memory::loadByte(unsigned int addr)
{
    addr -= 0x10010000;
    return memory[addr];
}

int Memory::loadWord(unsigned int addr)
{
    addr -= 0x10010000;
    return ((unsigned char)(memory[addr + 3]) << 24) | ((unsigned char)(memory[addr + 2]) << 16) | ((unsigned char)(memory[addr + 1]) << 8) | (unsigned char)(memory[addr]);
}

short Memory::loadHalf(unsigned int addr)
{
    addr -= 0x10010000;
    return ((unsigned char)(memory[addr + 1]) << 8) | (unsigned char)(memory[addr]);
}

void Memory::storeByte(unsigned int addr, char val)
{
    addr -= 0x10010000;
    memory[addr] = val;
}

void Memory::storeHalf(unsigned int addr, short val)
{
	if (addr % 2 != 0)
	{
        textBrowser->append("Error in memory store half.\n");
		TERMINATE = true;
	}
	else
	{
		addr -= 0x10010000;
		memory[addr] = val & 0xff;
		memory[addr + 1] = val >> 8;
	}
}

void Memory::storeWord(unsigned int addr, int val)
{
	if (addr % 4 != 0)
	{
        textBrowser->append("Error in memory store word.\n");
		TERMINATE = true;
	}
	else
	{
		addr -= 0x10010000;
		memory[addr] = val & 0xff;
		memory[addr + 1] = (val >> 8) & 0xff;
		memory[addr + 2] = (val >> 16) & 0xff;
		memory[addr + 3] = (val >> 24);
	}
}

void Memory::storeByte(char val)
{
	memory[currAddress] = val;
	currAddress++;
}

void Memory::storeHalf(short val)
{
	while (currAddress%2!=0)currAddress++;
	memory[currAddress] = val & 0xff;
	memory[currAddress + 1] = val >> 8;
	currAddress += 2;
}

void Memory::storeWord(int val)
{
	while (currAddress%4!=0)currAddress++;
	memory[currAddress] = val & 0xff;
	memory[currAddress + 1] = (val >> 8) & 0xff;
	memory[currAddress + 2] = (val >> 16) & 0xff;
	memory[currAddress + 3] = (val >> 24);
	currAddress += 4;
}

void Memory::MemoryArray(string path)
{
    int i=0;
    ifstream in;
    in.open(path.c_str(), ios_base::in | ios_base::binary);
    if(in.is_open())
    {
        while (!in.eof() && i<8192 )
        {
           in.get(memory[i]);
           i++;
        }
    }
    else
    {
        textBrowser->append("Error in opening memory file.\n");
		TERMINATE = true;
    }
}

void Memory::memoryDump(const string & memoryDumpFile)
{
	ofstream output(memoryDumpFile.c_str(), ios_base::out | ios_base::binary);
	for (int i = 0; i < 8192; i++)
		output << memory[i];
	output.close();
}

