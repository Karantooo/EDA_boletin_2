#include <iostream>
#include <vector>
#include <random>
#include <chrono>

// Headers provistos por el usuario
#include "binary_heap.h"      // se asume: binary_heap<T> con push(T) y pop()
#include "binomial_heap.h"    // se asume: binomial_heap<T> con push(T) y pop()
#include "fibonacci.hpp"        // se asume: FibonacciHeap<T> con insert(T) y extractMin()

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

    // Generador determinístico para reproducibilidad
    std::random_device rd;
    std::mt19937 gen(rd());
    uniform_int_distribution<int> dist(1, 1'000'000'000);

    vector<int> datos;
    datos.reserve(n);
    for (size_t i = 0; i < n; ++i) datos.push_back(dist(gen));

    long long t_ins_bin = 0, t_ins_bino = 0, t_ins_fibo = 0;
    long long t_ext_bin = 0, t_ext_bino = 0, t_ext_fibo = 0;

    // ---- Binary Heap (push + pop) ----
    {
        binary_heap<int> bh;
        auto t0 = steady_clock::now();
        for (size_t i = 0; i < n; ++i) bh.push(datos[i]);
        auto t1 = steady_clock::now();
        t_ins_bin = duration_cast<microseconds>(t1 - t0).count();

        t0 = steady_clock::now();
        for (size_t i = 0; i < n; ++i) bh.pop();
        t1 = steady_clock::now();
        t_ext_bin = duration_cast<microseconds>(t1 - t0).count();
    }

    // ---- Binomial Heap (push + pop) ----
    {
        binomial_heap bino;
        auto t0 = steady_clock::now();
        for (size_t i = 0; i < n; ++i) bino.push(datos[i]);
        auto t1 = steady_clock::now();
        t_ins_bino = duration_cast<microseconds>(t1 - t0).count();

        t0 = steady_clock::now();
        for (size_t i = 0; i < n; ++i) bino.pop();
        t1 = steady_clock::now();
        t_ext_bino = duration_cast<microseconds>(t1 - t0).count();
    }

    // ---- Fibonacci Heap (insert + extractMin) ----
    {
        FibonacciHeap<int> fibo;
        auto t0 = steady_clock::now();
        for (size_t i = 0; i < n; ++i) fibo.insert(datos[i]);
        auto t1 = steady_clock::now();
        t_ins_fibo = duration_cast<microseconds>(t1 - t0).count();

        t0 = steady_clock::now();
        for (size_t i = 0; i < n; ++i) fibo.removeMinimum();
        t1 = steady_clock::now();
        t_ext_fibo = duration_cast<microseconds>(t1 - t0).count();
    }

    // CSV sin encabezado:
    // n, tiempo_binary_insert_ms, tiempo_binomial_insert_ms, tiempo_fibonacci_insert_ms, tiempo_binary_extract_ms, tiempo_binomial_extract_ms, tiempo_fibonacci_extract_ms
    cout << n << ","
         << t_ins_bin  << ","
         << t_ins_bino << ","
         << t_ins_fibo << ","
         << t_ext_bin  << ","
         << t_ext_bino << ","
         << t_ext_fibo << "\n";

    return 0;
}