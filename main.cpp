#include <iostream>
#include "circuit.cpp"

using std::cout, std::cin, std::endl, std::string;

int main(int argc, char const *argv[])
{
    const char* txt = "V1 5 1 0\n"
    "R1 2 1 0";
    
    Circuit myCircuit(txt);

    cout << myCircuit << endl;
    cout << myCircuit.solve() << endl;

    return 0;
}
