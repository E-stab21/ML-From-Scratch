//
// Created by ethan on 1/14/2026.
//

#ifndef LAYER_H
#define LAYER_H

//includes
#include <vector>
#include <random>
#include "Node.h"
using namespace std;

class Layer
{
    private:
    int size;
    double upper;
    vector<Node> nodes;

    public:
    Layer(int const size, int const upper)
    {
        this->size = size;
        this->upper = upper;

        //random bias
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<double> dist(0, upper);

        for (int i = 0; i < size; i++)
        {
            nodes.emplace_back(dist(gen));
        }
    }

    void integrateLayer(Layer &layer)
    {
        //random weights
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<double> dist(0, upper);

        for (Node &node: nodes)
        {
            for (Node &connect: layer.nodes)
            {
                node.addConnection(connect, dist(gen));
            }
        }
    }

    void setLayerValues(vector<double> const &layerValues)
    {
        for (int i = 0; i < layerValues.size(); i++)
        {
            nodes[i].setValue(layerValues[i]);
        }
    }

    void forwardOutput()
    {
        for (Node &node: nodes)
        {
            node.sendOutput();
        }
    }

    vector<double> receiveOutput()
    {
        auto output = vector<double>(nodes.size());
        for (int i = 0; i < nodes.size(); i++)
        {
            output[i] = nodes[i].setOutputValue();
        }
        return output;
    }
};

#endif //LAYER_H
