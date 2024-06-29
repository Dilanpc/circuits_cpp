#ifndef CIRCUIT_H
#define CIRCUIT_H

#include <iostream>
#include <vector>
#include <string>
#include "linearAlgebra.h"


class Node;
class Component;


class Circuit
{
public:
    Circuit(std::string text);
    ~Circuit();

    std::vector<Node*> nodes;
    std::vector<Component*> components;

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

    std::string textSolution();

protected:

private:
    std::vector<std::string> separeComponentsTxt(std::string text);
    void createComponent(std::string text);
    Node* createNode(int id);
    void readDependentSource(std::vector<std::string> words);
    int findComponent(const std::string referenceName) const;

};



class Node
{
public:
    Node(int id);

    std::vector<Component*> components;
    int id;
};



class Component
{
public:
    Component(const std::string type, const int id, const float value, Node* node1, Node* node2, std::string referenceName = "");

    const std::string type;
    const int id;
    const float value;
    Node* node1;
    Node* node2;
    Node* pos;
    Node* neg;
    std::string referenceName; // For dependent sources
    
};



#endif
