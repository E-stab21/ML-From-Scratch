#include <iostream>
#include <vector>
#include <random>
#include "MatrixNetwork.h"
using namespace std;

int main()
{
    const auto m = MatrixNetwork(5, 780, 10, 800);
    VectorXf input = VectorXf::Random(780, 1);
    const auto output = m.forward(input);
    cout << output << endl;
}