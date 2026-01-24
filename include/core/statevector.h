#ifndef STATEVECTOR_H
#define STATEVECTOR_H

#include <Eigen/Dense>
#include <complex>
#include <vector>
#include <iostream>
#include <random>

using Complex = std::complex<double>;
using StateVec = Eigen::VectorXcd;
using Matrix = Eigen::MatrixXcd;

class StateVector {
private:
    int num_qubits_;
    int num_states_;
    StateVec amplitudes_;
    std::mt19937 rng_;

public:
    // Constructor
    explicit StateVector(int num_qubits);
    
    // Apply gates
    void applyGate(const Matrix& gate, int target);
    void applyGate(const Matrix& gate, int control, int target);
    
    // Measurement
    int measure();
    int measureQubit(int qubit);
    
    // Getters
    const StateVec& getAmplitudes() const { return amplitudes_; }
    std::vector<double> getProbabilities() const;
    int getNumQubits() const { return num_qubits_; }
    
    // Display
    void print() const;
    
private:
    void applySingleQubitGate(const Matrix& gate, int target);
    void applyTwoQubitGate(const Matrix& gate, int control, int target);
};

#endif // STATEVECTOR_H
