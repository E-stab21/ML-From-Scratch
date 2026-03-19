//
// Created by ethan on 1/15/2026.
//

#ifndef ML_FROM_SCRATCH_MATRIXNETWORK_H
#define ML_FROM_SCRATCH_MATRIXNETWORK_H

//includes
#include <vector>
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
        MatrixXf weightVelocity;
        VectorXf biasVelocity;

        MatrixLayer(int n, int m);

        void update(int batchSize);
    };

    //fields
    int numOfLayers;
    int numOfInputs;
    int numOfOutputs;
    int middleLayerSize;
    vector<MatrixLayer> layers;
    static inline auto reLu = [](const float x){ return x > 0 ? x : x * 0.01f; };
    static inline auto leaky = [](const float x){ return x > 0 ? 1.0f : 0.01f; };

public:
    //public data type
    struct Comparable
    {
        VectorXf input;
        VectorXf answer;
    };

    //constructor
    MatrixNetwork(int numOfLayers, int numOfInputs, int numOfOutputs, int middleLayerSize);

    [[nodiscard]] VectorXf forward(VectorXf input) const;

    void train(vector<Comparable> &trainingSet, float maxAlpha, float minAlpha,
        float beta, int batchSize, int epochs, vector<float> &averageBatchLoss);
};

#endif //ML_FROM_SCRATCH_MATRIXNETWORK_H