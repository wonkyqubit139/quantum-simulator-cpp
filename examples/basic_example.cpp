#include "circuits/circuit.h"
#include <iostream>

int main() {
    std::cout << "=== Basic Quantum Simulator Example ===\n\n";
    
    // Example 1: Single qubit superposition
    std::cout << "1. Creating superposition with Hadamard:\n";
    QuantumCircuit qc1(1);
    qc1.h(0);
    qc1.printState();
    std::cout << "\n";
    
    // Example 2: Bell State
    std::cout << "2. Creating Bell State (maximal entanglement):\n";
    QuantumCircuit qc2(2);
    qc2.h(0).cnot(0, 1);
    qc2.printState();
    
    auto probs = qc2.getProbabilities();
    std::cout << "Probabilities: [";
    for (size_t i = 0; i < probs.size(); ++i) {
        std::cout << probs[i];
        if (i < probs.size() - 1) std::cout << ", ";
    }
    std::cout << "]\n\n";
    
    // Example 3: Measurement
    std::cout << "3. Measurement demonstration:\n";
    QuantumCircuit qc3(2);
    qc3.h(0).cnot(0, 1);
    
    std::cout << "Before measurement:\n";
    qc3.printState();
    
    int result = qc3.measure();
    std::cout << "\nMeasured: |" << (result >> 1) << (result & 1) << "⟩\n";
    std::cout << "After measurement:\n";
    qc3.printState();
    
    return 0;
}
