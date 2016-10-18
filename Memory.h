#ifndef MEMORY_H
#define MEMORY_H

#include <QTextBrowser>

#include <string>

using namespace std;

class Memory
{
private:
	//Address pointer for filling memory from data segment
	unsigned int currAddress;
    QTextBrowser *textBrowser;
public:
    Memory();
    void getPointers(QTextBrowser *);
    char memory[8192];
    char loadByte(unsigned int);
    int loadWord(unsigned int);
    short loadHalf(unsigned int);
    void storeByte(unsigned int, char);
    void storeHalf(unsigned int, short);
    void storeWord(unsigned int, int);
	void MemoryArray(string);
	void memoryDump(const string &);
	//Filling memory from data segment
	void storeByte(char);
	void storeHalf(short);
	void storeWord(int);
};

#endif
