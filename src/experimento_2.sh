#!/bin/bash
set -euo pipefail

# Cambiar al directorio padre (ajusta si no corresponde)
cd ..

# Archivo de salida
OUTPUT="resultados_experimentos/resultados_experimento_2.csv"

# Asegurar carpeta destino
mkdir -p "$(dirname "$OUTPUT")"

# Encabezado CONSISTENTE con la salida del programa (usa ;)
echo "ratio;binario_insert;binomial_insert;fibonacci_insert;binario_extract;binomial_extract;fibonacci_extract;cnt_insert;cnt_extract" > "$OUTPUT"

# Número de repeticiones
REPS=32

for ((r=1; r<=REPS; r++)); do
  echo "Repetición $r de $REPS"
  # Ratios: 0.50, 0.55, ..., 1.00 (paso 0.05)
  for ((i=50; i<=100; i+=5)); do
    ratio=$(awk -v x="$i" 'BEGIN{printf "%.2f", x/100}')
    ./experimento_2.out "$ratio" >> "$OUTPUT"
  done
done
