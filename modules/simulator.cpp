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

struct Register{        // Register Structure
    int32_t value;
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

        // Registers
        Register $zero;
        Register $at;
        Register $v0;
        Register $v1;
        Register $a0;
        Register $a1;
        Register $a2;
        Register $a3;
        Register $t0;
        Register $t1;
        Register $t2;
        Register $t3;
        Register $t4;
        Register $t5;
        Register $t6;
        Register $t7;
        Register $s0;
        Register $s1;
        Register $s2;
        Register $s3;
        Register $s4;
        Register $s5;
        Register $s6;
        Register $s7;
        Register $t8;
        Register $t9;
        Register $k0;
        Register $k1;
        Register $gp;
        Register $sp;
        Register $fp;
        Register $ra;        
};
// Initialize Simulator
void simInit(Simulator& sim){
    sim.ram.resize(0x5B8D80); // 6MB of RAM initialised
    sim.pc = 0x10000000;      // Start of text segment
    sim.npc = 0x10000000+4;   // Next instruction pointer (increments by 4)
    sim.reg.resize(32,0);     // 32 Registers
    sim.Hi = 0;
    sim.Lo = 0;
}
    



int main(){
    Simulator simulation;
    simInit(simulation);
    return 0;
}