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
    //data structs
    struct MatrixLayer
    {
        int n;
        int m;

        MatrixXf weights;
        VectorXf biases;

        MatrixXf weightChanges;
        VectorXf biasesChanges;

        MatrixLayer(int const n, int const m) :
        n(n), m(m), weights(MatrixXf::Random(n, m) * sqrt(2.0f / static_cast<float>(n))),
        biases(VectorXf::Zero(n))
        {
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
    static inline auto leaky = [](const float x){ return x > 0 ? 1.0f : 0.01f; };
    static inline auto reLu = [](const float x){ return x > 0 ? x : x * 0.01f; };

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
        const VectorXf input;
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

    [[nodiscard]] VectorXf forward(VectorXf input) const
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

    void train(vector<Comparable> &trainingObjects, const float learningRate)
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

                //changing stuff
                // for (int row{0}; row < layers[layerIndex].n; row++)
                // {
                //     //weights
                //     for (int col{0}; col < layers[layerIndex].m; col++)
                //     {
                //         layers[layerIndex].weights(row, col) -= learningRate * (saved[layerIndex].first[col]
                //             * copy[row]);
                //     }
                //
                //     //biases
                //     layers[layerIndex].biases(row) -= learningRate * copy[row];
                // }
            }
        }

    //     vector<VectorXf> saved;
    //     saved.reserve(layers.size() + 1);
    //     saved.emplace_back(input);
    //
    //     //getting output
    //     for (auto & layer : layers)
    //     {
    //         input = (layer.weights * input + layer.biases).unaryExpr([](float x)
    //         {
    //             return max(0.0f, x);
    //         });
    //
    //         saved.emplace_back(input);
    //     }
    //
    //     //backprop
    //     for (int outputI = 0; outputI < numOfOutputs - 2; outputI++)
    //     {
    //         for (int layerI = 0; layerI < layers.size(); layerI++)
    //         {
    //             //first matrix layer
    //             if (layerI == layers.size() - 1)
    //             {
    //                 for (int m = 0; m < layers[layerI].m; m++)
    //                 {
    //                     //weights
    //                     float weightChange = saved[layerI](m) * (saved[layerI + 1][outputI] > 0 ? 1 : 0.01f) *
    //                         -(answer[outputI] - saved[layerI + 1][outputI]);
    //
    //                     layers[layerI].weightChanges(outputI, m) += -(learningRate * weightChange);
    //                 }
    //
    //                 //basis
    //                 float basisChange = layers[layerI].biases(outputI) * (saved[layerI + 1][outputI] > 0 ? 1 : 0.01f) *
    //                     -(answer[outputI] - saved[layerI + 1][outputI]);
    //
    //                 layers[layerI].biasesChanges(outputI) += -(learningRate * basisChange);
    //
    //             } else if (layerI == layers.size() - 2)
    //             {
    //                 for (int n = 0; n < layers[layerI].n; n++)
    //                 {
    //                     for (int m = 0; m < layers[layerI].m; m++)
    //                     {
    //                         //weights
    //                         float weightChange = saved[layerI][m] * (saved[layerI + 1][n] > 0 ? 1 : 0.01f) *
    //                             layers[layerI + 1].weights(outputI, n) * (saved[layerI + 2][outputI] > 0 ? 1 : 0.01f) *
    //                                 -(answer[outputI] - saved[layerI + 2][outputI]);
    //
    //                         layers[layerI].weightChanges(n, m) += -(learningRate * weightChange);
    //                     }
    //
    //                     //basis
    //                     float basisChange = layers[layerI].biases(n) * (saved[layerI + 1][n] > 0 ? 1 : 0.01f) *
    //                         layers[layerI + 1].weights(outputI, n) * (saved[layerI + 2][outputI] > 0 ? 1 : 0.01f) *
    //                             -(answer[outputI] - saved[layerI + 2][outputI]);
    //
    //
    //                     layers[layerI].biasesChanges(n) += -(learningRate * basisChange);
    //                 }
    //             } else
    //             {
    //                 for (int n = 0; n < layers[layerI].n; n++)
    //                 {
    //                     for (int m = 0; m < layers[layerI].m; m++)
    //                     {
    //                         //weights
    //                         float weightChange = saved[layerI][m] * (saved[layerI + 1][n] > 0 ? 1 : 0.01f) *
    //                             summer(saved, answer, layerI + 1, n, outputI);
    //
    //                         layers[layerI].weightChanges(n, m) += -(learningRate * weightChange);
    //                     }
    //
    //                     //basis
    //                     float basisChange = layers[layerI].biases(n) * (saved[layerI + 1][n] > 0 ? 1 : 0.01f) *
    //                         summer(saved, answer, layerI + 1, n, outputI);
    //
    //                     layers[layerI].biasesChanges(n) += -(learningRate * basisChange);
    //                 }
    //             }
    //         }
    //     }
    //
    //     //updating
    //     for (auto & layer : layers)
    //     {
    //         layer.update(numOfOutputs);
    //     }
    }
};

#endif //ML_FROM_SCRATCH_MATRIXNETWORK_H