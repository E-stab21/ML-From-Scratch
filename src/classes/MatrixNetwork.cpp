//
// Implementation for MatrixNetwork class.
//

#include <random>
#include "MatrixNetwork.h"

MatrixNetwork::MatrixLayer::MatrixLayer(const int n, const int m) :
n(n), m(m),
weights(MatrixXf::Random(n, m) * sqrt(2.0f / static_cast<float>(n))),
biases(VectorXf::Zero(n)),
weightChanges(MatrixXf::Zero(n, m)),
biasesChanges(VectorXf::Zero(n)),
weightVelocity(MatrixXf::Zero(n, m)),
biasVelocity(VectorXf::Zero(n)) {};

void MatrixNetwork::MatrixLayer::update(const int batchSize)
{
    weights = weights + weightChanges / batchSize;
    biases = biases + biasesChanges / batchSize;
    weightChanges.setZero();
    biasesChanges.setZero();
}

MatrixNetwork::MatrixNetwork(int const numOfLayers, int const numOfInputs, int const numOfOutputs, int const middleLayerSize) :
numOfLayers(numOfLayers),
numOfInputs(numOfInputs),
numOfOutputs(numOfOutputs),
middleLayerSize(middleLayerSize)
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

void MatrixNetwork::train(vector<Comparable> &trainingSet, const float maxAlpha, const float minAlpha,
    const float beta, const int batchSize, const int epochs, vector<float> &averageBatchLoss)
{
    mt19937 g{random_device{}()};

    //input val
    if (batchSize < 1 || maxAlpha < 0 || minAlpha < 0 || epochs < 1)
    {
        throw runtime_error("incorrect training params");
    }

    //alpha warmup
    const int warmups = ceil(0.1 * epochs);

    for (int epoch = 0; epoch < epochs; epoch++)
    {
        float alpha = 0;
        if (epoch < warmups)
        {
            //warmup
            alpha = maxAlpha * (static_cast<float>(epoch + 1) / static_cast<float>(warmups));
        } else
        {
            //cosine annealing
            alpha = minAlpha + (maxAlpha - minAlpha) * (1 + cos(static_cast<float>(epoch - warmups)
                * static_cast<float>(M_PI) / static_cast<float>(epochs)));
        }

        //shuffle the training set
        shuffle(trainingSet.begin(), trainingSet.end(), g);

        //divide the training set into batch's
        vector<vector<Comparable>> batchedData;
        batchedData.reserve(ceil(trainingSet.size() / batchSize));
        for (int i = 0; i < trainingSet.size(); i += batchSize)
        {
            const int endIndex = min(static_cast<int>(trainingSet.size()), i + batchSize);
            batchedData.emplace_back(trainingSet.begin() + i, trainingSet.begin() + endIndex);
        }

        for (const auto &batch: batchedData)
        {
            float storedLoss = 0;

            for (const auto & [input, answer] : batch)
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

                //softmax
                copy = (copy.array() - copy.maxCoeff()).exp();
                copy = copy / copy.sum();

                //add loss
                float epsilon = 1e-7;
                VectorXf log_output = (copy.array() + epsilon).log();
                const float loss = -answer.dot(log_output);
                storedLoss += loss;

                //back propagation
                for (int layerIndex = static_cast<int>(layers.size()) - 1; layerIndex >= 0; layerIndex--)
                {
                    //get a_k
                    if (layerIndex == static_cast<int>(layers.size()) - 1)
                    {
                        for (int row{0}; row < copy.size(); row++)
                        {
                            copy[row] = copy[row] - answer[row];
                        }
                    } else
                    {
                        copy = layers[layerIndex + 1].weights.transpose() * copy;
                        VectorXf derivative = saved[layerIndex + 1].second.unaryExpr(leaky);
                        copy = copy.cwiseProduct(derivative);
                    }

                    MatrixXf weightGradients = copy * saved[layerIndex].first.transpose();
                    VectorXf biasGradients = copy;
                    //velocity's
                    layers[layerIndex].weightVelocity = beta * layers[layerIndex].weightVelocity + weightGradients;
                    layers[layerIndex].biasVelocity = beta * layers[layerIndex].biasVelocity + biasGradients;
                    //changes
                    layers[layerIndex].weightChanges -= alpha * layers[layerIndex].weightVelocity;
                    layers[layerIndex].biasesChanges -= alpha * layers[layerIndex].biasVelocity;
                }
            }

            for (auto &layer : layers)
            {
                layer.update(batchSize);
            }

            averageBatchLoss.push_back(storedLoss / static_cast<float>(batch.size()));
        }
    }
}
