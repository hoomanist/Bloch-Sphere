#pragma once
#include "linalg.hpp"
#include "pauli.hpp"

ComplexMatrix2x2 LandauZener_Hamiltonian(double t) {
    double alpha = 1.5;
    double delta = 1;
    return (alpha*t/2)*Pauli::Z + (delta/2)*Pauli::X;
}

ComplexMatrix2x2 RotatingField_Hamiltonian(double t) {
    double theta = M_PI / 2;
    double omega = 0.05;
    double T = 2*M_PI / omega;
    double B_0 = 1;
    ComplexMatrix2x2 H_x = B_0 * sin(theta)*cos(omega*t) * Pauli::X;
    ComplexMatrix2x2 H_y = B_0 * sin(theta)*sin(omega*t) * Pauli::Y;
    ComplexMatrix2x2 H_z = B_0 * cos(theta) * Pauli::Z;
    return -1 * (H_x+H_y+H_z);

}