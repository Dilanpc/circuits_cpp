#include "circuit.h"

// CIRCUIT
Circuit::Circuit(string text)
{
    vector<string> componentsTxt = separeComponentsTxt(text);
    for (string componentTxt : componentsTxt)
    {
        createComponent(componentTxt);
    }
    
}

vector<string> Circuit::separeComponentsTxt(string text)
{
    vector<string> componentsTxt;
    string word; 
    for (char c : text)
    {
        if (c == '\n'){
            componentsTxt.push_back(word);
            word = "";
        }else{
            word += c;
        }
    }
    componentsTxt.push_back(word);
    return componentsTxt;
}

Node* Circuit::createNode(int id)
{
    for (Node* node : nodes)
    {
        if (node->id == id)
        {
            return node;
        }
    }
    nodes.push_back(new Node(id));
    return nodes.back();
}

void Circuit::createComponent(string text)
{
    vector<string> words;
    string word;
    for (char c : text)
    {
        if (c == ' '){
            words.push_back(word);
            word = "";
        }else{
            word += c;
        }
        
    }
    words.push_back(word);

    // find type and id
    char type;
    int id;
    for (int i=0; i<words[0].size(); i++)
    {
        if (isalpha(words[0][i]))
        {
            type = words[0][i];
            id = stoi(words[0].substr(i+1));
            break;
        }

    }
    Component* component = new Component(
        type,
        id,
        stof(words[1]), // Value
        createNode(stoi(words[2])), // Node 1
        createNode(stoi(words[3]))  // Node 2
    );
    components.push_back(component);
}



std::ostream& operator<<(std::ostream& os, const Circuit& circuit)
{
    os << "Circuit: " << std::endl;
    for (Component* component : circuit.components)
    {
        os << "Component: " << component->type << component->id << std::endl;
        os << "Value: " << component->value << std::endl;
        os << "Node 1: " << component->node1->id << std::endl;
        os << "Node 2: " << component->node2->id << std::endl;
    }
    return os;
}


// COMPONENT

Component::Component(const char type, int id, const float value, Node* node1, Node* node2)
{
    this->type = type;
    this->id = id;
    this->value = value;
    this->node1 = node1;
    this->node2 = node2;
}



// NODE
Node::Node(int id)
{
    this->id = id;
}