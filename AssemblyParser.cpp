#include "AssemblyParser.h"
#include "GLOBALS.h"

#include <fstream>
#include <iostream>

using namespace std;

AssemblyParser::AssemblyParser(const string & file, const string & textDumpFile, const string & memoryDumpFile, Memory & memory, vector<Instruction> & inst)
{
	this->assemblyFile = file;
	this->textDumpFile = textDumpFile;
	this->memoryDumpFile = memoryDumpFile;
	//No need for comments, just parsing
    getLabels();
    if (!TERMINATE)
        parseAssembly(memory, inst);
}

void AssemblyParser::getLabels()
{
	unsigned int dataAddress = 0x10010000;
    unsigned int textAddress = 0x00400000;
    string str;
	ifstream input;
    input.open(assemblyFile.c_str());
	if (!input.fail())
	{
        input >> str;
		while (!input.eof() && !TERMINATE)
		{
            while (str != ".data" && str != ".text" && !input.eof())
            {
                if (str[0] == '#')
                {
                    char c;
                    input.get(c);
                    while (c != '\n')
                        input.get(c);
                }
                input >> str;
            }
            if (str == ".data")
			{
                input >> str;
                while (str != ".text" && !input.eof() && !TERMINATE)
				{
                    if (str[0] == '#')
					{
						char c;
						input.get(c);
						while (c != '\n')
							input.get(c);
					}
                    else if (str == ".word")
					{
						char c;
						input.get(c);
                        while (!isalpha(c) && !isdigit(c) && c != '\'' && c != '#' && c != '\n' && !input.eof())
							input.get(c);
						while (c != '#' && c != '\n' && !input.eof())
						{
                            if (isdigit(c) || isalpha(c) || c == '\'')
							{
								while (dataAddress % 4)
									dataAddress++;
								dataAddress += 4;
								if (c == '\'')
								{
									input.get(c);
									while (c != '\'')
										input.get(c);
								}
								while (c != ' ' && c != '\t' && c != '\r' && c != ',' && c != '#' && c != '\n' && !input.eof())
									input.get(c);
								while (!isalpha(c) && !isdigit(c) && c != '\'' && c != '#' && c != '\n' && !input.eof())
									input.get(c);
							}
						}
						while (c != '\n')
							input.get(c);
					}
                    else if (str == ".half")
					{
						char c;
						input.get(c);
						while (!isalpha(c) && !isdigit(c) && c != '\'' && c != '#' && c != '\n' && !input.eof())
							input.get(c);
						while (c != '#' && c != '\n' && !input.eof())
						{
							if (isdigit(c) || isalpha(c) || c == '\'')
							{
								while (dataAddress % 2)
									dataAddress++;
								dataAddress += 2;
								if (c == '\'')
								{
									input.get(c);
									while (c != '\'')
										input.get(c);
								}
								while (c != ' ' && c != '\t' && c != '\r' && c != ',' && c != '#' && c != '\n' && !input.eof())
									input.get(c);
								while (!isalpha(c) && !isdigit(c) && c != '\'' && c != '#' && c != '\n' && !input.eof())
									input.get(c);
							}
						}
						while (c != '\n')
							input.get(c);
					}
                    else if (str == ".byte")
					{
						char c;
						input.get(c);
						while (!isalpha(c) && !isdigit(c) && c != '\'' && c != '#' && c != '\n' && !input.eof())
							input.get(c);
						while (c != '#' && c != '\n' && !input.eof())
						{
							if (isdigit(c) || isalpha(c) || c == '\'')
							{
								dataAddress++;
								if (c == '\'')
								{
									input.get(c);
									while (c != '\'')
										input.get(c);
								}
								while (c != ' ' && c != '\t' && c != '\r' && c != ',' && c != '#' && c != '\n' && !input.eof())
									input.get(c);
								while (!isalpha(c) && !isdigit(c) && c != '\'' && c != '#' && c != '\n' && !input.eof())
									input.get(c);
							}
						}
						while (c != '\n')
							input.get(c);
					}
                    else if (str == ".space")
					{
						char c;
						input.get(c);
						while (!isdigit(c) && c != '#' && c != '\n' && c != '\'' && !input.eof())
							input.get(c);
						if (isdigit(c))
						{
							string n = "";
							while (c != '#' && c != '\n' && c != ' ' && c != '\t' && c != '\r' && !input.eof())
							{
								n += c;
								input.get(c);
							}
							if (n.length() > 2)
							{
								if (n[1] == 'x')
									dataAddress += (unsigned int)(wordHexaToDecimal(n.substr(2)));
								else
									dataAddress += (unsigned int)(atoi(n.c_str()));
							}
							else
								dataAddress += (unsigned int)(atoi(n.c_str()));
						}
						else if (c == '\'')
						{
							input.get(c);
							if (c == '\\')
							{
								input.get(c);
								c = specialCharacter(c);
							}
							dataAddress += (unsigned char)(c);
						}
						while (c != '\n' && !input.eof())
							input.get(c);
					}
                    else if (str == ".ascii")
					{
						char c;
						input.get(c);
						while (c != '#' && c != '\n' && !input.eof())
						{
							while (c != '"' && c != '#' && c != '\n' && !input.eof())
								input.get(c);
							if (c == '"')
							{
								input.get(c);
								while (c != '"')
								{
									if (c == '\\')
										input.get(c);
									dataAddress++;
									input.get(c);
								}
								input.get(c);
							}
						}
						while (c != '\n' && !input.eof())
							input.get(c);
					}
                    else if (str == ".asciiz")
					{
						char c;
						input.get(c);
						while (c != '#' && c != '\n' && !input.eof())
						{
							while (c != '"' && c != '#' && c != '\n' && !input.eof())
								input.get(c);
							if (c == '"')
							{
								input.get(c);
								while (c != '"')
								{
									if (c == '\\')
										input.get(c);
									dataAddress++;
									input.get(c);
								}
								dataAddress++;
								input.get(c);
							}
						}
						while (c != '\n' && !input.eof())
							input.get(c);
					}
                    else if (str[str.length() - 1] == ':')
                        labels[str.substr(0, str.length() - 1)] = dataAddress;
					else
					{
                        cerr << "Parsing error.\n";
						TERMINATE = true;
					}
                    input >> str;
				}
			}
			else if (str == ".text")
			{
				input >> str;
				while (str != ".data" && !input.eof() && !TERMINATE)
				{
					if (str[str.length() - 1] == ':')
						labels[str.substr(0, str.length() - 1)] = textAddress;
					else if (str[0] == '#')
					{
						char c;
						input.get(c);
						while (c != '\n')
							input.get(c);
					}
					else if (str == ".globl")
						input >> str;
					else if (isInstruction(str))
					{
						char c;
						input.get(c);
						while (c != '\n' && !input.eof())
							input.get(c);
						textAddress += 4;
					}
					else
					{
						if (str == "li" || str == "la" || str == "subi")
						{
							char c;
							input.get(c);
							while (c != '\n' && !input.eof())
								input.get(c);
							textAddress += 8;
						}
						else if (str == "move")
						{
							char c;
							input.get(c);
							while (c != '\n' && !input.eof())
								input.get(c);
							textAddress += 4;
						}
						else if (str[str.length() - 1] != ':')
						{
							cerr << "Parsing error!\n";
							TERMINATE = true;
						}
					}
					input >> str;
					if (str[str.length() - 1] == ':')
					{
						labels[str.substr(0, str.length() - 1)] = textAddress;
						input >> str;
					}
				}
			}
		}
		input.close();
	}
	else
	{
		cerr << "Failed to open assembly code file.\n";
		TERMINATE = true;
	}
}

void AssemblyParser::parseAssembly(Memory & memory, vector<Instruction> & inst)
{
    unsigned int textAddress = 0x00400000;
    string str;
    char c;
    vector<char>characters;
    ifstream input;
    ofstream output;
    input.open(assemblyFile.c_str());
    output.open(textDumpFile.c_str(), ios_base::out | ios_base::binary);
    if (!input.fail() && !output.fail())
    {
        input >> str;
        while (!input.eof())
        {
            while (str != ".data" && str != ".text" && !input.eof())
            {
                if (str[0] == '#')
                {
                    input.get(c);
                    while (c != '\n')
                        input.get(c);
                }
                input >> str;
            }
            if (str == ".data")
            {
                while (str != ".text" && !input.eof())
                {
                    if (str[0] == '#')
                    {
                        input.get(c);
                        while (c != '\n')
                            input.get(c);
                    }
                    else if (str == ".word")
                    {
                        input.get(c);
                        while (!isalpha(c) && !isdigit(c) && c != '-' && c != '\'' && c != '#' && c != '\n' && !input.eof())
                            input.get(c);
                        while (c != '\n' && !input.eof())
                        {
                            if (isdigit(c) || isalpha(c) || c == '\'' || c == '-')
                            {
                                string word = "";
                                if (c == '\'')
                                {
                                    input.get(c);
                                    if (c == '\\')
                                    {
                                        input.get(c);
                                        c = specialCharacter(c);
                                    }
                                    memory.storeWord((int)((unsigned char)(c)));
                                    input.get(c);
                                    input.get(c);
                                }
                                else
                                {
                                    while (c != ' ' && c != '\t' && c != '\r' && c != ',' && c != '#' && c != '\n' && !input.eof())
                                    {
                                        word += c;
                                        input.get(c);
                                    }
                                    if (isalpha(word[0]))
                                        memory.storeWord(labels[word]);
                                    else
                                    {
                                        if (word.length() > 2)
                                        {
                                            if (word[1] == 'x')
                                                memory.storeWord(wordHexaToDecimal(word.substr(2)));
                                            else
                                                memory.storeWord(atoi(word.c_str()));
                                        }
                                        else
                                            memory.storeWord(atoi(word.c_str()));
                                    }
                                }
                                while (!isalpha(c) && !isdigit(c) && c != '-' && c != '\'' && c != '#' && c != '\n' && !input.eof())
                                    input.get(c);
                            }
                        }
                        while (c != '\n' && !input.eof())
                            input.get(c);
                    }
                    else if (str == ".half")
                    {
                        input.get(c);
                        while (!isalpha(c) && !isdigit(c) && c != '-' && c != '\'' && c != '#' && c != '\n' && !input.eof())
                            input.get(c);
                        while (c != '\n' && !input.eof())
                        {
                            if (isdigit(c) || isalpha(c) || c == '\'' || c == '-')
                            {
                                string half = "";
                                if (c == '\'')
                                {
                                    input.get(c);
                                    if (c == '\\')
                                    {
                                        input.get(c);
                                        c = specialCharacter(c);
                                    }
                                    memory.storeHalf((short)((unsigned char)(c)));
                                    input.get(c);
                                    input.get(c);
                                }
                                else
                                {
                                    while (c != ' ' && c != '\t' && c != '\r' && c != ',' && c != '#' && c != '\n' && !input.eof())
                                    {
                                        half += c;
                                        input.get(c);
                                    }
                                    if (isalpha(half[0]))
                                        memory.storeHalf((short)(labels[half]));
                                    else
                                    {
                                        if (half.length() > 2)
                                        {
                                            if (half[1] == 'x')
                                                memory.storeHalf(halfHexaToDecimal(half.substr(2)));
                                            else
                                                memory.storeHalf((short)(atoi(half.c_str())));
                                        }
                                        else
                                            memory.storeHalf((short)(atoi(half.c_str())));
                                    }
                                }
                                while (!isalpha(c) && !isdigit(c) && c != '-' && c != '\'' && c != '#' && c != '\n' && !input.eof())
                                    input.get(c);
                            }
                        }
                        while (c != '\n' && !input.eof())
                            input.get(c);
                    }
                    else if (str == ".byte")
                    {
                        input.get(c);
                        while (!isalpha(c) && !isdigit(c) && c != '-' && c != '\'' && c != '#' && c != '\n' && !input.eof())
                            input.get(c);
                        while (c != '\n' && !input.eof())
                        {
                            if (isdigit(c) || isalpha(c) || c == '\'' || c == '-')
                            {
                                string byte = "";
                                if (c == '\'')
                                {
                                    input.get(c);
                                    if (c == '\\')
                                    {
                                        input.get(c);
                                        c = specialCharacter(c);
                                    }
                                    memory.storeByte(c);
                                    input.get(c);
                                    input.get(c);
                                }
                                else
                                {
                                    while (c != ' ' && c != '\t' && c != '\r' && c != ',' && c != '#' && c != '\n' && !input.eof())
                                    {
                                        byte += c;
                                        input.get(c);
                                    }
                                    if (isalpha(byte[0]))
                                        memory.storeByte((char)(labels[byte]));
                                    else
                                    {
                                        if (byte.length() > 2)
                                        {
                                            if (byte[1] == 'x')
                                                memory.storeByte(byteHexaToDecimal(byte.substr(2)));
                                            else
                                                memory.storeByte((char)(atoi(byte.c_str())));
                                        }
                                        else
                                            memory.storeByte((char)(atoi(byte.c_str())));
                                    }
                                }
                                while (!isalpha(c) && !isdigit(c) && c != '-' && c != '\'' && c != '#' && c != '\n' && !input.eof())
                                    input.get(c);
                            }
                        }
                        while (c != '\n' && !input.eof())
                            input.get(c);
                    }
                    else if (str == ".space")
                    {
                        input.get(c);
                        while (!isdigit(c) && c != '#' && c != '\n' && c != '\'' && !input.eof())
                            input.get(c);
                        if (c == '\'')
                        {
                            input.get(c);
                            if (c == '\\')
                            {
                                input.get(c);
                                c = specialCharacter(c);
                            }
                            for (char i = 0; i < c; i++)
                                memory.storeByte('\0');
                            input.get(c);
                        }
                        else if (isdigit(c))
                        {
                            string n = "";
                            while (c != ' ' && c != '\t' && c != '\r' && c != '#' && c != '\n' && !input.eof())
                            {
                                n += c;
                                input.get(c);
                            }
                            if (n.length() > 2)
                            {
                                if (n[1] == 'x')
                                {
                                    for (int i = 0; i < wordHexaToDecimal(n.substr(2)); i++)
                                        memory.storeByte('\0');
                                }
                                else
                                {
                                    for (int i = 0; i < atoi(n.c_str()); i++)
                                        memory.storeByte('\0');
                                }
                            }
                            else
                            {
                                for (int i = 0; i < atoi(n.c_str()); i++)
                                    memory.storeByte('\0');
                            }
                        }
                        while (c != '\n' && !input.eof())
                            input.get(c);
                    }
                    else if (str == ".ascii")
                    {
                        input.get(c);
                        while (c != '#' && c != '\n' && !input.eof())
                        {
                            while (c != '"' && c != '#' && c != '\n' && !input.eof())
                                input.get(c);
                            if (c == '"')
                            {
                                input.get(c);
                                while (c != '"')
                                {
                                    if (c == '\\')
                                    {
                                        input.get(c);
                                        c = specialCharacter(c);
                                    }
                                    memory.storeByte(c);
                                    input.get(c);
                                }
                                input.get(c);
                            }
                        }
                        while (c != '\n' && !input.eof())
                            input.get(c);
                    }
                    else if (str == ".asciiz")
                    {
                        input.get(c);
                        while (c != '#' && c != '\n' && !input.eof())
                        {
                            while (c != '"' && c != '#' && c != '\n' && !input.eof())
                                input.get(c);
                            if (c == '"')
                            {
                                input.get(c);
                                while (c != '"')
                                {
                                    if (c == '\\')
                                    {
                                        input.get(c);
                                        c = specialCharacter(c);
                                    }
                                    memory.storeByte(c);
                                    input.get(c);
                                }
                                memory.storeByte('\0');
                                input.get(c);
                            }
                        }
                        while (c != '\n' && !input.eof())
                            input.get(c);
                    }
                    input >> str;
                }
            }
            else if (str == ".text")
            {
                while (str != ".data" && !input.eof())
                {
                    if (str[0] == '#')
                    {
                        input.get(c);
                        while (c != '\n')
                            input.get(c);
                    }
                    else if (str == ".globl")
                        input >> str;
                    else if (isInstruction(str))
                    {
                        string instruction = "", initialInstruction = "";
                        initialInstruction += instruction += str;
                        if (str != "beq" && str != "bne" && str != "j" && str != "jal" && str != "lui")
                        {
                            input.get(c);
                            while (c != '\n' && c != '#' && !input.eof())
                            {
                                if (c == '\'')
                                {
                                    input.get(c);
                                    if (c == '\\')
                                    {
                                        input.get(c);
                                        c = specialCharacter(c);
                                    }
                                    initialInstruction += to_string((int)(c));
                                    instruction += to_string((int)(c));
                                    input.get(c);
                                    input.get(c);
                                }
                                else
                                {
                                    initialInstruction += c;
                                    instruction += c;
                                }
                                input.get(c);
                            }
                            while (c != '\n' && !input.eof())
                                input.get(c);
                        }
                        else if (str == "lui")
                        {
                            input.get(c);
                            while (c != ',')
                            {
                                initialInstruction += c;
                                instruction += c;
                                input.get(c);
                            }
                            while (!isdigit(c) && !isalpha(c) && c != '-' && c != '\'')
                            {
                                initialInstruction += c;
                                instruction += c;
                                input.get(c);
                            }
                            if (isalpha(c))
                            {
                                string label = "";
                                while (c != '\n' && c != '#' && c != ' ' && c != '\t' && c != '\r' && !input.eof())
                                {
                                    initialInstruction += c;
                                    label += c;
                                    input.get(c);
                                }
                                instruction += to_string(labels[label]);
                            }
                            else if (isdigit(c))
                            {
                                while (c != '\n' && c != '#' && !input.eof() && c != ' ' && c != '\t' && c != '\r')
                                {
                                    initialInstruction += c;
                                    instruction += c;
                                    input.get(c);
                                }
                            }
                            else if (c == '\'')
                            {
                                initialInstruction += c;
                                instruction += c;
                                input.get(c);
                                initialInstruction += c;
                                while (c != '\'')
                                {
                                    if (c == '\\')
                                    {
                                        input.get(c);
                                        initialInstruction += c;
                                        c = specialCharacter(c);
                                    }
                                    instruction += (int)(c);
                                    input.get(c);
                                    initialInstruction += c;
                                }
                                initialInstruction += c;
                            }
                            while (c != '\n' && !input.eof())
                                input.get(c);
                        }
                        else if (str == "j" || str == "jal")
                        {
                            input.get(c);
                            while (!isdigit(c) && !isalpha(c))
                            {
                                initialInstruction += c;
                                instruction += c;
                                input.get(c);
                            }
                            if (isalpha(c))
                            {
                                string label = "";
                                while (c != '\n' && c != '#' && c != ' ' && c != '\t' && c != '\r' && !input.eof())
                                {
                                    initialInstruction += c;
                                    label += c;
                                    input.get(c);
                                }
                                instruction += to_string(labels[label]);
                            }
                            else
                            {
                                while (c != '\n' && c != '#' && !input.eof() && c != ' ' && c != '\t' && c != '\r')
                                {
                                    initialInstruction += c;
                                    instruction += c;
                                    input.get(c);
                                }
                            }
                            while (c != '\n' && !input.eof())
                                input.get(c);
                        }
                        else
                        {
                            input.get(c);
                            while (c != '$')
                            {
                                initialInstruction += c;
                                instruction += c;
                                input.get(c);
                            }
                            initialInstruction += c;
                            instruction += c;
                            input.get(c);
                            while (c != '$')
                            {
                                initialInstruction += c;
                                instruction += c;
                                input.get(c);
                            }
                            while (c != ',' && c != ' ' && c != '\t' && c != '\r')
                            {
                                initialInstruction += c;
                                instruction += c;
                                input.get(c);
                            }
                            while (!isalpha(c) && !isdigit(c))
                            {
                                initialInstruction += c;
                                instruction += c;
                                input.get(c);
                            }
                            if (isdigit(c))
                            {
                                string n = "";
                                while (c != ' ' && c != '\t' && c != '\r' && c != '#' && c != '\n' && !input.eof())
                                {
                                    initialInstruction += c;
                                    n += c;
                                    input.get(c);
                                }
                                int num;
                                if (n.length() > 2)
                                {
                                    if (n[1] == 'x')
                                        num = wordHexaToDecimal(n.substr(2));
                                    else
                                        num = atoi(n.c_str());
                                }
                                else
                                    num = atoi(n.c_str());
                                num = (num - (textAddress + 4)) / 4;
                                instruction += to_string(num);
                            }
                            else
                            {
                                string label = "";
                                while (c != ' ' && c != '\t' && c != '\r' && c != '#' && c != '\n' && !input.eof())
                                {
                                    initialInstruction += c;
                                    label += c;
                                    input.get(c);
                                }
                                int num = (labels[label] - (textAddress + 4)) / 4;
                                instruction += to_string(num);
                            }
                            while (c != '\n' && !input.eof())
                                input.get(c);
                        }
                        textAddress += 4;
                        Instruction instr;
                        instr.assemble(instruction, initialInstruction);
                        inst.push_back(instr);
                        toChar(instr.getBinaryInstruction(), characters);
                        for (int i = 3; i >= 0; i--)
                            output << characters[i];;
                    }
                    else
                    {
                        if (str == "li" || str == "la")
                        {
                            string instruction = "lui $1, ";
                            string reg = "", immediate = "";
                            int imm;
                            input.get(c);
                            while (c != '$')
                                input.get(c);
                            while (c != ' ' && c != ',' && c != '\t' && c != '\r')
                            {
                                reg += c;
                                input.get(c);
                            }
                            while (!isdigit(c) && !isalpha(c) && c != '\'')
                                input.get(c);
                            if (isdigit(c))
                            {
                                while (c != ' ' && c != '\t' && c != '\r' && c != '#' && c != '\n' && !input.eof())
                                {
                                    immediate += c;
                                    input.get(c);
                                }
                                if (immediate.length() > 2)
                                {
                                    if (immediate[1] == 'x')
                                        imm = wordHexaToDecimal(immediate.substr(2));
                                    else
                                        imm = atoi(immediate.c_str());
                                }
                                else
                                    imm = atoi(immediate.c_str());
                            }
                            else if (isalpha(c))
                            {
                                string label = "";
                                while (c != ' ' && c != '\t' && c != '\r'  && c != '\r'&& c != '#' && c != '\n' && !input.eof())
                                {
                                    label += c;
                                    input.get(c);
                                }
                                imm = labels[label];
                            }
                            else if (c == '\'')
                            {
                                input.get(c);
                                if (c == '\\')
                                {
                                    input.get(c);
                                    c = specialCharacter(c);
                                }
                                imm = (unsigned char)c;
                            }
                            int imm2;
                            imm2 = (imm >> 16);
                            instruction += to_string(imm2);
                            Instruction instr;
                            instr.assemble(instruction, instruction);
                            inst.push_back(instr);
                            toChar(instr.getBinaryInstruction(), characters);
                            for (int i = 3; i >= 0; i--)
                                output << characters[i];;
                            string instruction2 = "ori ";
                            instruction2 += reg;
                            instruction2 += ", $1, ";
                            instruction2 += to_string(imm & 0x0000ffff);
                            Instruction instr2;
                            instr2.assemble(instruction2, instruction2);
                            inst.push_back(instr2);
                            toChar(instr2.getBinaryInstruction(), characters);
                            for (int i = 3; i >= 0; i--)
                                output << characters[i];;
                            textAddress += 8;
                            while (c != '\n' && !input.eof())
                                input.get(c);
                        }
                        else if (str == "move")
                        {
                            string reg1 = "", reg2 = "";
                            input.get(c);
                            while (c != '$')
                                input.get(c);
                            while (c != ',' && c != ' ' && c != '\t' && c != '\r')
                            {
                                reg1 += c;
                                input.get(c);
                            }
                            while (c != '$')
                                input.get(c);
                            while (c != ' ' && c != '\t' && c != '\r' && c != '#' && c != '\n' && !input.eof())
                            {
                                reg2 += c;
                                input.get(c);
                            }
                            string instruction = "addu ";
                            instruction += reg1;
                            instruction += ", $0, ";
                            instruction += reg2;
                            Instruction instr;
                            instr.assemble(instruction, instruction);
                            inst.push_back(instr);
                            toChar(instr.getBinaryInstruction(), characters);
                            for (int i = 3; i >= 0; i--)
                                output << characters[i];;
                            textAddress += 4;
                            while (c != '\n' && !input.eof())
                                input.get(c);
                        }
                        else if (str == "subi")
                        {
                            string reg1 = "", reg2 = "", imm = "";
                            input.get(c);
                            while (c != '$')
                                input.get(c);
                            while (c != ' ' && c != '\t' && c != '\r' && c != ',')
                            {
                                reg1 += c;
                                input.get(c);
                            }
                            while (c != '$')
                                input.get(c);
                            while (c != ' ' && c != '\t' && c != '\r' && c != ',')
                            {
                                reg2 += c;
                                input.get(c);
                            }
                            while (!isdigit(c) && c != '-' && c != '\'')
                                input.get(c);
                            if (c == '\'')
                            {
                                input.get(c);
                                if (c == '\\')
                                {
                                    input.get(c);
                                    c = specialCharacter(c);
                                }
                                imm += c;
                            }
                            else
                            {
                                while (c != ' ' && c != '\t' && c != '\r' && c != '#' && c != '\n' && !input.eof())
                                {
                                    imm += c;
                                    input.get(c);
                                }
                            }
                            string instruction1 = "addi $1, $0, ";
                            instruction1 += imm;
                            Instruction instr1;
                            instr1.assemble(instruction1, instruction1);
                            inst.push_back(instr1);
                            toChar(instr1.getBinaryInstruction(), characters);
                            for (int i = 3; i >= 0; i--)
                                output << characters[i];;
                            string instruction2 = "sub ";
                            instruction2 += reg1;
                            instruction2 += ", ";
                            instruction2 += reg2;
                            instruction2 += ", $1";
                            Instruction instr2;
                            instr2.assemble(instruction2, instruction2);
                            inst.push_back(instr2);
                            toChar(instr2.getBinaryInstruction(), characters);
                            for (int i = 3; i >= 0; i--)
                                output << characters[i];;
                            textAddress += 8;
                            while (c != '\n' && !input.eof())
                                input.get(c);
                        }
                    }
                    input >> str;
                    if (str == "syscall")
                    {
                        textAddress += 4;
                        Instruction instr;
                        instr.assemble(str, str);
                        inst.push_back(instr);
                        toChar(instr.getBinaryInstruction(), characters);
                        for (int i = 3; i >= 0; i--)
                            output << characters[i];;
                        input >> str;
                    }
                    else if (str[str.length() - 1] == ':')
                        input >> str;
                }
            }
        }
        input.close();
        output.close();
        memory.memoryDump(memoryDumpFile);
    }
    else
    {
        if (input.fail())
			cerr << "Failed to open assembly code file.\n";
        else
			cerr << "Failed to open text dump file.\n";
        TERMINATE = true;
    }
}

bool AssemblyParser::isInstruction(const string & instruction) const
{
	return (instruction == "j" || instruction == "jal" || instruction == "add" || instruction == "addu" || instruction == "sub" || instruction == "and" || instruction == "or" || instruction == "xor" || instruction == "slt" || instruction == "jr" || instruction == "sll" || instruction == "srl" || instruction == "syscall" || instruction == "lui" || instruction == "beq" || instruction == "bne" || instruction == "lw" || instruction == "sw" || instruction == "lb" || instruction == "sb" || instruction == "lh" || instruction == "sh" || instruction == "addi" || instruction == "addiu" || instruction == "andi" || instruction == "ori" || instruction == "xori");
}

int AssemblyParser::wordHexaToDecimal(const string & hexa) const
{
	int ans = 0;
	int i = hexa.length() - 1;
	int n = 8;
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

short AssemblyParser::halfHexaToDecimal(const string & hexa) const
{
	short ans = 0;
	int i = hexa.length() - 1;
	int n = 4;
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

char AssemblyParser::byteHexaToDecimal(const string & hexa) const
{
	char ans = 0;
	int i = hexa.length() - 1;
	int n = 2;
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

void AssemblyParser::toChar(const string & s, vector<char> & characters) const
{
	string chars[4];
	characters.clear();
	characters.resize(4);
	for (int i = 0; i < 4; i++)
		chars[i] = s.substr(i * 8, 8);
	for (int i = 0; i < 4; i++)
	{
		unsigned char ans = 0;
		for (int j = 0; j < 8; j++)
			ans = (ans << 1) + (chars[i][j] - '0');
		characters[i] = ans;
	}
}

char AssemblyParser::specialCharacter(char c) const
{
	switch (c)
	{
	case 'a':
		return '\a';
	case 'b':
		return '\b';
	case 'f':
		return '\f';
	case 'n':
		return '\n';
	case 'r':
		return '\r';
	case 't':
		return '\t';
	case 'v':
		return '\v';
	case '0':
		return '\0';
	default:
		return c;
	}
}
