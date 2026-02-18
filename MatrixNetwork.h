//
// Created by ethan on 1/15/2026.
//

#ifndef ML_FROM_SCRATCH_MATRIXNETWORK_H
#define ML_FROM_SCRATCH_MATRIXNETWORK_H

//includes
#include <vector>
#include <cmath>
#include "Eigen"
using namespace Eigen;
using namespace std;

class MatrixNetwork
{
private:
    //data structs
    struct MatrixLayer
    {
        int n;
        int m;

        MatrixXf weights;
        VectorXf biases;

        MatrixXf weightChanges;
        VectorXf biasesChanges;

        MatrixLayer(int const n, int const m)
        {
            this->n = n;
            this->m = m;

            weights = MatrixXf::Random(n, m).matrix();
            biases = VectorXf::Random(n, 1).matrix();

            weightChanges = MatrixXf(n, m).matrix();
            biasesChanges = VectorXf(n, 1).matrix();
        }

        void update(const int numOfOuts)
        {
            weights = (weights.array() + (weightChanges.array() / numOfOuts)).matrix();
            biases = (biases.array() + (biasesChanges.array() / numOfOuts)).matrix();
        }
    };

    //fields
    int numOfLayers;
    int numOfInputs;
    int numOfOutputs;
    int middleLayerSize;
    vector<MatrixLayer> layers;

    float summer(vector<VectorXf> &saved, VectorXf &answer, int layerI, int m, int outputI)
    {
        float change = 0;

        //base case
        if (layerI == layers.size() - 1)
        {
            return layers[layerI].weights(outputI, m) *
                (saved[layerI + 1][outputI] > 0 ? 1 : 0.01f) * -(answer(outputI) - saved[layerI + 1][outputI]);
        }

        for (int n = 0; n < layers[layerI].n; n++)
        {
            change += layers[layerI].weights(n, m) * (saved[layerI + 1][n] > 0 ? 1 : 0.01f) *
                summer(saved, answer, layerI + 1, n, outputI);
        }

        return change;
    }

public:
    //public data type
    struct Comparable
    {
        VectorXf input;
        VectorXf answer;
    };

    //constructor
    MatrixNetwork(int const numOfLayers, int const numOfInputs, int const numOfOutputs, int const middleLayerSize)
    {
        this->numOfLayers = numOfLayers;
        this->numOfInputs = numOfInputs;
        this->numOfOutputs = numOfOutputs;
        this->middleLayerSize = middleLayerSize;

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

    void train(VectorXf &input, VectorXf &answer, const float learningRate)
    {
        vector<VectorXf> saved;
        saved.reserve(layers.size() + 1);
        saved.emplace_back(input);

        //getting output
        for (auto & layer : layers)
        {
            input = (layer.weights * input + layer.biases).unaryExpr([](float x)
            {
                return max(0.0f, x);
            });

            saved.emplace_back(input);
        }

        //backprop
        for (int outputI = 0; outputI < numOfOutputs - 2; outputI++)
        {
            for (int layerI = 0; layerI < layers.size(); layerI++)
            {
                //first matrix layer
                if (layerI == layers.size() - 1)
                {
                    for (int m = 0; m < layers[layerI].m; m++)
                    {
                        //weights
                        float weightChange = saved[layerI](m) * (saved[layerI + 1][outputI] > 0 ? 1 : 0.01f) *
                            -(answer[outputI] - saved[layerI + 1][outputI]);

                        layers[layerI].weightChanges(outputI, m) += -(learningRate * weightChange);
                    }

                    //basis
                    float basisChange = layers[layerI].biases(outputI) * (saved[layerI + 1][outputI] > 0 ? 1 : 0.01f) *
                        -(answer[outputI] - saved[layerI + 1][outputI]);

                    layers[layerI].biasesChanges(outputI) += -(learningRate * basisChange);

                } else if (layerI == layers.size() - 2)
                {
                    for (int n = 0; n < layers[layerI].n; n++)
                    {
                        for (int m = 0; m < layers[layerI].m; m++)
                        {
                            //weights
                            float weightChange = saved[layerI][m] * (saved[layerI + 1][n] > 0 ? 1 : 0.01f) *
                                layers[layerI + 1].weights(outputI, n) * (saved[layerI + 2][outputI] > 0 ? 1 : 0.01f) *
                                    -(answer[outputI] - saved[layerI + 2][outputI]);

                            layers[layerI].weightChanges(n, m) += -(learningRate * weightChange);
                        }

                        //basis
                        float basisChange = layers[layerI].biases(n) * (saved[layerI + 1][n] > 0 ? 1 : 0.01f) *
                            layers[layerI + 1].weights(outputI, n) * (saved[layerI + 2][outputI] > 0 ? 1 : 0.01f) *
                                -(answer[outputI] - saved[layerI + 2][outputI]);


                        layers[layerI].biasesChanges(n) += -(learningRate * basisChange);
                    }
                } else
                {
                    for (int n = 0; n < layers[layerI].n; n++)
                    {
                        for (int m = 0; m < layers[layerI].m; m++)
                        {
                            //weights
                            float weightChange = saved[layerI][m] * (saved[layerI + 1][n] > 0 ? 1 : 0.01f) *
                                summer(saved, answer, layerI + 1, n, outputI);

                            layers[layerI].weightChanges(n, m) += -(learningRate * weightChange);
                        }

                        //basis
                        float basisChange = layers[layerI].biases(n) * (saved[layerI + 1][n] > 0 ? 1 : 0.01f) *
                            summer(saved, answer, layerI + 1, n, outputI);

                        layers[layerI].biasesChanges(n) += -(learningRate * basisChange);
                    }
                }
            }
        }

        //updating
        for (auto & layer : layers)
        {
            layer.update(numOfOutputs);
        }
    }
};

#endif //ML_FROM_SCRATCH_MATRIXNETWORK_H