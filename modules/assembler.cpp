// Assembles MIPS ASL to Machine Code

#include "libraries.h"
#include "parser.cpp"

using namespace std;

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

int binToHex(int bin){
    return 0;
}

string mk_typeR(string op = "000000", string rs = "00000", string rt = "00000", string rd = "00000", string shamt = "00000", string funct = "000000"){
    //op = std::bitset<5>(shamt).to_string();
    string instr = typeR_map.find(funct)->first;
    rs = registerMap.find(rs)->second;
    rt = registerMap.find(rt)->second;
    rd = registerMap.find(rd)->second;
    shamt = std::bitset<5>(stoi(shamt)).to_string(); // shamt value change from int to 5 bit binary
    if (instr == "clo" || instr == "clz" || instr == "mul" || instr == "madd" ||
        instr == "maddu" || instr == "msub" || instr == "msubu") op = "011100";
    return op + rs + rt + rd + shamt + funct;
};

string mk_typeI(string op = "000000", string rs = "00000", string rt = "00000", string addr = "0000000000000000"){
    rs = registerMap.find(rs)->second;
    rt = registerMap.find(rt)->second;
    addr = std::bitset<16>(stoi(addr)).to_string();
    return op + rs + rt + addr;
};

string mk_typeJ(string op = "000000", string addr = "00000000000000000000000000"){
    addr = bitset<26>(stoi(addr)).to_string();
    return op + addr;
};


// Assemble tokens into machine code
string assembleLine(vector<string> tokens){
    string instrType;
    string MIPS_binary;
    string instr = tokens.at(0);
    string opcode;
    int opcode_int;

    if (typeR_map.count(instr)){
        instrType = "R";
        opcode = typeR_map.find(instr)->second;
        opcode_int = stoi(opcode);
        
        switch (opcode_int) { // Assemble R-Type Instructions
            
            // ADD, ADDU, AND, SUB, SUBU, OR, NOR, SLT, SLTU, XOR || CLO, CLZ
            case 100000: case 100001: case 100100: case 100010: case 100011: 
            case 100101: case 100111: case 101010: case 101011: case 100110:
    
                if (tokens[0] == "clo" || tokens[0] == "clz"){ // CLO, CLZ
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
                if (tokens[0] == "madd"){ // MADD
                    MIPS_binary = mk_typeR("011100", tokens[1], tokens[2], "00000", "00000", opcode);
                    break;
                }
                else if (tokens[0] == "mul"){ // MUL
                    MIPS_binary = mk_typeR("011100", tokens[2], tokens[3], tokens[1], "00000", opcode);
                    break;
                }
    
                else{ // SLL, SRL, SRA
                    MIPS_binary = mk_typeR("000000", "00000", tokens[2], tokens[1], tokens[3], opcode);
                    break;
                }
    
            // SLLV, SRLV, SRAV || MSUB
            case 000100: case 000110: case 000111:
                if (tokens[0] == "msub"){ // MSUB
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
        instrType = "I";
        opcode = typeI_map.find(instr)->second;
        opcode_int = stoi(opcode);
        string label;
        int32_t labelAddress;
        //machineCode = mk_typeI();
        
        switch(opcode_int){

            // BEQ, BNE
            case 000100: case 000101:
                mk_typeI(opcode, tokens[1], tokens[2], /*function to get address from labeltable)*/tokens[3]);
                break;
            
        }
    }

    if (typeJ_map.count(instr)){
        instrType = "J";
        opcode = typeJ_map.find(instr)->second;
        opcode_int = stoi(opcode);
        //machineCode = mk_typeJ();
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
    
    for (int ptr = textSection; ptr< ASMStore.size(); ptr++){ // Iterates line by line starting from .text section
        lineBuffer = ASMStore.at(ptr);
        stringstream instrStream(lineBuffer); // Tokenizes the instruction line
        istream_iterator<string> begin(instrStream);
        istream_iterator<string> end;
        vector<string> tokens(begin, end);

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

void debugMk_TypeR(){
    string TEST_MIPS_binary;
    vector<string> testToken;
    testToken.push_back("add");
    testToken.push_back("$t0");
    testToken.push_back("$t0");
    testToken.push_back("$t2");
    string testInstr = testToken[0];
    string testOpcode = typeR_map.find(testInstr)->second;
    TEST_MIPS_binary = mk_typeR("000000", testToken[2], testToken[3], testToken[1], "00000", testOpcode);
    cout << TEST_MIPS_binary <<'\n';
    cout << TEST_MIPS_binary.size() <<'\n';
}

int main(){
    //scanFile();
    //debug_ASMBuffer();
    //assembleBuffer();
    //debugMk_TypeR();
    return 0;
}