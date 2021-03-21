// Parser to store labels, addresses, memory names and values from the MIPS file
#include "parser.h"

using namespace std;

// Global Variables
vector<string> ASMStore;
vector<MemoryLabel> TextLabelTable;
vector<MemoryLabel> DataLabelTable;

int dataSection;
int textSection;

int mainExists = 0;
int mainIndex;      // Program counter should be set to main index if it exists
int inTextSection = 0;
int isLabelHeader = 0;


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
        isLabelHeader = 0;

        string::size_type pos = lineBuffer.find("#");
        if (pos != string::npos){
            lineBuffer = (lineBuffer.substr(0,pos));    // Splits string at position of # to remove comments
        }

        if (lineBuffer == ".data"){       // Marks first instruction of data and text section in ASMStore
            isLabelHeader = 1;
            inTextSection = 0;
            dataSection = lineCounter;  
        }
        if (lineBuffer == ".text"){ 
            isLabelHeader = 1;  
            inTextSection = 1; 
            textSection = lineCounter;
        }

        if (lineBuffer.find("main") != string::npos){
            mainExists = 1;
            mainIndex = lineCounter;    // Where the instructions under main start
        }
        
        if (lineBuffer.find(":") != string::npos){        // Stores location of function label
            isLabelHeader = 1;
            MemoryLabel tempLabel;
            tempLabel.address = lineCounter;
            tempLabel.label = lineBuffer;
            if (inTextSection == 1){
                TextLabelTable.push_back(tempLabel);
            }
            else DataLabelTable.push_back(tempLabel);
            
        }

        if (lineBuffer.size()>0 && isLabelHeader != 1) {
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
    cout << mainExists << " Main exists \n";
    cout << mainIndex << " Main index \n";
    cout << "First element of Textlabel table: " << TextLabelTable.front().label << "\n" ;
    cout << " First element of Datalabel table: " << DataLabelTable.front().label << "\n";
}

/* [DEBUG] -> to check if parser module works
int main(){
    scanFile();
    debug_ASMBuffer();
}
*/