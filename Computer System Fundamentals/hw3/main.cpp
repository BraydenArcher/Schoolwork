#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Cache.h"
#include <cmath>

using std::string;
using std::vector;
using std::istream;
using std::ifstream;
using std::cout;
using std::endl;
using std::stoi;
using std::cin;


bool isPowerOfTwo(int n) //Returns true if a power of two, otherwise false
{
    return (n != 0) && (ceil(log2(n)) == floor(log2(n))); //If the ceil and floor dont equal each other, the log 2 is not whole and therefore not a power of two. 0 is special case
}


int main(int argc, char** argv) {
    if (argc < 7) { //argument handling
        cout << "Too few arguments" << endl;
        return 1;
    } else if(((string) argv[4] != "write-allocate" && (string) argv[4] != "no-write-allocate") || ((string) argv[5] != "write-back" && (string) argv[5] != "write-through")) {
        cout << "Invalid Options" << endl;
        return 1;
    } else if (argc > 7) {
        cout << "Too many arguments" << endl;
        return 1;
    } else if(stoi(argv[1]) < 0 || stoi(argv[2]) < 0 || stoi(argv[3]) < 4) {
        cout << "Numbers too low" << endl;
        return 1;
    } else if(!isPowerOfTwo((stoi(argv[1]))) || !isPowerOfTwo(stoi(argv[2])) || !isPowerOfTwo(stoi(argv[3]))) {
        cout << "Invalid Arguments" << endl;
        return 1;
    } else if((string) (argv[6]) != "lru" && (string) (argv[6]) != "fifo") {
        cout << "Not a valid cache organization" << endl;
        return 1;
    } else {
        int sets = stoi(argv[1]); //Takes arguments and converts them to numbers in respective variables
        int blocks = stoi(argv[2]);
        int bytes = stoi(argv[3]);
        string allocate = argv[4];
        bool writeAllocate = allocate == "write-allocate"; //Creates a bool for allocate type
        string write = argv[5];
        bool writeThrough = write == "write-through"; //Creates a bool for write type
        string isLRU = argv[6];
        bool type = (string) (argv[6]) == "lru"; //Creates a bool for cache organization

        vector<char> processorStatus; //vectors to store processor status and traces
        vector<string> memoryAddress;

        string temp;
        while (cin >> temp) {
            processorStatus.emplace_back(temp[0]); //Add processor status
            cin >> temp;
            memoryAddress.emplace_back(temp.substr(2, temp.length())); //Add memory address
            cin >> temp; //Ignore third field
        }
        Cache LRU(sets, blocks, bytes, writeAllocate, writeThrough, type, processorStatus, memoryAddress); //Create a new cache and feed relevant data
        LRU.simulate(); //Run simulation
        LRU.print(); //Print results
    }
}
