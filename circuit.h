#include <iostream>
#include <vector>
#include <string>
#include <cstring>

using std::vector, std::string;

class Node;
class Component;


class Circuit
{
public:
    Circuit(string text);

    vector<Node*> nodes;
    vector<Component*> components;

    friend std::ostream& operator<<(std::ostream& os, const Circuit& circuit);

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

    
};