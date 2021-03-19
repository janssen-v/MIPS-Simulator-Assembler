// Parser to store labels, addresses, memory names and values from the MIPS file

#include "parser.h"

using namespace std;

// Global Variables
vector<string> ASMBuffer;
vector<MemoryLabel> LabelTable;
int dataSection;
int textSection;

bool mainExists;
int mainIndex;      // Program counter should be set to main index if it exists

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
            lineBuffer = (lineBuffer.substr(0,pos));    // Splits string at position of # to remove comments
        }

        if (lineBuffer.find('main')){
            mainExists = 1;
            mainIndex = lineCounter; // Where the instructions under main start (if using new implementation)
        }
        
        if (lineBuffer.find(':')){     // Stores where instructions under label start (if using new implementation of ASMBuffer.push_back())
            MemoryLabel tempLabel;
            tempLabel.address = lineCounter;
            tempLabel.label = lineBuffer;
            LabelTable.push_back(tempLabel);
        }

        if (lineBuffer == ".data"){     // Marks location of data and text section header in ASMBuffer vector
            dataSection = lineCounter;  // In new implementation, marks first instruction in section.
        }
        if (lineBuffer == ".text"){    
            textSection = lineCounter;
        }

        /* Old implementation that also stores section headers and labels (.data, .text, etc.)
        if (lineBuffer.size()>0){
            ASMBuffer.push_back(lineBuffer); // Assembly Buffer
            lineCounter += 1;
        }*/

        if ((lineBuffer.size() > 0) && !(lineBuffer.find(':')) && !(lineBuffer.find('.data')) && !(lineBuffer.find('.text')))  {
            ASMBuffer.push_back(lineBuffer); // If not a label or section header (.data or .text), stores instruction buffer
            lineCounter += 1;
        }
    }
}

void scanInstrType(){
    vector<string>::iterator ptr = ASMBuffer.begin();
    advance(ptr, textSection); // Advances pointer to first instruction under .text
    while (ptr != ASMBuffer.end()){
        // line by line (insert scan function here)
        ++ptr;
    }
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

int main(){
    scanFile();
    debug();
    return 0;
}