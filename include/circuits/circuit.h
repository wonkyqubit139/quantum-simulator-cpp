#ifndef CIRCUIT_H
#define CIRCUIT_H

#include "core/statevector.h"
#include "gates/gates.h"
#include <memory>
#include <vector>
#include <string>

class QuantumCircuit {
private:
    int num_qubits_;
    std::unique_ptr<StateVector> state_;
    std::vector<std::string> gate_history_;

public:
    explicit QuantumCircuit(int num_qubits);
    
    // Single-qubit gates
    QuantumCircuit& h(int qubit);
    QuantumCircuit& x(int qubit);
    QuantumCircuit& y(int qubit);
    QuantumCircuit& z(int qubit);
    QuantumCircuit& s(int qubit);
    QuantumCircuit& t(int qubit);
    
    // Rotation gates
    QuantumCircuit& rx(int qubit, double theta);
    QuantumCircuit& ry(int qubit, double theta);
    QuantumCircuit& rz(int qubit, double theta);
    
    // Two-qubit gates
    QuantumCircuit& cnot(int control, int target);
    QuantumCircuit& cz(int control, int target);
    
    // Measurement
    int measure();
    int measureQubit(int qubit);
    
    // State inspection
    std::vector<double> getProbabilities() const;
    void printState() const;
    void printCircuit() const;
};

#endif // CIRCUIT_H
