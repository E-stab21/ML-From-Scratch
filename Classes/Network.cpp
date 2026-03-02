//
// Implementation for Network class.
//

#include "Network.h"

Network::Network(int const numOfLayers, int const numOfInputs, int const numOfOutputs, int const middleLayerSize)
{
    this->numberOfLayers = numOfLayers;
    this->middleLayerSize = middleLayerSize;

    //add input layer
    network.emplace_back(numOfInputs, 1);
    //add middle layers
    for (int i = 0; i < numOfLayers - 2; i++)
    {
        network.emplace_back(middleLayerSize, 1);
    }
    //add output layer
    network.emplace_back(numOfOutputs, 1);
    //connect layers
    for (int i = 0; i < network.size() - 1; i++)
    {
        network[i].integrateLayer(network[i + 1]);
    }
}

vector<double> Network::forward(vector<double> const &input)
{
    network[0].setLayerValues(input);
    for (int i = 1; i < network.size() - 1; i++)
    {
        network[i].forwardOutput();
    }

    //sigmoid
    double max = -1e+99;
    double min = 1e+99;
    auto outputs = network[network.size() - 1].receiveOutput();
    for (double &output : outputs)
    {
        if (output < min)
        {
            min = output;
        }
        if (output > max)
        {
            max = output;
        }
    }

    for (double &output : outputs)
    {
        output = (output - min) / (max - min);
    }

    return outputs;
}