//
// Implementation for MatrixNetwork class.
//

#include "MatrixNetwork.h"

MatrixNetwork::MatrixLayer::MatrixLayer(const int n, const int m) :
n(n), m(m), weights(MatrixXf::Random(n, m) * sqrt(2.0f / static_cast<float>(n))),
biases(VectorXf::Zero(n))
{
    weightChanges = MatrixXf(n, m).matrix();
    biasesChanges = VectorXf(n, 1).matrix();
};

void MatrixNetwork::MatrixLayer::update(const int numOfOuts)
{
    weights = (weights.array() + (weightChanges.array() / numOfOuts)).matrix();
    biases = (biases.array() + (biasesChanges.array() / numOfOuts)).matrix();
}

MatrixNetwork::MatrixNetwork(int const numOfLayers, int const numOfInputs, int const numOfOutputs, int const middleLayerSize) :
numOfLayers(numOfLayers), numOfInputs(numOfInputs), numOfOutputs(numOfOutputs), middleLayerSize(middleLayerSize)
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

VectorXf MatrixNetwork::forward(VectorXf input) const
{
    for (auto & layer : layers)
    {
        input = (layer.weights * input + layer.biases).unaryExpr(reLu);
    }

    //softmax
    input = (input.array() - input.maxCoeff()).exp();
    input = input / input.sum();

    return input;
}

void MatrixNetwork::train(vector<Comparable> &trainingObjects, const float learningRate)
{
    for (const auto & [input, answer] : trainingObjects)
    {
        VectorXf copy = input;
        vector<pair<VectorXf, VectorXf>> saved;
        saved.emplace_back(copy, VectorXf::Zero(copy.size()));

        //forward
        for (auto &layer : layers)
        {
            VectorXf z = layer.weights * copy + layer.biases;
            copy = z.unaryExpr(reLu);
            saved.emplace_back(copy, z);
        }

        copy = (copy.array() - copy.maxCoeff()).exp();
        copy = copy / copy.sum();

        //back prop
        for (int layerIndex = static_cast<int>(layers.size()) - 1; layerIndex >= 0; layerIndex--)
        {
            //get a_k
            if (layerIndex == static_cast<int>(layers.size()) - 1)
            {
                for (int row{0}; row < copy.size(); row++)
                {
                    // copy[row] = leaky(saved[layerIndex + 1].second[row]) * 2 * (copy[row] - answer(row));
                    copy[row] = copy[row] - answer[row];
                }
            } else
            {
                copy = layers[layerIndex + 1].weights.transpose() * copy;
                VectorXf derivative = saved[layerIndex + 1].second.unaryExpr(leaky);
                copy = copy.cwiseProduct(derivative);
            }

            layers[layerIndex].weights -= learningRate * (copy * saved[layerIndex].first.transpose());
            layers[layerIndex].biases -= learningRate * copy;
        }
    }
}

