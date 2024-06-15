#include <iostream>
#include "circuit.h"

using std::cout, std::cin, std::endl, std::string;

int main(int argc, char const *argv[])
{
    const char* bigCircuit = "I1 8 1 0\n"
    "I2 3 1 2\n"
    "I3 25 0 3\n"
    "V1 22 3 2\n"
    "R1 1 2 0\n"
    "R2 3 2 1\n"
    "R3 4 3 1\n"
    "R4 5 3 0";
    
    const char* smallCircuit = "V1 8 1 0\n"
    "R1 4 1 0";
    
    Circuit myCircuit(smallCircuit);
    
    cout << myCircuit.textSolution() << endl;
    

    return 0;
}
