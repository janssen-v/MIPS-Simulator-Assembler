// Assembles MIPS ASL to Machine Code

#include "libraries.h"
#include "parser.cpp"

using namespace std;

void mk_typeR(int op, int rs, int rt, int rd, int shamt, int funct);
void mk_typeI(int op, int rs, int rt, int addr);
void mk_typeJ(int op, int addr);

//void assembleLine(vector<string> tokens){}


// Assemble instruction buffer line by line
void assembleBuffer(){
    string lineBuffer;
    string opCode;
    
    for (int ptr = textSection_header+1; ptr< ASMStore.size(); ptr++){ // Iterates line by line starting from .text section
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

        // Check instruction type, then pass to create xType instruction function.
        
        /*
        void mk_typeR(int op, int rs, int rt, int rd, int shamt, int funct);
        void mk_typeI(int op, int rs, int rt, int addr);
        void mk_typeJ(int op, int addr);
        */
    
    }
}



int main(){
    scanFile();
    debug_ASMBuffer();
    //assembleBuffer();
    return 0;
}