#include "core/statevector.h"
#include <cmath>
#include <iomanip>

StateVector::StateVector(int num_qubits) 
    : num_qubits_(num_qubits), 
      num_states_(1 << num_qubits),
      rng_(std::random_device{}()) {
    
    // Initialize to |0...0⟩ state
    amplitudes_ = StateVec::Zero(num_states_);
    amplitudes_(0) = Complex(1.0, 0.0);
}

void StateVector::applyGate(const Matrix& gate, int target) {
    applySingleQubitGate(gate, target);
}

void StateVector::applyGate(const Matrix& gate, int control, int target) {
    applyTwoQubitGate(gate, control, target);
}

void StateVector::applySingleQubitGate(const Matrix& gate, int target) {
    StateVec new_state = StateVec::Zero(num_states_);
    
    for (int i = 0; i < num_states_; ++i) {
        // Check if target qubit is 0
        if (((i >> target) & 1) == 0) {
            int j = i | (1 << target);  // Flip target qubit to get |1⟩ state
            
            new_state(i) += gate(0, 0) * amplitudes_(i) + gate(0, 1) * amplitudes_(j);
            new_state(j) += gate(1, 0) * amplitudes_(i) + gate(1, 1) * amplitudes_(j);
        }
    }
    
    amplitudes_ = new_state;
}

void StateVector::applyTwoQubitGate(const Matrix& gate, int control, int target) {
    StateVec new_state = amplitudes_;
    
    for (int i = 0; i < num_states_; ++i) {
        // Only apply if control qubit is 1
        if (((i >> control) & 1) == 1) {
            if (((i >> target) & 1) == 0) {
                int j = i | (1 << target);  // Flip target qubit
                
                Complex amp_i = amplitudes_(i);
                Complex amp_j = amplitudes_(j);
                
                new_state(i) = gate(2, 2) * amp_i + gate(2, 3) * amp_j;
                new_state(j) = gate(3, 2) * amp_i + gate(3, 3) * amp_j;
            }
        }
    }
    
    amplitudes_ = new_state;
}

int StateVector::measure() {
    std::vector<double> probs = getProbabilities();
    std::discrete_distribution<int> dist(probs.begin(), probs.end());
    
    int outcome = dist(rng_);
    
    // Collapse to measured state
    amplitudes_ = StateVec::Zero(num_states_);
    amplitudes_(outcome) = Complex(1.0, 0.0);
    
    return outcome;
}

int StateVector::measureQubit(int qubit) {
    std::vector<double> probs = getProbabilities();
    
    double prob_0 = 0.0;
    for (int i = 0; i < num_states_; ++i) {
        if (((i >> qubit) & 1) == 0) {
            prob_0 += probs[i];
        }
    }
    
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    int outcome = (dist(rng_) < prob_0) ? 0 : 1;
    
    // Collapse state
    double norm = 0.0;
    for (int i = 0; i < num_states_; ++i) {
        if (((i >> qubit) & 1) != outcome) {
            amplitudes_(i) = Complex(0.0, 0.0);
        } else {
            norm += std::norm(amplitudes_(i));
        }
    }
    
    amplitudes_ /= std::sqrt(norm);
    
    return outcome;
}

std::vector<double> StateVector::getProbabilities() const {
    std::vector<double> probs(num_states_);
    for (int i = 0; i < num_states_; ++i) {
        probs[i] = std::norm(amplitudes_(i));
    }
    return probs;
}

void StateVector::print() const {
    std::cout << "Quantum State:\n";
    for (int i = 0; i < num_states_; ++i) {
        if (std::abs(amplitudes_(i)) > 1e-10) {
            std::cout << "|";
            for (int q = num_qubits_ - 1; q >= 0; --q) {
                std::cout << ((i >> q) & 1);
            }
            std::cout << "⟩: " << std::fixed << std::setprecision(4) 
                      << amplitudes_(i) << "\n";
        }
    }
}
