# Quantum Circuit Simulator in C++

A small state-vector quantum circuit simulator written in C++17 with [Eigen](https://eigen.tuxfamily.org/). An n-qubit state is stored as all 2ⁿ complex amplitudes. Each gate updates the amplitudes in place using bit operations on the basis-state index, so no 2ⁿ × 2ⁿ matrix is ever built. Circuits are written with a chainable API:

```cpp
QuantumCircuit qc(2);
qc.h(0).cnot(0, 1);   // Bell state (|00⟩ + |11⟩)/√2
int result = qc.measure();
```

## Features

- **State vector:** n qubits stored as an `Eigen::VectorXcd` of length 2ⁿ, starting in |0…0⟩.
- **Single-qubit gates:** I, X, Y, Z, H, S, T.
- **Rotation gates:** RX(θ), RY(θ), RZ(θ), Phase(θ).
- **Controlled gates:** CNOT and CZ.
- **Measurement:**
  - `measure()` samples the full register from the Born-rule probabilities and collapses the state to that basis state.
  - `measureQubit(q)` measures one qubit, sets the amplitudes that disagree with the result to zero, and renormalises.
- **Inspection:** `getProbabilities()`, `printState()` (non-zero amplitudes only), and `printCircuit()` (the list of gates applied).

## Project layout

```
include/
  core/statevector.h      StateVector: amplitudes, gate application, measurement
  gates/gates.h           gate matrices (Gates::H, Gates::CNOT, Gates::RX(θ), ...)
  circuits/circuit.h      QuantumCircuit: chainable front end over StateVector
src/
  core/statevector.cpp
  gates/gates.cpp
  circuits/circuit.cpp
examples/
  basic_example.cpp       superposition, Bell state, measurement collapse
  bell_state.cpp          1000-shot Bell state statistics
tests/
  test_gates.cpp          GoogleTest setup (placeholder test)
CMakeLists.txt
```

## Building

**Requirements:**
- A C++17 compiler.
- CMake 3.15 or newer.
- Eigen 3.
- GoogleTest (optional; it's only needed for the tests).

**Install the dependencies:**

```bash
# macOS
brew install cmake eigen googletest

# Ubuntu / Debian
sudo apt install cmake libeigen3-dev libgtest-dev
```

**Build and run:**

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/basic_example
./build/bell_state
cd build && ctest
```

`-DCMAKE_BUILD_TYPE=Release` matters. The `-O3 -march=native` flags are only set for Release builds, so without it you get an unoptimised build.

## Example output

`basic_example`:

```
1. Creating superposition with Hadamard:
Quantum State:
|0⟩: (0.7071,0.0000)
|1⟩: (0.7071,0.0000)

2. Creating Bell State (maximal entanglement):
Quantum State:
|00⟩: (0.7071,0.0000)
|11⟩: (0.7071,0.0000)
Probabilities: [0.5000, 0.0000, 0.0000, 0.5000]

3. Measurement demonstration:
...
Measured: |11⟩
After measurement:
Quantum State:
|11⟩: (1.0000,0.0000)
```

`bell_state`, 1000 shots (the counts change from run to run):

```
|00⟩: 494 (49.4%)
|11⟩: 506 (50.6%)
```

Only |00⟩ and |11⟩ ever appear, which is what entanglement means here: the two qubits always agree.

## How it works

**Indexing.** Basis state |qₙ₋₁ … q₁q₀⟩ is stored at index i = Σ qₖ·2ᵏ. Qubit 0 is the least significant bit, and `printState()` prints qubit 0 on the right. So `x(0)` on two qubits gives |01⟩.

**Single-qubit gate on qubit t.** Every index i with bit t equal to 0 is paired with j = i | (1 << t), which is the same index with bit t set to 1. The 2×2 gate U is then applied to that pair:

- a′ᵢ = U₀₀·aᵢ + U₀₁·aⱼ
- a′ⱼ = U₁₀·aᵢ + U₁₁·aⱼ

That is 2ⁿ⁻¹ small 2×2 updates, so the cost is O(2ⁿ) per gate.

**Controlled gate (control c, target t).** Only the pairs where bit c is 1 are touched. The lower-right 2×2 block of the 4×4 matrix (rows and columns 2–3) is applied to those pairs. For CNOT that block is X, and for CZ it is Z.

**Measurement.** The probability of basis state i is pᵢ = |aᵢ|².
- `measure()` draws an outcome with `std::discrete_distribution` using a Mersenne Twister (`std::mt19937`).
- `measureQubit(q)` adds up the probability of every index with bit q = 0 to get P(0), samples 0 or 1, zeroes the amplitudes that disagree with the result, and renormalises.

## Performance

Memory is 16 bytes per amplitude (one `std::complex<double>`), so it doubles with every extra qubit:

| Qubits | State vector |
|---|---|
| 20 | 16 MB |
| 24 | 256 MB |
| 28 | 4 GB |
| 30 | 16 GB |

Each single-qubit gate also allocates a second vector of the same size. In a Release build on a cloud Linux VM, applying H to every qubit took about 0.15 s at 20 qubits and about 5.9 s at 24 qubits. Everything runs on one thread.

## Limitations

- **SWAP and other non-controlled two-qubit gates can't be applied correctly.** `Gates::SWAP` exists, but the two-qubit routine only uses the lower-right 2×2 block, so it only works for controlled-U gates. Calling `StateVector::applyGate(Gates::SWAP, 0, 1)` on |01⟩ zeroes the state instead of producing |10⟩. `QuantumCircuit` has no `swap()` method, so this only happens if you use `StateVector` directly. A SWAP can be built from three CNOTs.
- **Qubit indices aren't checked.** Passing a qubit ≥ n, or the same qubit as control and target, isn't caught.
- **The qubit count is limited by `int`.** `num_states_ = 1 << n` is an `int`, which overflows past 30 qubits, although memory runs out well before that.
- **The test suite is a placeholder.** `tests/test_gates.cpp` contains only `EXPECT_EQ(1, 1)`. The examples check behaviour by eye.
- **Not all gates are in the circuit API.** `Gates::Phase(θ)`, `Gates::SWAP` and `Gates::I` are defined but have no `QuantumCircuit` method.
- **No noise model, no density matrices, no multi-controlled gates.**

## Possible extensions

- Real unit tests: unitarity of every gate, H·H = I, Bell and GHZ amplitudes, and measurement statistics within tolerance.
- A general two-qubit gate routine (all four amplitudes per group), which fixes SWAP.
- In-place updates without the extra copy, and OpenMP across index pairs.
- Toffoli and other multi-controlled gates; a small algorithm demo such as Deutsch–Jozsa, Grover or the QFT.
