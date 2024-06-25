#include <iostream>
#include <fstream>
#include <sstream>
#include "circuit.h"

using std::cout, std::cin, std::endl, std::string;

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

const char* dependentCircuit = "V1 8 1 0\n"
    "R1 4 1 2\n"
    "VCVS1 2V1 2 0";


int main(int argc, char const *argv[])
{
    Circuit* myCircuit{};
    if (argc == 2) {
        std::ifstream file(argv[1]);
        if (file.is_open()) {
            std::stringstream buffer;
            buffer << file.rdbuf();

            myCircuit = new Circuit(buffer.str());
            file.close();
        }
    }
	else {
		myCircuit = new Circuit(bigCircuit);
	}

    cout << myCircuit->textSolution() << endl;

}
