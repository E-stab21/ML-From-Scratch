//
// Created by ethan on 1/15/2026.
//

#ifndef ML_FROM_SCRATCH_MATRIXNETWORK_H
#define ML_FROM_SCRATCH_MATRIXNETWORK_H

//includes
#include <vector>
#include <random>
#include <cmath>
#include "Eigen"
using namespace Eigen;
using namespace std;

class MatrixNetwork
{
    struct MatrixLayer
    {
        MatrixXf weights;
        VectorXf biases;

        MatrixLayer(int const n, int const m)
        {
            weights = MatrixXf::Random(n, m).matrix();
            biases = VectorXf::Random(n, 1).matrix();
        }
    };

    vector<MatrixLayer> layers;

    public:

    MatrixNetwork(int const numOfLayers, int const numOfInputs, int const numOfOutputs, int const middleLayerSize)
    {
        layers.reserve(numOfLayers);
        //input layer
        layers.emplace_back(middleLayerSize, numOfInputs);
        //middle layers
        for (int i = 0; i < numOfLayers - 2; i++)
        {
            layers.emplace_back(middleLayerSize, middleLayerSize);
        }
        //output
        layers.emplace_back(numOfOutputs, middleLayerSize);
    }

    VectorXf forward(VectorXf &input) const
    {
        for (auto & layer : layers)
        {
            input = (layer.weights * input + layer.biases).unaryExpr([](float x)
            {
                return max(0.0f, x);
            });
        }

        //softmax
        //input = (input.array() - input.maxCoeff()).exp();
        //input = input / input.sum()

        //sigmoid
        input = (input.array() - input.minCoeff()) / (input.maxCoeff() - input.minCoeff());

        return input;
    }
};

#endif //ML_FROM_SCRATCH_MATRIXNETWORK_H