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
    Node(double const bias, double const value)
    {
        this->bias = bias;
        this->value = value;
        connections = vector<Connection>();
        inputs = vector<double>();
    }

    explicit Node(double const bias)
    {
        this->bias = bias;
        this->value = 0.0;
        connections = vector<Connection>();
        inputs = vector<double>();
    }

    void addConnection(Node &child, double const weight)
    {
        Connection const connection{
            &child,
            weight
        };

        connections.push_back(connection);
    }

    void receiveInput(double const input)
    {
        inputs.push_back(input);
    }

    void sendOutput()
    {
        setOutputValue();

        //sending output
        for (auto & [child, weight] : connections)
        {
            child->receiveInput(weight * value);
        }
    }

    double setOutputValue()
    {
        double inputSums = bias;
        for (double const input : inputs)
        {
            inputSums += input;
        }
        value = max(0.0, inputSums);
        return value;
    }

    void setValue(double const input)
    {
        value = max(0.0, input);
    }
};

#endif //NODE_H
