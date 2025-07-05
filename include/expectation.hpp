#pragma once
#include "linalg.hpp"
#include <cassert>


double expectation(ComplexMatrix2x2 Op, ComplexVector2 state) {
    // <\psi|O|\psi>
    Complex expectation = inner_product(state, Op*state);
    // assert(std::abs(expectation.imag()) < 1e-12);
    return expectation.real();
}