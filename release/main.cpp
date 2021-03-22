// CPP Libraries
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include <queue>
#include <iterator>
#include <map>
#include <algorithm>
#include <bitset>
// C Libraries
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <cstdio>

using namespace std;

// Structures
struct MemoryLabel{
    string label;
    int32_t address;
};

struct InstructionField{
    int op;
    int rs;
    int rt;
    int shamt;
    int funct;
};

struct Simulator{
    // Main Memory
    vector<string> ram;
    int pc;
    int npc;
    
    // Processor Register
    vector <int> reg;
    int Hi;
    int Lo;
};

// Global Variables
vector<string> instrStore;
vector<string> ASMStore;
vector<MemoryLabel> TextLabelTable;
vector<MemoryLabel> DataLabelTable;
int dataSection;
int textSection;
int instrIndex = 0;
int mainExists = 0;
int mainIndex;      // Program counter should be set to main index if it exists
int inTextSection = 0;
int isLabelHeader = 0;
int textStart = 0x0400000;
int dataStart = 0x0500000;

// Parser
void scanFile(string inputFileName = ""){
    string lineBuffer;
    ifstream inputFile;

    int lineCounter = 0;

    if (inputFileName.empty()){
        cout << "Input file: ";
        cin >> inputFileName;
    }

    inputFile.open(inputFileName.c_str(), ifstream::in);
    
    while (getline(inputFile, lineBuffer)){
        isLabelHeader = 0;
        inTextSection = 1;

        string::size_type pos = lineBuffer.find("#");
        if (pos != string::npos){
            lineBuffer = (lineBuffer.substr(0,pos));    // Splits string at position of # to remove comments
        }

        if (lineBuffer == ".data"){       // Marks first instruction of data and text section in ASMStore
            isLabelHeader = 1;
            inTextSection = 0;
            dataSection = lineCounter;  
        }
        if (lineBuffer == ".text"){ 
            isLabelHeader = 1;  
            inTextSection = 1; 
            textSection = lineCounter;
        }

        if (lineBuffer.find("main") != string::npos){
            mainExists = 1;
            mainIndex = lineCounter;    // Where the instructions under main start
        }
        
        if (lineBuffer.find(":") != string::npos){        // Stores location of function label
            isLabelHeader = 1;
            MemoryLabel tempLabel;
            lineBuffer.erase(std::remove(lineBuffer.begin(), lineBuffer.end(),':'), lineBuffer.end()); // Remove ':'
            tempLabel.address = lineCounter;
            tempLabel.label = lineBuffer;
            if (inTextSection == 1){
                TextLabelTable.push_back(tempLabel);
            }
            else DataLabelTable.push_back(tempLabel);
            
        }

        if (lineBuffer.size()>0 && isLabelHeader != 1) {
            lineBuffer.erase(std::remove(lineBuffer.begin(), lineBuffer.end(),','), lineBuffer.end());
            ASMStore.push_back(lineBuffer); // Assembly Buffer
            lineCounter += 1;
        }
        
    }
}

// Parser Debug
void debug_ASMBuffer(){ // Check if buffer works properly
    while (!(ASMStore.empty())){
        cout << ASMStore.front() << '\n';
        ASMStore.erase(ASMStore.begin());
    }
    cout << "Data Section is in ASMStore[" << dataSection << "] \n";
    cout << "Text Section is in ASMStore[" << textSection << "] \n"; 
    cout << mainExists << " Main exists \n";
    cout << mainIndex << " Main index \n";
    cout << "First element of Textlabel table: " << TextLabelTable.front().label << "\n" ;
    cout << " First element of Datalabel table: " << DataLabelTable.front().label << "\n";
}

// Assembler

// Instruction Opcode Map
map<string, string> instrMap{
    {"add"    , "100000"},//
    {"addu"   , "100001"},//
    {"and"    , "100100"},//
    {"div"    , "011010"},//
    {"divu"   , "011011"},//
    {"jalr"   , "001001"},//
    {"jr"     , "001000"},//
    {"mfhi"   , "010000"},//
    {"mflo"   , "010010"},//
    {"mthi"   , "010001"},//
    {"mtlo"   , "010011"},//
    {"mult"   , "011000"},//
    {"multu"  , "011001"},//
    {"nor"    , "100111"},//
    {"or"     , "100101"},//
    {"sll"    , "000000"},//
    {"sllv"   , "000100"},//
    {"slt"    , "101010"},//
    {"sltu"   , "101011"},//
    {"sra"    , "000011"},//
    {"srav"   , "000111"},//
    {"srl"    , "000010"},//
    {"srlv"   , "000110"},//
    {"sub"    , "100010"},//
    {"subu"   , "100011"},//
    {"syscall", "001100"},//
    {"xor"    , "100110"},//
    {"clo"    , "100001"},//
    {"clz"    , "100000"},//
    {"mul"    , "000010"},//
    {"madd"   , "000000"},//
    {"maddu"  , "000001"},//
    {"msub"   , "000100"},//
    {"msubu"  , "000101"},//
    {"tlt"    , "110010"},//
    {"tltu"   , "110011"},//
    {"tge"    , "110000"},//
    {"tgeu"   , "110001"},//
    {"tne"    , "110110"},//
    {"teq"    , "110100"},//
    {"addi"   , "001000"},//
    {"addiu"  , "001001"},//
    {"andi"   , "001100"},//
    {"beq"    , "000100"},//
    {"bgez"   , "000001"},//
    {"bgezal" , "000001"},//
    {"bgtz"   , "000111"},//
    {"blez"   , "000110"},//
    {"bltz"   , "000001"},//
    {"bne"    , "000101"},//
    {"lb"     , "100000"},//
    {"lbu"    , "100100"},//
    {"lh"     , "100001"},//
    {"lhu"    , "100101"},//
    {"lui"    , "001111"},//
    {"lw"     , "100011"},//
    {"ori"    , "001101"},//
    {"sb"     , "101000"},//
    {"slti"   , "001010"},//
    {"sc"     , "111000"},//
    {"sltiu"  , "001011"},//
    {"sh"     , "101001"},//
    {"sw"     , "101011"},//
    {"xori"   , "001110"},//
    {"tlti"   , "000001"},//
    {"bltzal" , "000001"},//
    {"swr"    , "101110"},//
    {"swl"    , "101010"},//
    {"ll"     , "110000"},//
    {"tltiu"  , "000001"},//
    {"lwr"    , "100110"},//
    {"lwl"    , "100010"},//
    {"tgei"   , "000001"},//
    {"tgeiu"  , "000001"},//
    {"tnei"   , "000001"},//
    {"teqi"   , "000001"}, //
    {"j"      , "000010"},//
    {"jal"    , "000011"},//
};

map<string, string> registerMap {
    {"$zero"  , "00000"},
    {"$at"    , "00001"},
    {"$v0"    , "00010"},
    {"$v1"    , "00011"},
    {"$a0"    , "00100"},
    {"$a1"    , "00101"},
    {"$a2"    , "00110"},
    {"$a3"    , "00111"},
    {"$t0"    , "01000"},
    {"$t1"    , "01001"},
    {"$t2"    , "01010"},
    {"$t3"    , "01011"},
    {"$t4"    , "01100"},
    {"$t5"    , "01101"},
    {"$t6"    , "01110"},
    {"$t7"    , "01111"},
    {"$s0"    , "10000"},
    {"$s1"    , "10001"},
    {"$s2"    , "10010"},
    {"$s3"    , "10011"},
    {"$s4"    , "10100"},
    {"$s5"    , "10101"},
    {"$s6"    , "10110"},
    {"$s7"    , "10111"},
    {"$t8"    , "11000"},
    {"$t9"    , "11001"},
    {"$k0"    , "11010"},
    {"$k1"    , "11011"},
    {"$gp"    , "11100"},
    {"$sp"    , "11101"},
    {"$fp"    , "11110"},
    {"$ra"    , "11111"}
};

int getLabelAddr(string label){ // Check if works, otherwise replace with string
    int rel_labelAddr;
    int textStart = 0x0400000;
    for (int i = 0; i < int(TextLabelTable.size()); i++){  // iterator not working
        // cout << TextLabelTable[i].label <<'\n'; [For Debugging]
        if (TextLabelTable[i].label.find(label) != string::npos){
            rel_labelAddr = ((TextLabelTable[i].address)*4); // -> address is line of code starting from 0    
            int labelAddr = textStart + rel_labelAddr;
            return labelAddr;
        }
    }
    return -1; // If not label or not found
}

// MIPS Instruction Field
/*
 
 < R-type >
  31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
 +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 |    op(6bits)    |   rs(5bits)  |   rt(5bits)  |   rd(5bits)  | shamt(5bits) |   funct(6bits)  |
 +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 
 
 < I-type >
  31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
 +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 |    op(6bits)    |   rs(5bits)  |   rt(5bits)  |         constant or address (16bits)          |
 +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 
 < J-type >
  31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
 +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 |    op(6bits)    |                            address (30bits)                                 |
 +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 
*/

string mk_typeR(string op, string rs, string rt, string rd, string shamt, string funct){
    //op = std::bitset<5>(shamt).to_string();
    string instr = instrMap.find(funct)->first;
    rs = registerMap.find(rs)->second;
    rt = registerMap.find(rt)->second;
    rd = registerMap.find(rd)->second;
    if (rs.empty()) rs = "00000";
    if (rt.empty()) rt = "00000";
    if (rd.empty()) rd = "00000";
    shamt = std::bitset<5>(stoi(shamt)).to_string(); // shamt value change from int to 5 bit binary
    return op + rs + rt + rd + shamt + funct;
};

string mk_typeI(string op, string rs, string rt, string addr){
    rs = registerMap.find(rs)->second;
    rt = registerMap.find(rt)->second;
    addr = std::bitset<16>(stoi(addr)).to_string();
    if (rs.empty()) rs = "00000";
    if (rt.empty()) rt = "00000";
    return op + rs + rt + addr;
};

string mk_typeJ(string op, string addr){
    int tmp = stoi(addr);
    string ad = std::bitset<26>(tmp).to_string();
    return op + ad;
};

// Assemble tokens into machine code
string assembleLine(vector<string> tokens){
    string MIPS_binary;
    string instr = tokens.at(0);
    string opcode;
    int opcode_int;
    opcode = instrMap.find(instr)->second;
    opcode_int = stoi(opcode);
    
    // Type R instructions
    // CLO, CLZ
    if (instr == "clo" || instr == "clz"){ 
        MIPS_binary = mk_typeR("011100", tokens[2], "00000", tokens[1], "00000", opcode);
    }
    // ADD, ADDU, AND, SUB, SUBU, OR, NOR, SLT, SLTU, XOR
    else if (instr == "add" || instr == "addu" || instr == "and" || instr == "sub" || instr == "subu" || instr == "or" || instr == "nor" || instr == "slt" || instr == "sltu" || instr == "xor"){ 
        MIPS_binary = mk_typeR("000000", tokens[2], tokens[3], tokens[1], "00000", opcode);
    }
        
    // DIV, DIVU, MULT, MULTU
    else if (instr == "div" || instr == "divu" || instr == "mult" || instr == "mult" || instr == "multu"){
        MIPS_binary = mk_typeR("000000", tokens[1], tokens[2], "00000", "00000", opcode);
    }
    // MADD
    else if (instr == "madd"){ 
        MIPS_binary = mk_typeR("011100", tokens[1], tokens[2], "00000", "00000", opcode);
        }
    // MUL
    else if (instr == "mul"){ 
        MIPS_binary = mk_typeR("011100", tokens[2], tokens[3], tokens[1], "00000", opcode);
        }
    // SLL, SRL, SRA
    else if (instr == "sll" || instr == "srl" || instr == "sra"){ 
        MIPS_binary = mk_typeR("000000", "00000", tokens[2], tokens[1], tokens[3], opcode);
        }
    // MSUB
    else if (instr == "msub"){
        MIPS_binary = mk_typeR("011100", tokens[1], tokens[2], "00000", "00000", opcode);
        }
    // SLLV, SRLV, SRAV
    else if (instr == "sllv" || instr == "srlv" || instr == "srav") { 
        MIPS_binary = mk_typeR("000000", tokens[3], tokens[2], tokens[1], "00000", opcode);
        }
    // JR
    else if (instr == "jr"){
        MIPS_binary = mk_typeR("000000", tokens[1], "00000", "00000", "00000", opcode);
    }
    // JALR
    else if (instr == "jalr"){
        MIPS_binary = mk_typeR("000000", tokens[2], "00000", tokens[1], "00000", opcode);
    }
    // SYSCALL
    else if (instr == "syscall"){
        MIPS_binary = mk_typeR("000000", "00000", "00000", "00000", "00000", opcode);
    }
    // MFHI, MFLO
    else if (instr == "mfhi" || instr == "mflo"){
        MIPS_binary = mk_typeR("000000", "00000", "00000", tokens[01], "00000", opcode);
    }
    // MTHI, MTLO
    else if (instr == "mthi" || instr == "mtlo"){
        MIPS_binary = mk_typeR("000000", tokens[1], "00000", "00000", "00000", opcode);
    }
    // MADDU, MSUBU
    else if (instr == "maddu" || instr == "msubu"){
        MIPS_binary = mk_typeR("011100", tokens[1], tokens[2], "00000", "00000", opcode);
    }
    // TGE, TGEU, TLT, TLTU, TNE, TEQ
    else if (instr == "tge" || instr == "tgeu" || instr == "tlt" || instr == "tltu" || instr == "tne" || instr == "teq"){
        MIPS_binary = mk_typeR("000000", tokens[1], tokens[2], "00000", "00000", opcode);
    }
    
    // Type I Instructions
    // BEQ, BNE
    if (instr == "beq" || instr == "bne"){
        int tmp = getLabelAddr(tokens[3]);
        if (tmp != -1){ // If is label
            int relAddr = tmp - (0x0400000 + ((instrIndex)*4)+4); //Because 4 byte blocks
            MIPS_binary = mk_typeI(opcode, tokens[1], tokens[2], to_string(relAddr/4));
        }
        else{
            MIPS_binary = mk_typeI(opcode, tokens[1], tokens[2], tokens[3]);
        }     
    }
    // BLEZ, BGTZ
    else if (instr == "blez" || instr == "bgtz"){
        int tmp = getLabelAddr(tokens[2]);
        //cout << tmp << '\n'; // Debug line
        if (tmp != -1){ // If is label
            int relAddr = tmp - (0x0400000 + ((instrIndex)*4)+4); //Because 4 byte blocks
            MIPS_binary = mk_typeI(opcode, tokens[1], "00000", to_string(relAddr/4));
        }
        else{
            MIPS_binary = mk_typeI(opcode, tokens[1], "00000", tokens[2]);
        }
    }
    // ADDI, ADDIU, SLTI, SLTIU, ANDI, ORI, XORI
    else if (instr == "addi" || instr == "addiu" || instr == "slti" || instr == "sltiu" || instr == "andi" || instr == "ori" || instr == "xori"){
        MIPS_binary = mk_typeI(opcode, tokens[2], tokens[1], tokens[3]);
        }
    // LUI
    else if (instr == "lui"){
        MIPS_binary = mk_typeI(opcode, "00000", tokens[1], tokens[2]);
    }
    // LB, LH, LW, LBU, LHU, SB, SH, SW, SC, SWR, SWL, LL, LWR, LWL
    else if (instr == "lb" || instr == "lh" || instr == "lw" || instr == "lbu"|| instr == "lbu"|| instr == "lhu"|| instr == "sb" || instr == "sh" || instr == "sw" || instr == "sc" || instr == "swr" || instr == "swl" || instr == "ll" || instr == "lwr" || instr == "lwl"){   
        size_t oBracket = tokens[2].find("(");
        size_t cBracket = tokens[2].find(")");
        string rs = tokens[2].substr(oBracket +1, cBracket-(oBracket+1));
        mk_typeI(opcode, rs, tokens[1], tokens[2]);
    }
    // BGEZ, BGEZAL, BLTZAL, BLTZ
    else if (instr == "bgez" || instr == "bgezal" || instr == "bltzal" || instr == "bltz"){
        int tmp = getLabelAddr(tokens[2]);
        if (tmp != -1){ // If is label
            int relAddr = tmp - (0x0400000 + ((instrIndex)*4)+4); //Because 4 byte blocks
            if      (instr == "bgez")   MIPS_binary = mk_typeI(opcode, tokens[1], "00001", to_string(relAddr/4));
            else if (instr == "bgezal") MIPS_binary = mk_typeI(opcode, tokens[1], "10001", to_string(relAddr/4));
            else if (instr == "bltzal") MIPS_binary = mk_typeI(opcode, tokens[1], "10000", to_string(relAddr/4));
            else if (instr == "bltz")   MIPS_binary = mk_typeI(opcode, tokens[1], "00000", to_string(relAddr/4));            
        }
        else{
            if      (instr == "bgez")   MIPS_binary = mk_typeI(opcode, tokens[1], "00001", tokens[2]);
            else if (instr == "bgezal") MIPS_binary = mk_typeI(opcode, tokens[1], "10001", tokens[2]);
            else if (instr == "bltzal") MIPS_binary = mk_typeI(opcode, tokens[1], "10000", tokens[2]);
            else if (instr == "bltz")   MIPS_binary = mk_typeI(opcode, tokens[1], "00000", tokens[2]);
        }
    }
    // TLTI
    else if (instr == "tlti"){
        MIPS_binary = mk_typeI(opcode, tokens[1], "01010", tokens[2]);
    }
    // TLTIU
    else if (instr == "tltiu"){
        MIPS_binary = mk_typeI(opcode, tokens[1], "01011", tokens[2]);
    }
    // TGEI
    else if (instr == "tgei"){
        MIPS_binary = mk_typeI(opcode, tokens[1], "01000", tokens[2]);
    }
    // TGEIU
    else if (instr == "tgeiu"){
        MIPS_binary = mk_typeI(opcode, tokens[1], "01001", tokens[2]);
    }
    // TNEI
    else if (instr == "tnei"){
        MIPS_binary = mk_typeI(opcode, tokens[1], "01110", tokens[2]);
    }
    // TEQI
    else if (instr == "teqi"){
        MIPS_binary = mk_typeI(opcode, tokens[1], "01100", tokens[2]);
    }
    // Type J Instructions
    // J
    else if (instr == "j"){
        int tmp = getLabelAddr(tokens[1]);
        if (tmp != -1){
            MIPS_binary = mk_typeJ(opcode, to_string(tmp/4));
        }
    }
    // JAL    
    else if (instr == "jal"){
        int tmp = getLabelAddr(tokens[1]);
        if (tmp != -1){
            MIPS_binary = mk_typeJ(opcode, to_string(tmp>>2));
        }
    }
    instrIndex++;
    return MIPS_binary;
}

// Assemble instruction buffer line by line
void assembleBuffer(){
    string lineBuffer;
    string opCode;
    string machineCode; 
    
    for (int ptr = textSection; ptr< int(ASMStore.size()); ptr++){ // Iterates line by line starting from .text section
        lineBuffer = ASMStore.at(ptr);
        //lineBuffer.erase(std::remove(lineBuffer.begin(), lineBuffer.end(),','), lineBuffer.end()); // Remove commas
        stringstream instrStream(lineBuffer); // Tokenizes the instruction line
        istream_iterator<string> begin(instrStream);
        istream_iterator<string> end;
        vector<string> tokens(begin, end);
        machineCode = assembleLine(tokens);
        instrStore.push_back(machineCode);
        //cout << instrStore.back() << '\n';
    }
}

void debugMkInst(){
    string TEST_MIPS_binary;
    vector<string> testToken;
    testToken.push_back("add");
    testToken.push_back("$t0");
    testToken.push_back("$t0");
    testToken.push_back("$t2");
    TEST_MIPS_binary = assembleLine(testToken);
    cout << TEST_MIPS_binary <<'\n';
    cout << TEST_MIPS_binary.size() <<'\n';
}

void printMachineCode(){ // Not working
    for (int i; i <= int(instrStore.size()); i++){
        cout << instrStore[i] << '\n';
    }
}

void debugTextLabelTable(){
    cout << "Text Label Table Size: " << TextLabelTable.size() << '\n';
    cout << TextLabelTable[0].label << '\n';
}

/*
int main(){
    scanFile();
    //debug_ASMBuffer(); // Only works if there is both data and text
    //debugTextLabelTable();
    assembleBuffer();
    //debugMkInst();
    return 0;
}*/

// TO-DO
// Fix Jump and Branch instructions

int main(int argc, char *argv[]){
    // Assembler Module
    if(argc != 2){ // If no file passed to program
        scanFile();
    }
    else{
        scanFile(argv[1]);
    }
    
    assembleBuffer(); // Assembles MIPS ASM and stores to vector instrStore    
    
    // Initialize Simulation
    Simulator sim;
    sim.ram.resize(0x5B8D80);    // 6MB of RAM initialised
    sim.pc = 0x0400000;          // Start of text segment
    sim.npc = 0x0400000+4;       // Next instruction pointer (increments by 4)
    sim.reg.resize(32,0);        // 32 Registers
    sim.Hi = 0;
    sim.Lo = 0;

    // Load Instructions into Memory
    int instEnd = 0;

    for (int i = 0; i < int(instrStore.size()); i++){
        int ramBeg = 0x0400000;
        int ramPtr = (i*4);
        int ramAddr = ramBeg + ramPtr;
        sim.ram.at(ramAddr) = (instrStore[i]);
        instEnd = ramAddr;
    }

    // Load Data Section
    for (int j = 0; j < int(DataLabelTable.size()); j++){
        int ramBeg = 0x0500000;
        int ramPtr = (j*4);
        sim.ram[ramBeg + ramPtr] = (DataLabelTable[j].label);
    }

    // Simulation cycle || Fetch, Decode, Execute

    while (int(sim.pc) <= instEnd){
        sim.reg.at(0) = 0; // Zero register always 0
        // Fetch instruction from memory
        string mCodeBuffer = sim.ram[sim.pc];

        // Instruction Decode
        string instrBuffer = mCodeBuffer;
        
        // Execute Instruction
        cout << "execute instruction: " << instrBuffer << '\n';

        sim.pc += 4;
        if (sim.pc == instEnd){
            break;
        } 
    }
    return 0;
}