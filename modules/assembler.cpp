// Assembles MIPS ASL to Machine Code

#include "libraries.h"
#include "parser.cpp"

using namespace std;

// MIPS Instruction Field
/*
 
 < R-type >
  31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
 +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 |    op(6bits)    |   rs(5bits)  |   rt(5bits)  |   rd(5bits)  | shamt(5bits) |   fucnt(6bits)  |
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
    op = "000000";
    string instr = typeR_map.find(funct)->first;
    string r1 = registerMap.find(rs)->second;
    string r2 = registerMap.find(rt)->second;
    string rd = registerMap.find(rd)->second;
    string sa = std::bitset<5>(shamt).to_string();
    if (r1.empty()) r1 = "00000";
    if (r2.empty()) r2 = "00000";
    if (rd.empty()) rd = "00000";
    if (instr == "clo" || instr == "clz" || instr == "mul" || instr == "madd" ||
        instr == "maddu" || instr == "msub" || instr == "msubu") op = "011100";
    return op + r1 + r2 + rd + sa + funct;
};

string mk_typeI(string op, string rs, string rt, string addr){
    string r1 = registerMap.find(rs)->second;
    string rd = registerMap.find(rt)->second;
    string imm = std::bitset<16>(addr).to_string();
    if (r1.empty()) r1 = "00000";
    if (rd.empty()) rd = "00000";
    return op + r1 + rd + imm;
};

string mk_typeJ(string op, string addr){
    string addr = bitset<26>(addr).to_string();
    return op + addr;
};


// Assemble tokens into machine code
string assembleLine(vector<string> tokens){
    string instrType;
    int opcode;
    string MIPS_binary;
    string instr = tokens.at(0);

    if (typeR_map.count(instr)){
        instrType = "R";
        opcode = stoi(typeR_map.find(instr)->second);
        //machineCode = mk_typeR("", tokens[1], tokens[2], tokens[3], tokensp3, opcode);
    }
    if (typeI_map.count(instr)){
        instrType = "I";
        opcode = stoi(typeI_map.find(instr)->second);
        //machineCode = mk_typeI();
    }
    if (typeJ_map.count(instr)){
        instrType = "J";
        opcode = stoi(typeJ_map.find(instr)->second);
        //machineCode = mk_typeJ();
    }

    switch (opcode) {
        case 100000: // add
            MIPS_binary = mk_typeR("", tokens[1], tokens[2], tokens[3], "", instr);
        
        case 100001: // addu
            MIPS_binary = mk_typeR("", tokens[1], tokens[2], tokens[3], "", instr);

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

}



int main(){
    scanFile();
    //debug_ASMBuffer();
    assembleBuffer();
    return 0;
}