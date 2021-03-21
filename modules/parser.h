#include "libraries.h"
#include "instructions.cpp"

// External Maps
extern map<string, string> typeR_map;
extern map<string, string> typeI_map;
extern map<string, string> typeJ_map;

struct MemoryLabel{
    string label;
    int32_t address;
};