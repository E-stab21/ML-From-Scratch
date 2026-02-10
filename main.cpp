#include <iostream>
#include <vector>
#include <random>
#include "MatrixNetwork.h"
#include "Network.h"
using namespace std;

int main()
{
    random_device rd;
    uniform_real_distribution<> dist(0, 1);
    mt19937 gen(rd());

    //object version
    auto n = Network(5, 784, 10, 16);
    vector<double> input(784);
    for (double &i : input) {
        i = dist(gen);
    }
    auto output = n.forward(input);
    for (double i : output)
    {
        cout << i << endl;
    }
    cout << endl;

    //matrix version
    const auto m = MatrixNetwork(5, 784, 10, 16);
    VectorXf inputV = VectorXf::Random(784, 1);
    const auto outputV = m.forward(inputV);
    cout << outputV << endl;
}