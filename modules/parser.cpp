// Parser to store labels, addresses, memory names and values from the MIPS file
#include "parser.h"

using namespace std;

// Global Variables
vector<string> ASMStore;
vector<MemoryLabel> LabelTable;

int dataSection;
int textSection;

bool mainExists;
int mainIndex;      // Program counter should be set to main index if it exists


// Functions
void scanFile(string inputFileName = ""){
    string lineBuffer;
    ifstream inputFile;
    bool inTextSection = 0;
    bool isLabelHeader = 0;

    int lineCounter = 0;

    if (inputFileName.empty()){
        cout << "Input file: ";
        cin >> inputFileName;
        inputFile.open(inputFileName.c_str(), ifstream::in);
    }
    
    while (getline(inputFile, lineBuffer)){
        isLabelHeader = 0;
        string::size_type pos = lineBuffer.find("#");
        if (pos != string::npos){
            lineBuffer = (lineBuffer.substr(0,pos));    // Splits string at position of # to remove comments
        }

        if (lineBuffer.find("main")){
            mainExists = 1;
            mainIndex = lineCounter;    // Where the instructions under main start
        }
        
        if (lineBuffer.find(":")){        // Stores location of function label
            isLabelHeader = 1;
            MemoryLabel tempLabel;
            tempLabel.address = lineCounter;
            tempLabel.label = lineBuffer;
            LabelTable.push_back(tempLabel);
        }

        if (lineBuffer == ".data"){       // Marks first instruction of data and text section in ASMStore
            dataSection = lineCounter;
            isLabelHeader = 1; 
        }
        if (lineBuffer == ".text"){    
            textSection = lineCounter;
            isLabelHeader = 1;   
            inTextSection = 1;
        }

        if (lineBuffer.size()>0 && isLabelHeader == 0){
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
    cout << "Data Section is in ASMStore[" << dataSection << "] \n";
    cout << "Text Section is in ASMStore[" << textSection << "] \n"; 
}