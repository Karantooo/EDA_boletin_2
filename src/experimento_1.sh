#!/bin/bash

# Cambiar al directorio padre
cd ..

OUTPUT="resultados_experimentos/resultados_experimento_heaps.csv"

# Añadir encabezado al archivo
echo "n,binario_insert,binomial_insert,fibonacci_insert,binario_extract,binomial_extract,fibonacci_extract" > $OUTPUT

# Número de repeticiones
REPS=32

# Bucle de repeticiones
for ((r=1; r<=REPS; r++)); do
    echo "Repetición $r de $REPS"
    # Bucle de tamaños
    for ((n=1000; n<=100000; n+=1000)); do
        ./experimento_1.out $n >> $OUTPUT
    done
done
