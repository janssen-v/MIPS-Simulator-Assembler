// Simulator

#include "assembler_module.cpp"

using namespace std;

struct InstructionField{
    int op;
    int rs;
    int rt;
    int shamt;
    int funct;
};

struct Simulator{
    // Main Memory
    vector<int32_t> ram;
    uint32_t pc;
    uint32_t npc;
    
    // Processor Register
    vector <int32_t> reg;
    int32_t Hi;
    int32_t Lo;
};

class Inst{
    private:

        // ASM Program Data
        int InstructionCount;
        int ProgramCounter;
        int ProgramLength;
        bool HaltStatus;

        /*
        // REGISTERS
        $zero; sim.reg[0]
        $at;   sim.reg[1]
        $v0;   sim.reg[2]
        $v1;   sim.reg[3]
        $a0;   sim.reg[4]
        $a1;   sim.reg[5]
        $a2;   sim.reg[6]
        $a3;   sim.reg[7]
        $t0;   sim.reg[8]
        $t1;   sim.reg[9]
        $t2;   sim.reg[10]
        $t3;   sim.reg[11]
        $t4;   sim.reg[12]
        $t5;   sim.reg[13]
        $t6;   sim.reg[14]
        $t7;   sim.reg[15]
        $s0;   sim.reg[16]
        $s1;   sim.reg[17]
        $s2;   sim.reg[18]
        $s3;   sim.reg[19]
        $s4;   sim.reg[20]
        $s5;   sim.reg[21]
        $s6;   sim.reg[22]
        $s7;   sim.reg[23]
        $t8;   sim.reg[24]
        $t9;   sim.reg[25]
        $k0;   sim.reg[26]
        $k1;   sim.reg[27]
        $gp;   sim.reg[28]
        $sp;   sim.reg[29]
        $fp;   sim.reg[30]
        $ra;   sim.reg[31]      
        */  
};




int main(int argc, char *argv[]){
    // Assembler Module
    if(argc != 2){ // If no file passed to program
        scanFile();
    }
    else{
        scanFile(argv[1]);
    }
    
    assembleBuffer(); // Assembles MIPS ASM and stores to vector instrStore    
    
    // Simulator Module
    
    // Initialize Simulation
    Simulator sim;
    sim.ram.resize(0x5B8D80);    // 6MB of RAM initialised
    sim.pc = 0x0400000;          // Start of text segment
    sim.npc = 0x0400000+4;       // Next instruction pointer (increments by 4)
    sim.reg.resize(32,0);        // 32 Registers
    sim.Hi = 0;
    sim.Lo = 0;

    // Load Text Section
    int instEnd = 0;

    for (int i = 0; i < int(instrStore.size()); i++){
        int ramBeg = 0x0400000;
        int ramPtr = (i*4);
        int ramAddr = ramBeg + ramPtr;
        sim.ram[ramAddr] = stoi(instrStore[i]);
        instEnd = ramAddr;
    }

    // Load Data Section
    for (int j = 0; j < int(DataLabelTable.size()); j++){
        int ramBeg = 0x0500000;
        int ramPtr = (j*4);
        sim.ram[ramBeg + ramPtr] = stoi(DataLabelTable[j].label);
    }

    // Simulation cycle || Fetch, Decode, Execute

    while (int(sim.pc) != instEnd){

        // Fetch instruction from memory
        int32_t mCodeBuffer = sim.ram[sim.pc];

        // Instruction Decode
        string instrBuffer = std::bitset<32>(mCodeBuffer).to_string();
        cout << instrBuffer << '\n';
        
        // Execute Instruction
        
        sim.pc += 4;
    }
    
    return 0;
}

//when data load into memory from asmstore, need to check type of data before loading memory to allocate blocks