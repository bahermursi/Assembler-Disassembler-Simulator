-d <Text Dump File> [<Memory Dump File>] [<Assembly Code File>]
Example 1: Project.exe -d TextDump.bin MemoryDump.bin AssemblyCode.asm
Example 2: Project.exe -d TextDump.bin MemoryDump.bin AssemblyCode.s
Example 3: Project.exe -d TextDump.bin MemoryDump.bin
Example 4: Project.exe -d TextDump.bin AssemblyCode.asm
Example 5: Project.exe -d TextDump.bin AssemblyCode.s
Example 6: Project.exe -d TextDump.bin
-a <Assembly Code File> [<Text Dump File>] [<Memory Dump File>]
Example 1: Project.exe -a AssemblyCode.asm TextDump.bin MemoryDump.bin
Example 2: Project.exe -a AssemblyCode.s TextDump.bin MemoryDump.bin
Example 3: Project.exe -a AssemblyCode.asm TextDump.bin
Example 4: Project.exe -a AssemblyCode.s TextDump.bin
Example 5: Project.exe -a AssemblyCode.asm
Example 6: Project.exe -a AssemblyCode.s
Notes: 
1. -d means disassembler, -a means assembler
2. <> means necessary argument, [<>] means optional argument
3. An argument should not contain any spaces
4. The arguments are space-seperated
5. Dump files should have the extension .bin
6. Assembly code files should have the extension .asm or .s
7. Either one or both of the optional arguments in the disassembler can be used
8. If only one optional argument is used in the assembler, it is assumed to be the text dump file