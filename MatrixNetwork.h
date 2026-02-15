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
    };

    //fields
    vector<MatrixLayer> layers;
    int numOfLayers;
    int numOfInputs;
    int numOfOutputs;
    int middleLayerSize;

    double summer(vector<VectorXf> &saved, VectorXf &answer, int layerI, int m, int outputI)
    {
        double change = 0;

        //base case
        if (layerI == layers.size() - 1)
        {
            return layers[layerI].weights(outputI, m) *
                (saved[layerI][outputI] > 0 ? 1 : 0.01) * -(answer(outputI) - saved[layerI][outputI]);
        }

        for (int n = 0; n < layers[layerI].n; n++)
        {
            change += layers[layerI].weights(n, m) * (saved[layerI][n] > 0 ? 1 : 0.01) *
                summer(saved, answer, layerI + 1, n, outputI);
        }

        return change;
    }

public:

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

    void train(VectorXf &input, VectorXf &answer, double const learningRate)
    {
        vector<VectorXf> saved;
        saved.reserve(layers.size());

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
                        double weightChange = saved[layerI - 1][m] * (saved[layerI][outputI] > 0 ? 1 : 0.01) *
                            -(answer[outputI] - saved[layerI][outputI]);

                        layers[layerI].weightChanges(outputI, m) += -(learningRate * weightChange);
                    }

                    //basis
                    double basisChange = layers[layerI].biases(outputI) * (saved[layerI][outputI] > 0 ? 1 : 0.01) *
                        -(answer[outputI] - saved[layerI][outputI]);

                    layers[layerI].biasesChanges(outputI) += -(learningRate * basisChange);

                } else if (layerI == layers.size() - 2)
                {
                    for (int n = 0; n < layers[layerI].n; n++)
                    {
                        for (int m = 0; m < layers[layerI].m; m++)
                        {
                            //weights
                            double weightChange = saved[layerI - 1][m] * (saved[layerI][n] > 0 ? 1 : 0.01) *
                                layers[layerI + 1].weights(outputI, n) * (saved[layerI + 1][outputI] > 0 ? 1 : 0.01) *
                                    -(answer[outputI] - saved[layerI][outputI]);

                            layers[layerI].weightChanges(n, m) += -(learningRate * weightChange);
                        }

                        //basis
                        double basisChange = layers[layerI].biases(n) * (saved[layerI][n] > 0 ? 1 : 0.01) *
                            layers[layerI + 1].weights(outputI, n) * (saved[layerI + 1][outputI] > 0 ? 1 : 0.01) *
                                -(answer[outputI] - saved[layerI][outputI]);


                        layers[layerI].biasesChanges(n) += -(learningRate * basisChange);
                    }
                } else
                {
                    for (int n = 0; n < layers[layerI].n; n++)
                    {
                        for (int m = 0; m < layers[layerI].m; m++)
                        {
                            //weights
                            double weightChange = saved[layerI - 1][m] * (saved[layerI][n] > 0 ? 1 : 0.01) *
                                summer(saved, answer, layerI + 1, n, outputI);

                            layers[layerI].weightChanges(n, m) += -(learningRate * weightChange);
                        }

                        //basis
                        double basisChange = layers[layerI].biases(n) * (saved[layerI][n] > 0 ? 1 : 0.01) *
                            summer(saved, answer, layerI + 1, n, outputI);

                        layers[layerI].biasesChanges(n) += -(learningRate * basisChange);
                    }
                }
            }
        }
    }
};

#endif //ML_FROM_SCRATCH_MATRIXNETWORK_H