#include <cmath>
#include <cstdio>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;


// OOP Solution

class Node {
private:
    string name;
    int value;
public:
    Node() : name(""), value(0) {}

    Node(string _name) : name(_name), value(0) {}

    Node(string _name, int _value) : name(_name), value(_value) {}

    void setName(string _name) {
        name = _name;
    }

    string getName() const {
        return name;
    }

    void setValue(int _value) {
        value = _value;
    }

    int getValue() const {
        return value;
    }

    friend ostream& operator<<(ostream& output, const Node& node) {
        output << node.name << ": " << node.value;
        return output;
    }
};

class Gate {
protected:
    Node* inputA;
    Node* inputB;
    Node* outputData;

public:
    Gate() : inputA(nullptr), inputB(nullptr), outputData(nullptr) {}

    ~Gate() {
        delete inputA;
        delete inputB;
        delete outputData;
    }

    void setInputA(Node* _inputA) {
        inputA = _inputA;
    }

    Node* getInputA() const {
        return inputA;
    }

    void setInputB(Node* _inputB) {
        inputB = _inputB;
    }

    Node* getInputB() const {
        return inputB;
    }

    void setOutputNode(Node* _outputData) {
        outputData = _outputData;
    }

    Node* getOutput() const {
        return outputData;
    }

    virtual void simulateGate() = 0;
};

class AND : public Gate {
public:
    AND() : Gate() {}

    void simulateGate() {
        if (inputA->getValue() && inputB->getValue())
            outputData->setValue(1);
        else
            outputData->setValue(0);
    }
};

class NAND : public Gate {
public:
    NAND() : Gate() {}

    void simulateGate() {
        if (inputA->getValue() == 0 || inputB->getValue() == 0)
            outputData->setValue(1);
        else
            outputData->setValue(0);
    }
};

class OR : public Gate {
public:
    OR() : Gate() {}

    void simulateGate() {
        if (inputA->getValue() == 1 || inputB->getValue() == 1)
            outputData->setValue(1);
        else
            outputData->setValue(0);
    }
};

class NOR : public Gate {
public:
    NOR() : Gate() {}

    void simulateGate() {
        if (inputA->getValue() == 0 && inputB->getValue() == 0)
            outputData->setValue(1);
        else
            outputData->setValue(0);
    }
};

class XOR : public Gate {
public:
    XOR() : Gate() {}

    void simulateGate() {
        if (inputA->getValue() != inputB->getValue())
            outputData->setValue(1);
        else
            outputData->setValue(0);
    }
};

class XNOR : public Gate {
public:
    XNOR() : Gate() {}

    void simulateGate() {
        if (inputA->getValue() == inputB->getValue())
            outputData->setValue(1);
        else
            outputData->setValue(0);
    }
};

class NOT : public Gate {
public:
    NOT() : Gate() {}

    void simulateGate() {
        bool out = inputA->getValue();
        outputData->setValue(!out);
    }
};


class Simulator {
private:
    Node* nodes[1000];
    Gate* gates[1000];
    int curNodeIdx = 0;
    int curGateIdx = 0;

public:
    Simulator() : curNodeIdx(0), curGateIdx(0) {

        for (int i = 0; i < getNodeSize(); ++i) {
            nodes[i] = nullptr;
        }

        for (int i = 0; i < getGateSize(); ++i) {
            gates[i] = nullptr;
        }
    }

    virtual ~Simulator() {

        for (int i = 0; i < getNodeSize(); ++i) {
            delete[] nodes[i];
        }

        for (int i = 0; i < getGateSize(); ++i) {
            delete[] gates[i];
        }
    }

    void setNodeSize(int n) {
        curNodeIdx = n;
    }

    int getNodeSize() {
        return curNodeIdx;
    }

    void setGateSize(int n) {
        curGateIdx = n;
    }

    int getGateSize() {
        return curGateIdx;
    }

    Node* createNode(string s) {
        nodes[curNodeIdx++] = new Node(s);
        return nodes[curNodeIdx - 1];
    }

    Node* findNode(string s) {

        for (int i = 0; i < getNodeSize(); i++) {

            if (s == (nodes[i]->getName()))
                return nodes[i];
        }
        return createNode(s);
    }

    Gate* createGate(string s) {

        Gate* p = NULL;

        if (s == "AND")
            p = new AND();

        else if (s == "NAND")
            p = new NAND();

        else if (s == "OR")
            p = new OR();

        else if (s == "NOR")
            p = new NOR();

        else if (s == "XOR")
            p = new XOR();

        else if (s == "XNOR")
            p = new XNOR();

        else if (s == "NOT")
            p = new NOT();

        else
            return NULL;

        curGateIdx++;
        gates[curGateIdx - 1] = p;
        return gates[curGateIdx - 1];
    }

    void startSimulate() {

        for (int i = 0; i < getGateSize(); i++) {
            cout << i << '\n';
            gates[i]->simulateGate();
        }
    }

    void printAll() {

        for (int i = 0; i < getNodeSize(); i++)
            cout << nodes[i]->getName() << ": " << nodes[i]->getValue() << '\n';
    }

    void parseInput() {

        string Nodes;
        while (cin >> Nodes) {
            cout << Nodes << '\n';
            if (Nodes == "SET") {
                string node;
                int value;
                cin >> node >> value;
                findNode(node)->setValue(value);
            }
            else if (Nodes == "OUT") {
                string out;
                cin >> out;
                if (out == "ALL")
                    printAll();
                else
                    cout << findNode(out)->getName() << ": " << findNode(out)->getValue() << '\n';
            }
            else if (Nodes == "SIM")
                startSimulate();

            else if (Nodes == "END")
                break;

            else {
                Gate* g = createGate(Nodes);
                string node1, node2, node3;
                cin >> node1 >> node2;
                if (Nodes != "NOT") {
                    cin >> node3;
                    g->setInputA(findNode(node1));
                    g->setInputB(findNode(node2));
                    g->setOutputNode(findNode(node3));
                }
                else {
                    g->setInputA(findNode(node1));
                    g->setOutputNode(findNode(node3));
                }
            }
        }
    }
};

class GateGenerator {
private:
    vector<Node*> nodes;
    vector<Gate*> gates;
    int numOfNodes = 0;
    int numOfGates = 0;

public:
    GateGenerator() : nodes(), gates(), numOfNodes(0), numOfGates(0) {}

    ~GateGenerator() {}

    Node* createNode(string nodeName) {
        auto node = new Node(nodeName);
        nodes.push_back(node);
        numOfNodes++;
        return node;
    }

    Node* findNode(string s) {
        for (int i = 0; i < numOfNodes; i++) {

            if (s == (nodes[i]->getName()))
                return nodes[i];
        }
        return createNode(s);
    }

    Gate* createGate(string gateName) {

        Gate* gate = NULL;

        if (gateName == "AND")
            gate = new AND();

        else if (gateName == "NAND")
            gate = new NAND();

        else if (gateName == "OR")
            gate = new OR();

        else if (gateName == "NOR")
            gate = new NOR();

        else if (gateName == "XOR")
            gate = new XOR();

        else if (gateName == "XNOR")
            gate = new XNOR();

        else if (gateName == "NOT")
            gate = new NOT();

        else
            return NULL;

        gates.push_back(gate);
        numOfGates++;

        return gate;
    }

    void startSimulate() {

        for (int i = 0; i < numOfGates; i++)
            gates[i]->simulateGate();
    }

    void printAll() {

        for (auto const& node : nodes)
            cout << node->getName() << ": " << node->getValue() << "\n";
    }

    void parseInput() {
        string Nodes;
        while (cin >> Nodes) {
            if (Nodes == "SET") {
                string node;
                int value;
                cin >> node >> value;
                findNode(node)->setValue(value);
            }
            else if (Nodes == "OUT") {
                string out;
                cin >> out;
                if (out == "ALL")
                    printAll();
                else
                    cout << findNode(out)->getName() << ": " << findNode(out)->getValue() << '\n';
            }
            else if (Nodes == "SIM")
                startSimulate();

            else if (Nodes == "END")
                break;

            else {
                Gate* g = createGate(Nodes);
                string node1, node2, node3;
                cin >> node1 >> node2;
                if (Nodes != "NOT") {
                    cin >> node3;
                    g->setInputA(findNode(node1));
                    g->setInputB(findNode(node2));
                    g->setOutputNode(findNode(node3));
                }
                else {
                    g->setInputA(findNode(node1));
                    g->setOutputNode(findNode(node2));
                }
            }
        }
    }
};



// Procedural Solution

void findNode(vector<pair<string, int>>& nodes, vector<pair<string, vector<pair<string, int>>>>& gates, string& node, int value) {
    for (int i = 0; i < (int)nodes.size(); ++i) {
        if (nodes[i].first == node) {
            nodes[i].second = value;
            break;
        }
    }
    for (int i = 0; i < (int)gates.size(); ++i) {
        for (auto x : gates[i].second) {
            if (x.first == node) {
                x.second = value;
            }
        }
    }
}

int findValue(vector<pair<string, int>>& nodes, string& node) {
    for (int i = 0; i < (int)nodes.size(); ++i) {
        if (nodes[i].first == node)
            return nodes[i].second;
    }
    return -1;
}

void printAll(vector<pair<string, int>>& nodes) {
    for (auto node : nodes)
        cout << node.first << ": " << node.second << '\n';
}

void printOne(vector<pair<string, int>>& nodes, string node) {
    for (int i = 0; i < (int)nodes.size(); ++i) {
        if (nodes[i].first == node) {
            cout << node << ": " << nodes[i].second << "\n";
            break;
        }
    }
}

void startSimulate(vector<pair<string, int>>& nodes, vector<pair<string, vector<pair<string, int>>>>& gates) {
    for (int i = 0; i < (int)gates.size(); ++i) {
        string gateName = gates[i].first;

        if (gateName == "AND") {
            if (findValue(nodes, gates[i].second[0].first) && findValue(nodes, gates[i].second[1].first))
                findNode(nodes, gates, gates[i].second[2].first, 1);
            else
                findNode(nodes, gates, gates[i].second[2].first, 0);
        }

        else if (gateName == "NAND") {
            if (findValue(nodes, gates[i].second[0].first) == 0 || findValue(nodes, gates[i].second[1].first) == 0)
                findNode(nodes, gates, gates[i].second[2].first, 1);
            else
                findNode(nodes, gates, gates[i].second[2].first, 0);
        }

        else if (gateName == "OR") {
            if (findValue(nodes, gates[i].second[0].first) || findValue(nodes, gates[i].second[1].first))
                findNode(nodes, gates, gates[i].second[2].first, 1);
            else
                findNode(nodes, gates, gates[i].second[2].first, 0);
        }

        else if (gateName == "NOR") {
            if (findValue(nodes, gates[i].second[0].first) == 0 && findValue(nodes, gates[i].second[1].first) == 0)
                findNode(nodes, gates, gates[i].second[2].first, 1);
            else
                findNode(nodes, gates, gates[i].second[2].first, 0);
        }

        else if (gateName == "XOR") {
            if (findValue(nodes, gates[i].second[0].first) ^ findValue(nodes, gates[i].second[1].first))
                findNode(nodes, gates, gates[i].second[2].first, 1);
            else
                findNode(nodes, gates, gates[i].second[2].first, 0);
        }

        else if (gateName == "XNOR") {
            if (findValue(nodes, gates[i].second[0].first) == findValue(nodes, gates[i].second[1].first))
                findNode(nodes, gates, gates[i].second[2].first, 1);
            else
                findNode(nodes, gates, gates[i].second[2].first, 0);
        }

        else if (gateName == "NOT") {
            bool val = findValue(nodes, gates[i].second[0].first);
            findNode(nodes, gates, gates[i].second[1].first, !val);
        }
    }
}

void isFound(vector<pair<string, int>>& nodes, string node) {
    for (int i = 0; i < (int)nodes.size(); ++i) {
        if (nodes[i].first == node)
            return;
    }
    nodes.push_back({ node, 0 });
}

void parseInput(vector<pair<string, int>>& nodes, vector<pair<string, vector<pair<string, int>>>>& gates) {
    string word;
    while (cin >> word) {
        if (word == "SET") {
            string node;
            int value;
            cin >> node >> value;
            findNode(nodes, gates, node, value);
        }
        else if (word == "OUT") {
            string out;
            cin >> out;
            if (out == "ALL")
                printAll(nodes);
            else
                printOne(nodes, out);
        }
        else if (word == "SIM")
            startSimulate(nodes, gates);

        else if (word == "END")
            break;

        else {
            string node1, node2, node3;
            cin >> node1 >> node2;
            if (word != "NOT") {
                cin >> node3;
                gates.push_back({ word, {{node1, 0}, {node2, 0}, {node3, 0}} });
                isFound(nodes, node1);
                isFound(nodes, node2);
                isFound(nodes, node3);
            }
            else {
                gates.push_back({ word, {{node1, 0}, {node2, 0}} });
                isFound(nodes, node1);
                isFound(nodes, node2);
            }
        }
    }
}

int main() {
    // to run oop solution
    GateGenerator test;
    test.parseInput();

    // comment the previous one and uncomment the following to run procedural solution
    /*
    vector<pair<string, int>> nodes;
    vector<pair<string, vector<pair<string, int>>>> gates;

    parseInput(nodes, gates);
    */

    return 0;
}