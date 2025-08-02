// g++ -o percep perceptron.cpp
#include <bits/stdc++.h>
#include <cstdlib>
#include <ctime>

// #define RAND_MAX 10

int main(void)
{
    std::cout << "Hello, World!" << std::endl;
    // srand(10);
    srand (static_cast <unsigned> (time(0)));
    double rt = static_cast <double> (RAND_MAX);
    double r  = static_cast <double> (rand()) / rt ;
    double w2 =  rand() / rt;
    double w1 =  rand() / rt;
    printf("RAND_MAX: %.0f\n", rt);
    printf("w1: %.0f\nw2: %.0f\nr: %.0f\n", w1, w2, r);
    // std::cout << "w1: " << w1 << "\n"
    //           << "w2: " << w2 << "\n";
    return 0;
}