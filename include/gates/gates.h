#ifndef GATES_H
#define GATES_H

#include <Eigen/Dense>
#include <complex>
#include <cmath>

using Complex = std::complex<double>;
using Matrix = Eigen::MatrixXcd;

namespace Gates {
    // Single-qubit gates
    extern const Matrix I;  // Identity
    extern const Matrix X;  // Pauli-X (NOT)
    extern const Matrix Y;  // Pauli-Y
    extern const Matrix Z;  // Pauli-Z
    extern const Matrix H;  // Hadamard
    extern const Matrix S;  // Phase gate
    extern const Matrix T;  // π/8 gate
    
    // Two-qubit gates
    extern const Matrix CNOT;  // Controlled-NOT
    extern const Matrix CZ;    // Controlled-Z
    extern const Matrix SWAP;  // SWAP
    
    // Parameterized gates
    Matrix RX(double theta);
    Matrix RY(double theta);
    Matrix RZ(double theta);
    Matrix Phase(double theta);
}

#endif // GATES_H
