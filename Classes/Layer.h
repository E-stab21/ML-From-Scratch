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
    Layer(int size, int upper);

    void integrateLayer(Layer &layer);

    void setLayerValues(vector<double> const &layerValues);

    void forwardOutput();

    vector<double> receiveOutput();
};

#endif //LAYER_H
