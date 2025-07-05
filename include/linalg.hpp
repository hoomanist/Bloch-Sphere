#pragma once
#include <complex>
#include <iostream>
#include <array>

using Complex = std::complex<double>;


struct ComplexVector2 {
    std::array<Complex, 2> data;

    ComplexVector2 conjugate() const {
        return ComplexVector2({std::conj(data[0]),std::conj(data[1])});
    }
    void normalize() {
        double norm = std::sqrt(
            std::norm(data[0]) + std::norm(data[1])
        );
        if (norm > 0) {
            data[0] /= norm;
            data[1] /= norm;
        }
    }

    ComplexVector2 operator+(const ComplexVector2& other) const {
        ComplexVector2 result;
        result.data[0] = data[0] + other.data[0];
        result.data[1] = data[1] + other.data[1];
        return result;
    }
    ComplexVector2 operator*(const Complex& scalar) const {
        ComplexVector2 result;
        result.data[0] = scalar * data[0];
        result.data[1] = scalar * data[1];
        return result;
    }
    public:
        ComplexVector2() {
            data = {{{0, 0}, {0, 0}}};
        };

        ComplexVector2(const std::array<Complex, 2>& arr)
        : data(arr) {};

};

inline ComplexVector2 operator*(Complex scalar, const ComplexVector2& V) {
    return V * scalar;
}
std::ostream& operator<<(std::ostream& os, const ComplexVector2& v) {
    os << "[" << v.data[0] << ", " << v.data[1] << "]";
    return os;
}

struct ComplexMatrix2x2{

    std::array<std::array<Complex, 2>, 2> data;

    ComplexVector2 operator*(const ComplexVector2& vec) const {
        ComplexVector2 result;
        result.data[0] = data[0][0]*vec.data[0] + data[0][1]*vec.data[1];
        result.data[1] = data[1][0]*vec.data[0] + data[1][1]*vec.data[1];
        return result;
    }

    ComplexMatrix2x2 operator*(const Complex& scalar) const {
        ComplexMatrix2x2 result;
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 2; ++j) {
                result.data[i][j] = scalar*data[i][j];
            }
        }
        return result;
    }

    ComplexMatrix2x2 operator+(const ComplexMatrix2x2& other) const {
        ComplexMatrix2x2 result;
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 2; ++j) {
                    result.data[i][j] = data[i][j] + other.data[i][j];
            }
        }
        return result;
    }
    ComplexMatrix2x2 operator*(const ComplexMatrix2x2& other) const {
        ComplexMatrix2x2 result;
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 2; ++j) {
                result.data[i][j] = Complex(0.0, 0.0);
                for (int k = 0; k < 2; ++k) {
                    result.data[i][j] += data[i][k] * other.data[k][j];
                }
            }
        }
    
        return result;
    }
    ComplexMatrix2x2 pow(unsigned int n) const {
        ComplexMatrix2x2 result(1, 0, 0, 1);
        ComplexMatrix2x2 base = *this;

        while (n > 0) {
            if (n % 2 == 1) result = result * base;
            base = base * base;
            n /= 2;
        }

        return result;
    }

    Complex trace() {
        return data[0][0] + data[1][1];
    }

    Complex determinant() {
        return (data[0][0]*data[1][1]) - (data[0][1]*data[1][0]);
    }


    std::pair<Complex, Complex> eigenvalues() {
        Complex tr = trace();
        Complex det = determinant();
        Complex delta = std::sqrt( tr*tr - (Complex(4)*det));
        auto lambda1 = 0.5*(-tr + delta);
        auto lambda2 = 0.5*(-tr - delta);
        return std::pair{lambda1, lambda2};
    }

    std::pair<ComplexVector2, ComplexVector2> eigenvectors() {
        auto lambdas = eigenvalues();
        Complex v2_1 = (lambdas.first - data[0][0]) / data[0][1];
        ComplexVector2 V1 = {{1.0, v2_1}};
        Complex v2_2 = (lambdas.second - data[0][0]) / data[0][1];
        ComplexVector2 V2 = {{1.0, v2_2}};
        V1.normalize();
        V2.normalize();
        return {V1, V2};
    }

    public:
        ComplexMatrix2x2() {
            data={{{0,0}, {0,0}}};
        }
        ComplexMatrix2x2(Complex a, Complex b, Complex c, Complex d){
            data={{{a, b}, {c, d}}};
        }
        ComplexMatrix2x2(const std::array<std::array<Complex, 2>, 2>& arr)
        : data(arr) {}

};

inline ComplexMatrix2x2 operator*(Complex scalar, const ComplexMatrix2x2& M) {
    return M * scalar;
}


inline std::ostream& operator<<(std::ostream& os, const ComplexMatrix2x2& m) {
    os << "[[" << m.data[0][0] << ", " << m.data[0][1] << "],\n"
       << " [" << m.data[1][0] << ", " << m.data[1][1] << "]]";
    return os;
}


ComplexMatrix2x2 Exp(ComplexMatrix2x2 A, size_t terms) {
    ComplexMatrix2x2 result;
    // taylor expansion
    for (size_t i = 0; i < terms; i++)
    {
        result = result + ((1/tgamma(i+1)))*(A.pow(i));
    }
    return result;
    
}


Complex inner_product(ComplexVector2 v1, ComplexVector2 v2) {
    auto conjugate = v1.conjugate();
    return conjugate.data[0]*v2.data[0] + conjugate.data[1]*v2.data[1];
}