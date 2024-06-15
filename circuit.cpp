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

Circuit::~Circuit()
{
    for (Node* node : nodes)
    {
        delete node;
    }
    for (Component* component : components)
    {
        delete component;
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


Matrix Circuit::getIncidenceMatrix()
{
    incidenceMatrix =  Matrix(nodes.size()-1, components.size());
    int fixIndex = 0;
    for (int i=0; i<nodes.size(); i++)
    {
        if (nodes[i]->id == 0){
            fixIndex = 1;
            continue;
        }
        for (int j=0; j<components.size(); j++)
        {
            if (components[j]->pos == nodes[i]){
                incidenceMatrix[i-fixIndex][j] = 1;
            }else if (components[j]->neg == nodes[i]){
                incidenceMatrix[i-fixIndex][j] = -1;
            }
        }
    }
    
    return incidenceMatrix;
}


Matrix Circuit::getVoltageMatrix()
{
    voltageMatrix = Matrix(components.size(), components.size());
    Matrix identity(components.size(), components.size());
    identity.identity();
    voltageMatrix.stack(identity, false);
    voltageMatrix.stack(incidenceMatrix.transpose(), false);

    return voltageMatrix;
}



Matrix Circuit::getSourcesVector()
{
    int start = nodes.size() - 1 + components.size();
    int rows = start + components.size();
    Matrix sources(rows, 1);

    for (int i=0; i<components.size(); i++)
    {
        if (components[i]->type == 'V'){
            sources[start+i][0] = -components[i]->value;
        }
        else if (components[i]->type == 'I'){
            sources[start+i][0] = components[i]->value;
        }
    }
    sourcesVector = sources;
    return sources;
}



Matrix Circuit::getBranchesMatrix()
{
    Matrix incidences(components.size(), components.size());
    Matrix voltages(components.size(), components.size());

    for (int i=0; i<components.size(); i++)
    {
        if (components[i]->type == 'V'){
            voltages[i][i] = 1;
        }
        else if (components[i]->type == 'R'){
            incidences[i][i] = 1;
            voltages[i][i] = -1 / components[i]->value;
        }
        else if (components[i]->type == 'I'){
            incidences[i][i] = 1;
        }
    }
    componentsMatrix = incidences;
    Matrix zeros(components.size(), nodes.size()-1);
    componentsMatrix.stack(voltages, false);
    componentsMatrix.stack(zeros, false);

    return componentsMatrix;
}


Matrix Circuit::solve()
{
    getIncidenceMatrix();
    getVoltageMatrix();
    getSourcesVector();
    getBranchesMatrix();

    incidenceMatrix.stack(Matrix(nodes.size()-1, components.size()+nodes.size()-1), false);
    

    Matrix fullMatrix = incidenceMatrix;
    fullMatrix.stack(voltageMatrix, true);
    fullMatrix.stack(componentsMatrix, true);

    std::cout << fullMatrix << std::endl;
    std::cout << "Sources:" << std::endl;
    std::cout << sourcesVector << std::endl;

    return fullMatrix.solve(sourcesVector);

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

    this->pos = node1;
    this->neg = node2;
}



// NODE
Node::Node(int id)
{
    this->id = id;
}