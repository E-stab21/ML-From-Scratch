//
// Node implementation.
//
#include "Node.h"

Node::Node(double const bias, double const value)
{
    this->bias = bias;
    this->value = value;
    connections = vector<Connection>();
    inputs = vector<double>();
}

Node::Node(double const bias)
{
    this->bias = bias;
    this->value = 0.0;
    connections = vector<Connection>();
    inputs = vector<double>();
}

void Node::addConnection(Node &child, double const weight)
{
    Connection const connection{
        &child,
        weight
    };

    connections.push_back(connection);
}

void Node::receiveInput(double const input)
{
    inputs.push_back(input);
}

void Node::sendOutput()
{
    setOutputValue();

    //sending output
    for (auto & [child, weight] : connections)
    {
        child->receiveInput(weight * value);
    }
}

double Node::setOutputValue()
{
    double inputSums = bias;
    for (double const input : inputs)
    {
        inputSums += input;
    }
    value = max(0.0, inputSums);
    return value;
}

void Node::setValue(double const input)
{
    value = max(0.0, input);
}