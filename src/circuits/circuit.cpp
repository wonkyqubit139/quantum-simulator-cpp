#include "circuits/circuit.h"

QuantumCircuit::QuantumCircuit(int num_qubits) 
    : num_qubits_(num_qubits), 
      state_(std::make_unique<StateVector>(num_qubits)) {}

QuantumCircuit& QuantumCircuit::h(int qubit) {
    state_->applyGate(Gates::H, qubit);
    gate_history_.push_back("H(" + std::to_string(qubit) + ")");
    return *this;
}

QuantumCircuit& QuantumCircuit::x(int qubit) {
    state_->applyGate(Gates::X, qubit);
    gate_history_.push_back("X(" + std::to_string(qubit) + ")");
    return *this;
}

QuantumCircuit& QuantumCircuit::y(int qubit) {
    state_->applyGate(Gates::Y, qubit);
    gate_history_.push_back("Y(" + std::to_string(qubit) + ")");
    return *this;
}

QuantumCircuit& QuantumCircuit::z(int qubit) {
    state_->applyGate(Gates::Z, qubit);
    gate_history_.push_back("Z(" + std::to_string(qubit) + ")");
    return *this;
}

QuantumCircuit& QuantumCircuit::s(int qubit) {
    state_->applyGate(Gates::S, qubit);
    gate_history_.push_back("S(" + std::to_string(qubit) + ")");
    return *this;
}

QuantumCircuit& QuantumCircuit::t(int qubit) {
    state_->applyGate(Gates::T, qubit);
    gate_history_.push_back("T(" + std::to_string(qubit) + ")");
    return *this;
}

QuantumCircuit& QuantumCircuit::rx(int qubit, double theta) {
    state_->applyGate(Gates::RX(theta), qubit);
    gate_history_.push_back("RX(" + std::to_string(qubit) + ", " + std::to_string(theta) + ")");
    return *this;
}

QuantumCircuit& QuantumCircuit::ry(int qubit, double theta) {
    state_->applyGate(Gates::RY(theta), qubit);
    gate_history_.push_back("RY(" + std::to_string(qubit) + ", " + std::to_string(theta) + ")");
    return *this;
}

QuantumCircuit& QuantumCircuit::rz(int qubit, double theta) {
    state_->applyGate(Gates::RZ(theta), qubit);
    gate_history_.push_back("RZ(" + std::to_string(qubit) + ", " + std::to_string(theta) + ")");
    return *this;
}

QuantumCircuit& QuantumCircuit::cnot(int control, int target) {
    state_->applyGate(Gates::CNOT, control, target);
    gate_history_.push_back("CNOT(" + std::to_string(control) + ", " + std::to_string(target) + ")");
    return *this;
}

QuantumCircuit& QuantumCircuit::cz(int control, int target) {
    state_->applyGate(Gates::CZ, control, target);
    gate_history_.push_back("CZ(" + std::to_string(control) + ", " + std::to_string(target) + ")");
    return *this;
}

int QuantumCircuit::measure() {
    return state_->measure();
}

int QuantumCircuit::measureQubit(int qubit) {
    return state_->measureQubit(qubit);
}

std::vector<double> QuantumCircuit::getProbabilities() const {
    return state_->getProbabilities();
}

void QuantumCircuit::printState() const {
    state_->print();
}

void QuantumCircuit::printCircuit() const {
    std::cout << "Circuit with " << num_qubits_ << " qubits:\n";
    for (const auto& gate : gate_history_) {
        std::cout << "  " << gate << "\n";
    }
}
