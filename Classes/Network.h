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
    int middleLayerSize;
    int numberOfLayers;
    vector<Layer> network;

    public:
    Network(int numOfLayers, int numOfInputs, int numOfOutputs, int middleLayerSize);

    vector<double> forward(vector<double> const &input);
};

#endif //NETWORK_H
