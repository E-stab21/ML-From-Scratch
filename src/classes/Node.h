//
// Created by ethan on 1/14/2026.
//

#ifndef NODE_H
#define NODE_H

//includes
#include <vector>
using namespace std;

class Node
{
    struct Connection
    {
        Node* const child;
        double weight;
    };

    double bias;
    double value;
    vector<Connection> connections;
    vector<double> inputs;

    public:
    Node(double  bias, double  value);

    explicit Node(double  bias);

    void addConnection(Node &child, double  weight);

    void receiveInput(double  input);

    void sendOutput();

    double setOutputValue();

    void setValue(double  input);
};

#endif //NODE_H
