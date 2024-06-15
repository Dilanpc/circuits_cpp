#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include "linearAlgebra.h"


using std::vector, std::string;

class Node;
class Component;


class Circuit
{
public:
    Circuit(string text);

    vector<Node*> nodes;
    vector<Component*> components;

    Matrix solve();

    friend std::ostream& operator<<(std::ostream& os, const Circuit& circuit);

public: // This method will be private after testing
    Matrix incidenceMatrix;
    Matrix voltageMatrix;
    Matrix sourcesVector;
    Matrix componentsMatrix;

    Matrix getIncidenceMatrix();
    Matrix getVoltageMatrix();
    Matrix getSourcesVector();
    Matrix getBranchesMatrix();

protected:

private:
    vector<string> separeComponentsTxt(string text);
    void createComponent(string text);
    Node* createNode(int id);

};



class Node
{
public:
    Node(int id);

    vector<Component*> components;
    int id;
};



class Component
{
public:
    Component(char type, int id, float value, Node* node1, Node* node2);

    char type;
    int id;
    float value;
    Node* node1;
    Node* node2;
    Node* pos;
    Node* neg;
    
};