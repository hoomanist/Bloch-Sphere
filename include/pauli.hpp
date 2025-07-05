#pragma once
#include "linalg.hpp"


namespace Pauli {
    inline const ComplexMatrix2x2 I(1, 0, 0, 1);
    inline const ComplexMatrix2x2 X(0, 1, 1, 0);
    inline const ComplexMatrix2x2 Y(
        0, Complex(0, -1),
        Complex(0, 1), 0
    );
    inline const ComplexMatrix2x2 Z(1, 0, 0, -1);
}
