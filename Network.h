//
// Created by ethan on 1/14/2026.
//

#ifndef NETWORK_H
#define NETWORK_H

//includes
#include <vector>
#include "Layer.h"
using namespace std;

class Network
{
    private:
    int middleLayerSize;
    int numberOfLayers;
    vector<Layer> network;

    public:
    Network(int const numOfLayers, int const numOfInputs, int const numOfOutputs, int const middleLayerSize)
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

    vector<double> forward(vector<double> const &input)
    {
        network[0].setLayerValues(input);
        for (int i = 1; i < network.size() - 1; i++)
        {
            network[i].forwardOutput();
        }
        return network[network.size() - 1].receiveOutput();
    }
};

#endif //NETWORK_H
