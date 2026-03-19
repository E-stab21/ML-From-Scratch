#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include "classes/MatrixNetwork.h"

using namespace std;

//data type
struct Mnist : MatrixNetwork::Comparable
{
    const int label;

    Mnist(const VectorXf &input, const int label) :
    Comparable(input, VectorXf::Zero(10)), label{label}
    {
        answer[label] = 1;
    }
};

int main(int argc, char* argv[])
{
    //args
    const int numOfLayers = stoi(string(argv[1]));
    const int middleLayerSize = stoi(string(argv[2]));
    const int trainingSize = stoi(string(argv[3]));
    const int batchSize = stoi(string(argv[4]));
    const int epochs = stoi(string(argv[5]));
    const float minLr = stof(string(argv[6]));
    const float maxLr = stof(string(argv[7]));
    const float beta = stof(string(argv[8]));

    //globals
    const int testingSize = 100;
    vector<MatrixNetwork::Comparable> train;
    vector<Mnist> test;
    vector<float> lossData;


    //read in training
    if (ifstream file("../MNIST_CSV/mnist_train.csv"); file)
    {
        for (int i{}; i < trainingSize; i++)
        {
            char comma;
            int label;
            VectorXf input(784);

            file >> label;

            for (int j = 0; j < 784; j++)
            {
                file >> comma >> input[j];
                input[j] = input[j] / 255;
            }

            train.push_back(Mnist(input, label));
        }
    } else
    {
        cerr << "file not found";
        exit(10);
    }

    //read in testing
    if (ifstream file("../MNIST_CSV/mnist_test.csv"); file)
    {
        for (int i{}; i < testingSize; i++)
        {
            char comma;
            int label;
            VectorXf input(784);

            file >> label;

            for (int j = 0; j < 784; j++)
            {
                file >> comma >> input[j];
                input[j] = input[j] / 255;
            }

            test.emplace_back(input, label);
        }
    } else
    {
        cerr << "file not found";
        exit(10);
    }

    //create matrix
    auto m = MatrixNetwork(numOfLayers, 784, 10, middleLayerSize);
    //train
    m.train(train, maxLr, minLr, beta, batchSize, epochs, lossData);

    //counting the number of correct
    int numCorrect = 0;
    for (auto &image : test)
    {
        float max = 0;
        int maxIndex = 0;
        auto output = m.forward(image.input);
        for (int row = 0; row < 10; row++)
        {
            if (output(row) > max)
            {
                max = output[row];
                maxIndex = row;
            }
        }
        if (maxIndex == image.label)
        {
            numCorrect++;
        }
    }
    cout << numCorrect << endl;

    //loss output to console
    for (int i = 0; i < lossData.size(); i++)
    {
        cout << i << ',' << lossData[i] << endl;
    }
}