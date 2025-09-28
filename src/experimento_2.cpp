#include <iostream>
#include <random>
#include <chrono>
#include <cstdint>

// === Headers reales (mismos que en experimento_1.cpp) ===
#include "binary_heap.h"      // binary_heap<T> con push(T) y pop()
#include "binomial_heap.h"    // binomial_heap<T> con push(T) y pop()
#include "fibonacci.hpp"      // FibonacciHeap<T> con insert(T) y extractMin()

using namespace std;
using namespace std::chrono;

static inline uint64_t now_ns() {
    return duration_cast<nanoseconds>(steady_clock::now().time_since_epoch()).count();
}

int main(int argc, char* argv[]) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (argc != 2) {
        cerr << "Uso: " << argv[0] << " <ratio_inserts 0..1>"
             << "Ejemplo: ./experimento_2.out 0.6";
        return 1;
    }

    // Prob. de INSERT
    double p_insert;
    try {
        p_insert = stod(argv[1]);
    } catch (...) {
        cerr << "Error: ratio inválido" << ' ';
        return 1;
    }
    if (!isfinite(p_insert)) { cerr << "Error: ratio no finito" << ' '; return 1; }
    if (p_insert < 0.0) p_insert = 0.0;
    if (p_insert > 1.0) p_insert = 1.0;

    constexpr uint64_t TOTAL_OPS = 1'000'000ULL;

    // RNG determinista
    mt19937_64 rng(0xC0FFEEULL);
    uniform_real_distribution<double> U01(0.0, 1.0);
    uniform_int_distribution<int> Uval(numeric_limits<int>::min()/2,
                                       numeric_limits<int>::max()/2);

    // Instancias reales (templadas) según el contrato de experimento_1.cpp
    binary_heap<int>       heap_binario;   // push(int), pop()
    binomial_heap     heap_binomial;  // push(int), pop()
    FibonacciHeap<int>     heap_fibonacci; // insert(int), extractMin()

    // Cantidad sincronizada de elementos (para evitar extract en vacío)
    uint64_t current_size = 0;

    // Formato de salida: operacion;tiempo_ns;heap
    // Ej.: insert;137;binary_heap

    for (uint64_t i = 0; i < TOTAL_OPS; ++i) {
        bool do_insert = (U01(rng) < p_insert);

        if (do_insert) {
            int key = Uval(rng); // mismo valor para los 3 heaps

            uint64_t t0 = now_ns();
            heap_binario.push(key);
            uint64_t t1 = now_ns();
            cout << "insert;" << (t1 - t0) << ";binary_heap\n";

            t0 = now_ns();
            heap_binomial.push(key);
            t1 = now_ns();
            cout << "insert;" << (t1 - t0) << ";binomial_heap\n";

            t0 = now_ns();
            heap_fibonacci.insert(key);
            t1 = now_ns();
            cout << "insert;" << (t1 - t0) << ";fibonacci_heap\n";

            ++current_size;
        } else {
            if (current_size == 0) continue; // evitar pop en vacío

            uint64_t t0 = now_ns();
            heap_binario.pop();
            uint64_t t1 = now_ns();
            cout << "extract_min;" << (t1 - t0) << ";binary_heap\n";

            t0 = now_ns();
            heap_binomial.pop();
            t1 = now_ns();
            cout << "extract_min;" << (t1 - t0) << ";binomial_heap\n";

            t0 = now_ns();
            heap_fibonacci.removeMinimum();
            t1 = now_ns();
            cout << "extract_min;" << (t1 - t0) << ";fibonacci_heap\n";

            --current_size;
        }
    }

    return 0;
}