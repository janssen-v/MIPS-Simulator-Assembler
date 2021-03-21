// Parser to store labels, addresses, memory names and values from the MIPS file
#include "parser.h"

using namespace std;

// Global Variables
vector<string> ASMStore;

int dataSection_header;
int textSection_header;

bool mainExists;
int mainIndex;      // Program counter should be set to main index if it exists

vector<MemoryLabel> LabelTable; // TODO move back to global variables after declaring in parser.h

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
        string::size_type pos = lineBuffer.find("#");
        if (pos != string::npos){
            lineBuffer = (lineBuffer.substr(0,pos));    // Splits string at position of # to remove comments
        }

        if (lineBuffer.find("main:")){
            mainExists = 1;
            mainIndex = lineCounter+1;    // Where the instructions under main start, 
        }
        
        if (lineBuffer.find(":")){        // Stores location of function label
            MemoryLabel tempLabel;
            tempLabel.address = lineCounter;
            tempLabel.label = lineBuffer;
            LabelTable.push_back(tempLabel);
        }

        if (lineBuffer == ".data"){             // Marks location of data and text section header in ASMStore
            dataSection_header = lineCounter; 
        }
        if (lineBuffer == ".text"){    
            textSection_header = lineCounter;   // TROUBLESHOOT: not working somehow
        }

        if (lineBuffer.size()>0){
            ASMStore.push_back(lineBuffer); // Assembly Buffer
            lineCounter += 1;
        }
    }
}

void debug_ASMBuffer(){ // Check if buffer works properly
    while (!(ASMStore.empty())){
        cout << ASMStore.front() << '\n';
        ASMStore.erase(ASMStore.begin());
    }
    cout << "Data Section is in ASMStore[" << dataSection_header << "] \n";
    cout << "Text Section is in ASMStore[" << textSection_header << "] \n"; 
}