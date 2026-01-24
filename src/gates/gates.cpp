#include "gates/gates.h"

namespace Gates {
    const Complex i(0.0, 1.0);
    const double sqrt2 = std::sqrt(2.0);
    
    // Identity
    const Matrix I = (Matrix(2, 2) << 
        1, 0,
        0, 1
    ).finished();
    
    // Pauli-X (NOT gate)
    const Matrix X = (Matrix(2, 2) << 
        0, 1,
        1, 0
    ).finished();
    
    // Pauli-Y
    const Matrix Y = (Matrix(2, 2) << 
        0, -i,
        i, 0
    ).finished();
    
    // Pauli-Z
    const Matrix Z = (Matrix(2, 2) << 
        1, 0,
        0, -1
    ).finished();
    
    // Hadamard
    const Matrix H = (Matrix(2, 2) << 
        1.0/sqrt2, 1.0/sqrt2,
        1.0/sqrt2, -1.0/sqrt2
    ).finished();
    
    // S gate (Phase)
    const Matrix S = (Matrix(2, 2) << 
        1, 0,
        0, i
    ).finished();
    
    // T gate
    const Matrix T = (Matrix(2, 2) << 
        1, 0,
        0, std::exp(i * M_PI / 4.0)
    ).finished();
    
    // CNOT gate
    const Matrix CNOT = (Matrix(4, 4) << 
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 0, 1,
        0, 0, 1, 0
    ).finished();
    
    // CZ gate
    const Matrix CZ = (Matrix(4, 4) << 
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, -1
    ).finished();
    
    // SWAP gate
    const Matrix SWAP = (Matrix(4, 4) << 
        1, 0, 0, 0,
        0, 0, 1, 0,
        0, 1, 0, 0,
        0, 0, 0, 1
    ).finished();
    
    // Rotation gates
    Matrix RX(double theta) {
        Matrix gate(2, 2);
        gate << std::cos(theta/2), -i * std::sin(theta/2),
                -i * std::sin(theta/2), std::cos(theta/2);
        return gate;
    }
    
    Matrix RY(double theta) {
        Matrix gate(2, 2);
        gate << std::cos(theta/2), -std::sin(theta/2),
                std::sin(theta/2), std::cos(theta/2);
        return gate;
    }
    
    Matrix RZ(double theta) {
        Matrix gate(2, 2);
        gate << std::exp(-i * theta / 2.0), 0,
                0, std::exp(i * theta / 2.0);
        return gate;
    }
    
    Matrix Phase(double theta) {
        Matrix gate(2, 2);
        gate << 1, 0,
                0, std::exp(i * theta);
        return gate;
    }
}
