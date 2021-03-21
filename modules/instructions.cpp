#include "libraries.h"
using namespace std;


// Instruction Opcode Map
map<string, string> typeR_map{
    {"add"    , "100000"},
    {"addu"   , "100001"},
    {"and"    , "100100"},
    {"div"    , "011010"},
    {"divu"   , "011011"},
    {"jalr"   , "001001"},
    {"jr"     , "001000"},
    {"mfhi"   , "010000"},
    {"mflo"   , "010010"},
    {"mthi"   , "010001"},
    {"mtlo"   , "010011"},
    {"mult"   , "011000"},
    {"multu"  , "011001"},
    {"nor"    , "100111"},
    {"or"     , "100101"},
    {"sll"    , "000000"},
    {"sllv"   , "000100"},
    {"slt"    , "101010"},
    {"sltu"   , "101011"},
    {"sra"    , "000011"},
    {"srav"   , "000111"},
    {"srl"    , "000010"},
    {"srlv"   , "000110"},
    {"sub"    , "100010"},
    {"subu"   , "100011"},
    {"syscall", "001100"},
    {"xor"    , "100110"},
    {"clo"    , "100001"},
    {"clz"    , "100000"},
    {"mul"    , "000010"},
    {"madd"   , "000000"},
    {"maddu"  , "000001"},
    {"msub"   , "000100"},
    {"msubu"  , "000101"},
    {"tlt"    , "110010"},
    {"tltu"   , "110011"},
    {"tge"    , "110000"},
    {"tgeu"   , "110001"},
    {"tne"    , "110110"},
    {"teq"    , "110100"}
};
map<string, string> typeI_map{
    {"addi"   , "001000"},
    {"addiu"  , "001001"},
    {"andi"   , "001100"},
    {"beq"    , "000100"},
    {"bgez"   , "000001"},
    {"bgezal" , "000001"},
    {"bgtz"   , "000111"},
    {"blez"   , "000110"},
    {"bltz"   , "000001"},
    {"bne"    , "000101"},
    {"lb"     , "100000"},
    {"lbu"    , "100100"},
    {"lh"     , "100001"},
    {"lhu"    , "100101"},
    {"lui"    , "001111"},
    {"lw"     , "100011"},
    {"ori"    , "001101"},
    {"sb"     , "101000"},
    {"slti"   , "001010"},
    {"sc"     , "111000"},
    {"sltiu"  , "001011"},
    {"sh"     , "101001"},
    {"sw"     , "101011"},
    {"xori"   , "001110"},
    {"tlti"   , "000001"},
    {"bltzal" , "000001"},
    {"swr"    , "101110"},
    {"swl"    , "101010"},
    {"ll"     , "110000"},
    {"tltiu"  , "000001"},
    {"lwr"    , "100110"},
    {"lwl"    , "100010"},
    {"tgei"   , "000001"},
    {"tgeiu"  , "000001"},
    {"tnei"   , "000001"},
    {"teqi"   , "000001"}
};
map<string, string> typeJ_map {
    {"j"      , "000010"},
    {"jal"    , "000011"}
};