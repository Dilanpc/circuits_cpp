#include "circuit.h"

// CIRCUIT
Circuit::Circuit(std::string text)
{
    std::vector<std::string> componentsTxt = separeComponentsTxt(text);
    for (std::string componentTxt : componentsTxt)
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

std::vector<std::string> Circuit::separeComponentsTxt(std::string text)
{
    std::vector<std::string> componentsTxt;
    std::string word;
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

void Circuit::createComponent(std::string text)
{
    std::vector<std::string> words;
    std::string word = "";
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
    std::string type="";
    int id;

    if (isalpha(words[0][0]) && !isalpha(words[0][1]))
    {
        type = words[0][0];
        id = stoi(words[0].substr(1));
    }
    else // Depending source
    {
        readDependentSource(words);
        return;
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

void Circuit::readDependentSource(std::vector<std::string> words)
{
    std::string type = words[0].substr(0,4);
    int id = stoi(words[0].substr(4));

    float value = 0;
    std::string referenceName = "";
    std::string valueTxt = "";
    for (int i=0; i<words[1].size(); i++) // Seapare value and reference
	{
        if (isdigit(words[1][i]) || words[1][i] == '.')
		    valueTxt += words[1][i];
        else
        {
            value = stof(valueTxt);
            referenceName = words[1].substr(i);
            break;
        }
	}
    Component* component = new Component(
		type,
		id,
		value,
		createNode(stoi(words[2])),
		createNode(stoi(words[3])),
		referenceName
	);
	components.push_back(component);
}

int Circuit::findComponent(const std::string referenceName) const
{
	for (int i=0; i<components.size(); i++)
	{
        std::string name = components[i]->type + std::to_string(components[i]->id);
		if (name == referenceName)
		{
			return i;
		}
	}
	return -1;
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
            }
            else if (components[j]->neg == nodes[i]){
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
    voltageMatrix.stack(-1*incidenceMatrix.transpose(), false);

    return voltageMatrix;
}



Matrix Circuit::getSourcesVector()
{
    int start = nodes.size() - 1 + components.size();
    int rows = start + components.size();
    Matrix sources(rows, 1);

    for (int i=0; i<components.size(); i++)
    {
        if (components[i]->type == "V" || components[i]->type == "I"){
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
        if (components[i]->type == "V") {
            voltages[i][i] = 1;
        }
        else if (components[i]->type == "R") {
            incidences[i][i] = 1;
            voltages[i][i] = -1 / components[i]->value;
        }
        else if (components[i]->type == "I") {
            incidences[i][i] = 1;
        }
        else if (components[i]->type == "VCVS") {
            int reference = findComponent(components[i]->referenceName);
            voltages[i][i] = 1;
            voltages[i][reference] = -components[i]->value;
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

    solution = fullMatrix.solve(sourcesVector);
    return solution;
}



std::string Circuit::textSolution()
{
    if (solution.size() == 0)
    {
        solve();
    }
    std::string text = "";
    int index = 0;
    for (auto component : components)
    {
        text += component->type + std::to_string(component->id) + " current: " + std::to_string(round(solution[index][0], 3)) + "\n";
        index++;
    }
    for (auto component : components)
    {
        text += component->type + std::to_string(component->id) + " voltage: " + std::to_string(round(solution[index][0], 3)) + "\n";
        index++;
    }
    for (auto node : nodes)
    {
        if (node->id == 0)
        {
            continue;
        }
        text += "Node " + std::to_string(node->id) + " voltage: " + std::to_string(round(solution[index][0], 3)) + "\n";
        index++;
    }
    return text;
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

Component::Component(const std::string type, const int id, const float value, Node* node1, Node* node2, std::string referenceName)
    : type(type), id(id), value(value),
    node1(node1), node2(node2), referenceName(referenceName),
    pos(node1), neg(node2)
{

}



// NODE
Node::Node(int id)
{
    this->id = id;
}