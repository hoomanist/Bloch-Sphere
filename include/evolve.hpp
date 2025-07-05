#pragma once
#include "linalg.hpp"
#include "hamiltonian.hpp"
#include <functional>


ComplexVector2 dpsi_dt(ComplexMatrix2x2 (*H)(double), const ComplexVector2& psi, double t) {
    return -Complex(0, 1) * H(t) * psi; 
}

ComplexVector2 rk4_step(ComplexMatrix2x2 (*H)(double), const ComplexVector2& psi, double t, double dt) {
    auto k1 = dpsi_dt(H, psi, t);
    auto k2 = dpsi_dt(H, psi + k1 * (dt / 2.0), t + dt / 2.0);
    auto k3 = dpsi_dt(H, psi + k2 * (dt / 2.0), t + dt / 2.0);
    auto k4 = dpsi_dt(H, psi + k3 * dt, t + dt);

    return psi + (dt / 6.0) * (k1 + 2.0*k2 + 2.0*k3 + k4);
}

std::pair<ComplexVector2, double> evolve(ComplexMatrix2x2 (*H)(double), const ComplexVector2& initial_state, double *t_end, double dt) {
    ComplexVector2 psi = initial_state;
    double phase;
    double t = 0;
    while (t < *t_end) {
        psi = rk4_step(H, psi, t, dt);
        phase +=  -0.5*(H(t+dt).eigenvalues().second - H(t).eigenvalues().second).real()*dt;
        t += dt;
    
        // normalize to counter numerical drift
        psi = (1/std::sqrt(std::norm(psi.data[0]) + std::norm(psi.data[1]))) *psi ;
    }
    return {psi, phase}; 
}