#ifndef CIRCUIT_H
#define CIRCUIT_H

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
    ~Circuit();

    vector<Node*> nodes;
    vector<Component*> components;

    Matrix solve();

    friend std::ostream& operator<<(std::ostream& os, const Circuit& circuit);

public: // This method will be private after testing
    Matrix incidenceMatrix;
    Matrix voltageMatrix;
    Matrix sourcesVector;
    Matrix componentsMatrix;
    Matrix solution = Matrix(0, 0);

    Matrix getIncidenceMatrix();
    Matrix getVoltageMatrix();
    Matrix getSourcesVector();
    Matrix getBranchesMatrix();

    string textSolution();

protected:

private:
    vector<string> separeComponentsTxt(string text);
    void createComponent(string text);
    Node* createNode(int id);
    void readDependentSource(vector<string> words);
    int findComponent(const string referenceName) const;

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
    Component(const string type, const int id, const float value, Node* node1, Node* node2, string referenceName = "");

    const string type;
    const int id;
    const float value;
    Node* node1;
    Node* node2;
    Node* pos;
    Node* neg;
    string referenceName; // For dependent sources
    
};



#endif
