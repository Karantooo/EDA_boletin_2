#include <iostream>
#include <random>
#include <chrono>
#include <cstdint>
#include <limits>
#include <cmath>
#include <iomanip>

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
             << "\nEjemplo: ./experimento_2.out 0.6\n";
        return 1;
    }

    // Prob. de INSERT
    double p_insert;
    try {
        p_insert = stod(argv[1]);
    } catch (...) {
        cerr << "Error: ratio inválido" << '\n';
        return 1;
    }
    if (!std::isfinite(p_insert)) { cerr << "Error: ratio no finito" << '\n'; return 1; }
    if (p_insert < 0.0) p_insert = 0.0;
    if (p_insert > 1.0) p_insert = 1.0;

    constexpr uint64_t TOTAL_OPS = 1'000'000ULL;

    // RNG determinista
    mt19937_64 rng(std::random_device{}());
    uniform_real_distribution<double> U01(0.0, 1.0);
    uniform_int_distribution<int> Uval(numeric_limits<int>::min()/2,
                                       numeric_limits<int>::max()/2);

    // Instancias reales (templadas) según el contrato de experimento_1.cpp
    binary_heap<int>    heap_binario;     // push(int), pop()
    binomial_heap  heap_binomial;    // push(int), pop()
    FibonacciHeap<int>  heap_fibonacci;   // insert(int), extractMin()/removeMinimum()

    // Cantidad sincronizada de elementos (para evitar extract en vacío)
    uint64_t current_size = 0;

    // Acumuladores de tiempo (en ns)
    uint64_t sum_bin_insert = 0, sum_bino_insert = 0, sum_fib_insert = 0;
    uint64_t sum_bin_extract = 0, sum_bino_extract = 0, sum_fib_extract = 0;

    // Contadores de operaciones efectivas
    uint64_t cnt_insert = 0, cnt_extract = 0;

    for (uint64_t i = 0; i < TOTAL_OPS; ++i) {
        bool do_insert = (U01(rng) < p_insert);

        if (do_insert) {
            int key = Uval(rng); // mismo valor para los 3 heaps

            uint64_t t0 = now_ns();
            heap_binario.push(key);
            uint64_t t1 = now_ns();
            sum_bin_insert += (t1 - t0);

            t0 = now_ns();
            heap_binomial.push(key);
            t1 = now_ns();
            sum_bino_insert += (t1 - t0);

            t0 = now_ns();
            heap_fibonacci.insert(key);
            t1 = now_ns();
            sum_fib_insert += (t1 - t0);

            ++current_size;
            ++cnt_insert;
        } else {
            if (current_size == 0) continue; // evitar pop en vacío

            uint64_t t0 = now_ns();
            heap_binario.pop();
            uint64_t t1 = now_ns();
            sum_bin_extract += (t1 - t0);

            t0 = now_ns();
            heap_binomial.pop();
            t1 = now_ns();
            sum_bino_extract += (t1 - t0);

            t0 = now_ns();
            // Algunas implementaciones usan extractMin(); otras removeMinimum();
            heap_fibonacci.removeMinimum();
            t1 = now_ns();
            sum_fib_extract += (t1 - t0);

            --current_size;
            ++cnt_extract;
        }
    }

    // Promedios en ns (si no hubo operaciones de un tipo, promedio = 0)
    auto avg = [](uint64_t sum, uint64_t cnt) -> double {
        return (cnt == 0) ? 0.0 : static_cast<double>(sum) / static_cast<double>(cnt);
    };

    double avg_bin_ins  = avg(sum_bin_insert,  cnt_insert);
    double avg_bino_ins = avg(sum_bino_insert, cnt_insert);
    double avg_fib_ins  = avg(sum_fib_insert,  cnt_insert);

    double avg_bin_ext  = avg(sum_bin_extract,  cnt_extract);
    double avg_bino_ext = avg(sum_bino_extract, cnt_extract);
    double avg_fib_ext  = avg(sum_fib_extract,  cnt_extract);

    // Salida única por stdout:
    // <ratio>;<avg binary insert>;<avg binomial insert>;<avg fibonacci insert>;<avg binary extract>;<avg binomial extract>;<avg fibonacci extract>
    cout.setf(std::ios::fixed); cout << setprecision(3);
    cout << p_insert << ';'
         << ' ' << avg_bin_ins  << ';'
         << ' ' << avg_bino_ins << ';'
         << ' ' << avg_fib_ins  << ';'
         << ' ' << avg_bin_ext  << ';'
         << ' ' << avg_bino_ext << ';'
         << ' ' << avg_fib_ext  << ";"
         << ' ' << cnt_insert << ';'
         << ' ' << cnt_extract << '\n';

    return 0;
}
