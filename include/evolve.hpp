#pragma once
#include "linalg.hpp"
#include "hamiltonian.hpp"
#include <functional>



ComplexVector2 dpsi_dt(std::function<ComplexMatrix2x2(double)> H, const ComplexVector2& psi, double t) {
    return -Complex(0, 1) * H(t) * psi; 
}

ComplexVector2 rk4_step(std::function<ComplexMatrix2x2(double)> H, const ComplexVector2& psi, double t, double dt) {
    auto k1 = dpsi_dt(H, psi, t);
    auto k2 = dpsi_dt(H, psi + k1 * (dt / 2.0), t + dt / 2.0);
    auto k3 = dpsi_dt(H, psi + k2 * (dt / 2.0), t + dt / 2.0);
    auto k4 = dpsi_dt(H, psi + k3 * dt, t + dt);

    return psi + (dt / 6.0) * (k1 + 2.0*k2 + 2.0*k3 + k4);
}

struct Evolution
{
    std::map<int, std::pair<ComplexVector2, double>> data; 
    std::function<ComplexMatrix2x2(double)> Hamiltonian;
    int processed = 0;
    double last_n = 0;
    double dt;

    void evolve(int steps) {
        ComplexVector2 psi = data[last_n].first;
        int n = 0;
        double phase = 0;
        while (n < steps)
        {
            auto t = (last_n + n)*dt;
            psi = rk4_step(Hamiltonian, psi, t, dt);
            psi = (1/std::sqrt(std::norm(psi.data[0]) + std::norm(psi.data[1]))) *psi ;
            phase +=  -0.5*(Hamiltonian(t + dt).eigenvalues().second - Hamiltonian(t).eigenvalues().second).real()*dt;
            data[last_n + n] = {psi, phase};
            n++;
        }
        last_n += n - 1;
    } 

    void increment_processed() {
        processed++;

        if(data.size() - processed < 10) {
            evolve(30);
        }
    }

    std::vector<sf::Vector3f> get_trail(double time){
        std::vector<sf::Vector3f> trail;   
        int n = static_cast<int>(time / dt);
        for (const auto& [key, value] : data) {
            if (key < n && key % 10 == 0){
                trail.push_back(blochVector(value.first));
            }
        }
        return trail;
    }

    ComplexVector2 get_current(double time) {
        int n = static_cast<int>(time / dt);
        if (n < last_n){
            std::cout<< n << "\t" << last_n << "\n";
            std:: cout << data[n].first << "\n";
            return data[n].first;
        } else {
            evolve(10000);
            std::cout << "Jill: " << last_n << "\n";
            return get_current(time); 
        }
    }

    public:
        Evolution(std::function<ComplexMatrix2x2(double)> H,
            ComplexVector2 initial_state,
            double dt) : dt(dt), Hamiltonian(H){
                data = {
                    {0, {
                        initial_state, 0,
                    }}
                };
        }
};
