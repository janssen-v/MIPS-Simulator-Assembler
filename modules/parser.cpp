// Parser to store labels, addresses, memory names and values from the MIPS file

#include "parser.h"

using namespace std;

// Global Variables
vector<string> ASMBuffer;
vector<MemoryLabel> LabelTable;
int dataSection;
int textSection;

// Structures
struct MemoryLabel{
    string label;
    int32_t address;
};

// Functions
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
        string::size_type pos = lineBuffer.find('#');
        
        if (pos != string::npos){
            lineBuffer = (lineBuffer.substr(0,pos));
        }
        if (lineBuffer == ".data"){     // Marks data and text sections in vector
            dataSection = lineCounter;
        }
        if (lineBuffer == ".text"){    
            textSection = lineCounter;
        }
        if (lineBuffer.size() > 0){
            ASMBuffer.push_back(lineBuffer);
            lineCounter += 1;
        }
    }
}

void scanInstrType(){

}

void debug(){ // Check if buffer works properly
    while (!(ASMBuffer.empty())){
        cout << ASMBuffer.front() << '\n';
        ASMBuffer.erase(ASMBuffer.begin());
        cout << '\n';
    }
    cout << "Data Section is in ASMBuffer[" << dataSection << "] \n";
    cout << "Text Section is in ASMBuffer[" << textSection << "] \n"; 
}

int main(){ // Takes input directly from command line
    scanFile();
    debug();
    return 0;
}