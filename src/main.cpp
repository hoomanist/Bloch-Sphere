#include "stdio.h"
#include "../include/linalg.hpp"
#include "../include/pauli.hpp"
#include <iostream>

int main() {
    // using namespace Pauli;
    auto M = Exp(Pauli::X, 10);
    std::cout << M;
}