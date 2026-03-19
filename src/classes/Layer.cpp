//
// Layer implementation.
//

#include "Layer.h"

Layer::Layer(int const size, int const upper)
{
    this->size = size;
    this->upper = upper;

    //random bias
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dist(-upper, upper);

    for (int i = 0; i < size; i++)
    {
        nodes.emplace_back(dist(gen));
    }
}

void Layer::integrateLayer(Layer &layer)
{
    //random weights
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dist(-upper, upper);

    for (Node &node: nodes)
    {
        for (Node &connect: layer.nodes)
        {
            node.addConnection(connect, dist(gen));
        }
    }
}

void Layer::setLayerValues(vector<double> const &layerValues)
{
    for (int i = 0; i < layerValues.size(); i++)
    {
        nodes[i].setValue(layerValues[i]);
    }
}

void Layer::forwardOutput()
{
    for (Node &node: nodes)
    {
        node.sendOutput();
    }
}

vector<double> Layer::receiveOutput()
{
    auto output = vector<double>(nodes.size());
    for (int i = 0; i < nodes.size(); i++)
    {
        output[i] = nodes[i].setOutputValue();
    }
    return output;
}