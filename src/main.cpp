#include <iostream>
#include <fstream>
#include <random>
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

int main()
{
    //globals
    const int trainingSize = 1000;
    const int testingSize = 100;
    const int numOfLayers = 2;
    const int middleLayerSize = 10;
    vector<MatrixNetwork::Comparable> train;
    vector<Mnist> test;
    vector<float> costData;

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
        cout << "file not found";
        exit(0);
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
        cout << "file not found";
        exit(0);
    }

    // cout << "Please enter the number of layers you want: " << endl;
    // while (check)
    // {
    //     check = true;
    //     if (!(cin >> layers))
    //     {
    //         cin.clear();
    //         cin.ignore(numeric_limits<streamsize>::max(), '\n');
    //         cout << "Please enter a valid int: " << endl;
    //     } else if (!(layers > 0 && layers < 8))
    //     {
    //         cout << "number given should not be either negative or greater than 8: " << endl;
    //     } else
    //     {
    //         check = false;
    //     }
    // }

    //create matrix
    auto m = MatrixNetwork(numOfLayers, 784, 10, middleLayerSize);
    //train
    m.train(train, 0.02, 0.001, 0.9, 10, 20, costData);

    //output to console
    int numCorrect{0};
    for (auto &image : test)
    {
        float max{0};
        int maxIndex{0};
        auto output = m.forward(image.input);
        for (int row = 0; row < 10; row++)
        {
            cout << output(row) << " ";
            if (output(row) > max)
            {
                max = output(row);
                maxIndex = row;
            }
        }
        if (maxIndex == image.label)
        {
            numCorrect++;
        }
        cout << image.label << endl;
    }

    cout << endl << numCorrect << endl;
}