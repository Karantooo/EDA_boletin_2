#include <iostream>
#include <vector>
#include <random>
#include <chrono>

// Headers provistos por ti
#include "binary_heap.h"      // class template: binary_heap<T> con push(T)
#include "binomial_heap.h"    // class template: binomial_heap<T> con push(T)
#include "fibonacci.hpp"        // class template: FibonacciHeap<T> con insert(T)

int main(int argc, char* argv[]) {
    using namespace std;
    using namespace std::chrono;

    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " <numero_elementos>\n";
        return 1;
    }

    size_t n = 0;
    try {
        n = stoull(argv[1]);
    } catch (...) {
        cerr << "Error: <numero_elementos> debe ser un entero positivo.\n";
        return 1;
    }

    // Generador de números aleatorios (determinístico para reproducibilidad)
    mt19937 gen();
    uniform_int_distribution<int> dist(1, 1'000'000'000);

    vector<int> datos;
    datos.reserve(n);
    for (size_t i = 0; i < n; ++i) datos.push_back(dist(gen));

    long long tiempo_binary = 0;
    long long tiempo_binomial = 0;
    long long tiempo_fibonacci = 0;

    // ---- Binary Heap (push) ----
    {
        binary_heap<int> bh;
        auto t0 = steady_clock::now();
        for (size_t i = 0; i < n; ++i) {
            bh.push(datos[i]);
        }
        auto t1 = steady_clock::now();
        tiempo_binary = duration_cast<milliseconds>(t1 - t0).count();
    }

    // ---- Binomial Heap (push) ----
    {
        binomial_heap bino;
        auto t0 = steady_clock::now();
        for (size_t i = 0; i < n; ++i) {
            bino.push(datos[i]);
        }
        auto t1 = steady_clock::now();
        tiempo_binomial = duration_cast<milliseconds>(t1 - t0).count();
    }

    // ---- Fibonacci Heap (insert) ----
    {
        FibonacciHeap<int> fibo;
        auto t0 = steady_clock::now();
        for (size_t i = 0; i < n; ++i) {
            fibo.insert(datos[i]);
        }
        auto t1 = steady_clock::now();
        tiempo_fibonacci = duration_cast<milliseconds>(t1 - t0).count();
    }

    // ---- Salida CSV en una sola línea ----
    cout << n << "," 
         << tiempo_binary << "," 
         << tiempo_binomial << "," 
         << tiempo_fibonacci << "\n";

    return 0;
}
