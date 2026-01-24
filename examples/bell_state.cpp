#include "circuits/circuit.h"
#include <iostream>
#include <map>

int main() {
    std::cout << "=== Bell State Statistics ===\n\n";
    
    const int num_shots = 1000;
    std::map<int, int> counts;
    
    for (int shot = 0; shot < num_shots; ++shot) {
        QuantumCircuit qc(2);
        qc.h(0).cnot(0, 1);
        int result = qc.measure();
        counts[result]++;
    }
    
    std::cout << "Results from " << num_shots << " measurements:\n";
    for (const auto& [state, count] : counts) {
        std::cout << "|" << (state >> 1) << (state & 1) << "⟩: " 
                  << count << " (" << (100.0 * count / num_shots) << "%)\n";
    }
    
    return 0;
}
