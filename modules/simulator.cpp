// Simulator

#include "libraries.h"

using namespace std;

struct InstructionField{
    int op;
    int rs;
    int rt;
    int shamt;
    int funct;
};

struct MemoryLabel{
    string label;
    uint32_t address;    // Address that corresponds to label
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
        

        // Instruction Set
        void mips_add();
        void mips_addu();
        void mips_addi();
        void mips_addiu();
        void mips_and();
        void mips_andi();
        void mips_clo();
        void mips_clz();
        void mips_div();
        void mips_divu();
        void mips_mult();
        void mips_mul();
        void mips_madd();
        void mips_msub();
        void mips_maddu();
        void mips_msubu();
        void mips_nor();
        void mips_or();
        void mips_ori();
        void mips_sll();
        void mips_sllv();
        void mips_sra();
        void mips_srav();
        void mips_srl();
        void mips_srlv();
        void mips_sub();
        void mips_subu();
        void mips_xor();
        void mips_xori();
        void mips_lui();
        void mips_slt();
        void mips_sltu();
        void mips_slti();
        void mips_sltiu();
        void mips_beq();
        void mips_bgez();
        void mips_bgezal();
        void mips_bgtz();
        void mips_blez();
        void mips_bltzal();
        void mips_bltz();
        void mips_bne();
        void mips_j();
        void mips_jal();
        void mips_jalr();
        void mips_jr();
        void mips_teq();
        void mips_teqi();
        void mips_tne();
        void mips_tnei();
        void mips_tge();
        void mips_tgeu();
        void mips_tgei();
        void mips_tgeiu();
        void mips_tlt();
        void mips_tltu();
        void mips_tlti();
        void mips_tltiu();
        void mips_lb();
        void mips_lbu();
        void mips_lh();
        void mips_lhu();
        void mips_lw();
        void mips_lwcl();
        void mips_lwl();
        void mips_lwr();
        void mips_ll();
        void mips_sb();
        void mips_sh();
        void mips_sw();
        void mips_swl();
        void mips_swr();
        void mips_sc();
        void mips_mfhi();
        void mips_mflo();
        void mips_mthi();
        void mips_mtlo();
        void mips_syscall();

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
// Initialize Simulator
void simInit(Simulator& sim){
    sim.ram.resize(0x5B8D80);   // 6MB of RAM initialised
    sim.pc = 0x0400000;          // Start of text segment
    sim.npc = 0x0400000+4;       // Next instruction pointer (increments by 4)
    sim.reg.resize(32,0);       // 32 Registers
    sim.Hi = 0;
    sim.Lo = 0;
}

int main(int argc, char *argv[]){
    Simulator simulation;
    simInit(simulation);
    return 0;
}

//when data load into memory from asmstore, need to check type of data before loading memory to allocate blocks