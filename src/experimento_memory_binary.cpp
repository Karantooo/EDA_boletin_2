#include <iostream>
#include <cstdlib>
#include <cstring>
#include <random>
#include <limits>

#include "binary_heap.h"      // binary_heap<T> con push(T)
#include "binomial_heap.h"    // binomial_heap<T> con push(T)
#include "fibonacci.hpp"      // FibonacciHeap<T> con insert(T)

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Uso: " << argv[0] << " <-B|-b|-F> <n>\n";
        return 1;
    }

    const char* flag = argv[1];
    int n = std::atoi(argv[2]);
    if (n < 0) {
        std::cerr << "Error: n debe ser >= 0\n";
        return 1;
    }

    // Generador de números aleatorios (no determinista)
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distrib(
        std::numeric_limits<int>::min(),
        std::numeric_limits<int>::max()
    );

    if (std::strcmp(flag, "-B") == 0) {
        // Solo crear e insertar en binary_heap
        binary_heap<int> h;
        for (int i = 0; i < n; ++i) {
            int value = distrib(gen);
            h.push(value);
        }
    } else if (std::strcmp(flag, "-b") == 0) {
        // Solo crear e insertar en binomial_heap
        binomial_heap h;
        for (int i = 0; i < n; ++i) {
            int value = distrib(gen);
            h.push(value);
        }
    } else if (std::strcmp(flag, "-F") == 0) {
        // Solo crear e insertar en FibonacciHeap
        FibonacciHeap<int> h;
        for (int i = 0; i < n; ++i) {
            int value = distrib(gen);
            h.insert(value);
        }
    } else {
        std::cerr << "Flag inválido. Usa -B (binary), -b (binomial) o -F (fibonacci)\n";
        return 1;
    }

    // Sin impresiones: ideal para medir memoria con Valgrind/Massif
    return 0;
}
