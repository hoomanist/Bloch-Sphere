#pragma once
#include "linalg.hpp"
#include "pauli.hpp"

ComplexMatrix2x2 LandauZener_hamiltonian(double t) {
    double alpha = 1.5;
    double delta = 1;
    return (alpha*t/2)*Pauli::Z + (delta/2)*Pauli::X;
}