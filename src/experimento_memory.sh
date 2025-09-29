#!/bin/bash
set -euo pipefail

EXEC="../experimento_memory_binary.out"             # Cambia si tu binario tiene otro nombre
OUTPUT_FILE="resultados_memoria_binary_heap.csv"
sizes=(1000 5000 10000 50000 100000 250000 500000 1000000)

# Flags y nombres legibles para la columna "estructura"
flags=("-B" "-b" "-F")
names=("binary" "binomial" "fibonacci")

echo "estructura;n;allocs;frees;bytes_allocated" > "$OUTPUT_FILE"

for i in "${!flags[@]}"; do
  flag="${flags[$i]}"
  nombre="${names[$i]}"

  for n in "${sizes[@]}"; do
    # Forzamos inglés para el mensaje "total heap usage"
    # Tomamos solo la PRIMERA coincidencia con -m1
    line=$(
      LANG=C valgrind "$EXEC" "$flag" "$n" 2>&1 \
        | grep -m1 "total heap usage:" || true
    )

    if [[ -z "$line" ]]; then
      echo "WARN: No se encontró 'total heap usage' para estructura=$nombre n=$n" >&2
      continue
    fi

    # Quitar prefijo ==PID== y comas
    clean=$(echo "$line" \
              | sed -E 's/^==[0-9]+==[[:space:]]*//' \
              | sed 's/,//g')

    # Extraer solo los números en orden (allocs, frees, bytes)
    mapfile -t nums < <(echo "$clean" | grep -oE '[0-9]+')

    # Sanidad: esperamos 3 números
    if (( ${#nums[@]} < 3 )); then
      echo "WARN: No se pudieron parsear números para estructura=$nombre n=$n" >&2
      continue
    fi

    allocs="${nums[0]}"
    frees="${nums[1]}"
    bytes="${nums[2]}"

    echo "${nombre};${n};${allocs};${frees};${bytes}" | tee -a "$OUTPUT_FILE"
  done
done

echo "✅ Resultados guardados en: $OUTPUT_FILE"
