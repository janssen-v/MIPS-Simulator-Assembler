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

// Parser
void scanFile(string inputFileName = ""){
    string lineBuffer;
    ifstream inputFile;

    int lineCounter = 0;

    if (inputFileName.empty()){
        cout << "Input file: ";
        cin >> inputFileName;
        inputFile.open(inputFileName.c_str(), ifstream::in);
    }
    
    while (getline(inputFile, lineBuffer)){
        isLabelHeader = 0;

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
            tempLabel.address = lineCounter;
            tempLabel.label = lineBuffer;
            if (inTextSection == 1){
                TextLabelTable.push_back(tempLabel);
            }
            else DataLabelTable.push_back(tempLabel);
            
        }

        if (lineBuffer.size()>0 && isLabelHeader != 1) {
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
map<string, string> typeR_map{
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
    {"teq"    , "110100"} //
};
map<string, string> typeI_map{
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
    {"teqi"   , "000001"} //
};
map<string, string> typeJ_map {
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
    for (int i; i <= int(TextLabelTable.size()); i++){
        if (TextLabelTable[i].label.find(label) != string::npos){
            rel_labelAddr = ((TextLabelTable[i].address)*4); // -> address is line of code starting from 0    
            int labelAddr = textStart + rel_labelAddr;
            return labelAddr;
            break;
        }
    }
    return -1; // If not label or not found
}

string hex2Bin16(int hex){
    bitset<16> b(hex);
    return b.to_string();
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

string mk_typeR(string op = "000000", string rs = "00000", string rt = "00000", string rd = "00000", string shamt = "00000", string funct = "000000"){
    //op = std::bitset<5>(shamt).to_string();
    string instr = typeR_map.find(funct)->first;
    rs = registerMap.find(rs)->second;
    rt = registerMap.find(rt)->second;
    rd = registerMap.find(rd)->second;
    shamt = std::bitset<5>(stoi(shamt)).to_string(); // shamt value change from int to 5 bit binary
    instrIndex += 1;
    return op + rs + rt + rd + shamt + funct;
};

string mk_typeI(string op, string rs, string rt, string addr){
    rs = registerMap.find(rs)->second;
    rt = registerMap.find(rt)->second;
    addr = std::bitset<16>(stoi(addr)).to_string();
    if (rs.empty()) rs = "00000";
    if (rt.empty()) rt = "00000";
    instrIndex += 1;
    return op + rs + rt + addr;
};

string mk_typeJ(string op = "000000", string addr = "00000000000000000000000000"){
    addr = std::bitset<26>(stoi(addr)).to_string();
    instrIndex += 1;
    return op + addr;
};

// Assemble tokens into machine code
string assembleLine(vector<string> tokens){
    string MIPS_binary;
    string instr = tokens.at(0);
    string opcode;
    int opcode_int;
    cout << "test" << '\n';
    if (typeR_map.count(instr)){
        opcode = typeR_map.find(instr)->second;
        opcode_int = stoi(opcode);
        
        switch (opcode_int) { // Assemble R-Type Instructions
            
            // ADD, ADDU, AND, SUB, SUBU, OR, NOR, SLT, SLTU, XOR || CLO, CLZ
            case 100000: case 100001: case 100100: case 100010: case 100011: 
            case 100101: case 100111: case 101010: case 101011: case 100110:
    
                if (instr == "clo" || instr == "clz"){ // CLO, CLZ
                    MIPS_binary = mk_typeR("011100", tokens[2], "00000", tokens[1], "00000", opcode);
                    break;
                }
                else{ // ADD, ADDU, AND, SUB, SUBU, OR, NOR, SLT, SLTU, XOR
                    MIPS_binary = mk_typeR("000000", tokens[2], tokens[3], tokens[1], "00000", opcode);
                    break;
                }
                
            // DIV, DIVU, MULT, MULTU
            case 011010: case 011011: case 011000: case 011001:
                MIPS_binary = mk_typeR("000000", tokens[1], tokens[2], "00000", "00000", opcode);
                break;
    
            // SLL, SRL, SRA || MADD || MUL
            case 000000: case 000010: case 000011:
                if (instr == "madd"){ // MADD
                    MIPS_binary = mk_typeR("011100", tokens[1], tokens[2], "00000", "00000", opcode);
                    break;
                }
                else if (instr == "mul"){ // MUL
                    MIPS_binary = mk_typeR("011100", tokens[2], tokens[3], tokens[1], "00000", opcode);
                    break;
                }
    
                else{ // SLL, SRL, SRA
                    MIPS_binary = mk_typeR("000000", "00000", tokens[2], tokens[1], tokens[3], opcode);
                    break;
                }
    
            // SLLV, SRLV, SRAV || MSUB
            case 000100: case 000110: case 000111:
                if (instr == "msub"){ // MSUB
                    MIPS_binary = mk_typeR("011100", tokens[1], tokens[2], "00000", "00000", opcode);
                    break;
                }
                else{ // SLLV, SRLV, SRAV
                    MIPS_binary = mk_typeR("000000", tokens[3], tokens[2], tokens[1], "00000", opcode);
                    break;
                }
    
            // JR
            case 001000:
                MIPS_binary = mk_typeR("000000", tokens[1], "00000", "00000", "00000", opcode);
                break;
    
            // JALR
            case 001001:
                MIPS_binary = mk_typeR("000000", tokens[2], "00000", tokens[1], "00000", opcode);
                break;
            // SYSCALL
            case 001100:
                MIPS_binary = mk_typeR("000000", "00000", "00000", "00000", "00000", opcode);
                break;
    
            // MFHI, MFLO
            case 010000: case 010010:
                MIPS_binary = mk_typeR("000000", "00000", "00000", tokens[01], "00000", opcode);
                break;
    
            // MTHI, MTLO
            case 010001: case 010011:
                MIPS_binary = mk_typeR("000000", tokens[1], "00000", "00000", "00000", opcode);
                break;
    
            // MADDU, MSUBU
            case 000001: case 000101:
                MIPS_binary = mk_typeR("011100", tokens[1], tokens[2], "00000", "00000", opcode);
                break;
    
            // TGE, TGEU, TLT, TLTU, TNE, TEQ
            case 110000: case 110001: case 110010: case 110011: case 110110: case 110100:
                MIPS_binary = mk_typeR("000000", tokens[1], tokens[2], "00000", "00000", opcode);
                break;
        }
    }

    if (typeI_map.count(instr)){
        opcode = typeI_map.find(instr)->second;
        opcode_int = stoi(opcode);
        string label;
        //machineCode = mk_typeI();

        // BEQ, BNE
        if (instr == "beq" || instr == "bne"){
            int tmp = getLabelAddr(tokens[3]);
            if (tmp != -1){ // If is label
                int relAddr = tmp - (0x0400000 + ((instrIndex)*4)+4); //Because 4 byte blocks
                MIPS_binary = mk_typeI(opcode, tokens[1], tokens[2], hex2Bin16(relAddr/4));
            }
            else{
                MIPS_binary = mk_typeI(opcode, tokens[1], tokens[2], tokens[3]);
            }     
        }
        // BLEZ, BGTZ
        else if (instr == "blez" || instr == "bgtz"){
            int tmp = getLabelAddr(tokens[2]);
            if (tmp != -1){ // If is label
                int relAddr = tmp - (0x0400000 + ((instrIndex)*4)+4); //Because 4 byte blocks
                MIPS_binary = mk_typeI(opcode, tokens[1], "00000", hex2Bin16(relAddr/4));
            }
            else{
                MIPS_binary = mk_typeI(opcode, tokens[1], "00000", tokens[2]);
            }
        }
        // ADDI, ADDIU, SLTI, SLTIU, ANDI, ORI, XORI
        else if (instr == "addi" || instr == "addiu" || instr == "slti" || instr == "sltiu" || instr == "andi" || instr == "ori" || instr == "xori"){
            MIPS_binary = mk_typeI(opcode, tokens[2], tokens[1], tokens[3]);
            }
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
                if      (instr == "bgez")   MIPS_binary = mk_typeI(opcode, tokens[1], "00001", hex2Bin16(relAddr/4));
                else if (instr == "bgezal") MIPS_binary = mk_typeI(opcode, tokens[1], "10001", hex2Bin16(relAddr/4));
                else if (instr == "bltzal") MIPS_binary = mk_typeI(opcode, tokens[1], "10000", hex2Bin16(relAddr/4));
                else if (instr == "bltz")   MIPS_binary = mk_typeI(opcode, tokens[1], "00000", hex2Bin16(relAddr/4));            
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


    if (typeJ_map.count(instr)){
        opcode = typeJ_map.find(instr)->second;
        opcode_int = stoi(opcode);
        //machineCode = mk_typeJ();

        if      (instr == "j"){
            int tmp = getLabelAddr(tokens[1]);
            if (tmp != -1){
                MIPS_binary = mk_typeJ(opcode, to_string(tmp/4));
            }
        }    
        else if (instr == "jal"){
            int tmp = getLabelAddr(tokens[1]);
            if (tmp != -1){
                MIPS_binary = mk_typeJ(opcode, to_string(tmp>>2));
            }
        }
    }

    return MIPS_binary;


    //string instrType = opcode.substr(opcode.length() - 2);
    //cout << instrType << '\n';

    /*
    void mk_typeR(int op, int rs, int rt, int rd, int shamt, int funct);
    void mk_typeI(int op, int rs, int rt, int addr);
    void mk_typeJ(int op, int addr);
    */
}

// Assemble instruction buffer line by line
void assembleBuffer(){
    string lineBuffer;
    string opCode;
    string machineCode; 
    
    for (int ptr = textSection; ptr< int(ASMStore.size()); ptr++){ // Iterates line by line starting from .text section
        lineBuffer = ASMStore.at(ptr);
        stringstream instrStream(lineBuffer); // Tokenizes the instruction line
        istream_iterator<string> begin(instrStream);
        istream_iterator<string> end;
        vector<string> tokens(begin, end);
        machineCode = assembleLine(tokens);
        instrStore.push_back(machineCode);

        //[DEBUG] -> Check if tokenisation of line correct
        /*
        cout << lineBuffer << '\n';
        cout << "ft: " << tokens.at(0) << " lt: " << tokens.back() <<'\n';
        cout << '\n';
        */
        
        // [DEBUG] -> Check whether instruction type detection is working
        /*
        cout << lineBuffer << '\n';
        cout << "instruction type: ";
        assembleLine(tokens);
        */

       // Check instruction type, then pass to create xType instruction function.
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

void printMachineCode(){

}

int main(){
    scanFile();
    debug_ASMBuffer();
    //assembleBuffer();
    debugMkInst();
    return 0;
}