#include <iostream>
#include <fstream>
#include "MatrixNetwork.h"
using namespace std;

//data type
struct Mnist : public MatrixNetwork::Comparable
{
    int label;

    Mnist(const VectorXf &input, const int label)
    {
        this->label = label;
        this->input = input;
        answer = VectorXf(10, 1);
        answer[label] = 1;
    }
};

int main()
{
    //read in data
    VectorXf test(784, 1);
    VectorXf answer(10, 1);

    if (ifstream file("../MNIST_CSV/mnist_train.csv"); file)
    {
        char comma;
        int label;
        file >> label;
        answer[label] = 1;

        for (int i = 0; i < 784; i++)
        {
            file >> comma >> test[i];
            test[i] /= 253;
        }

    } else
    {
        cout << "file not found";
        exit(0);
    }

    //create matrix
    auto m = MatrixNetwork(5, 784, 10, 16);
    m.train(test, answer, 0.01);
}