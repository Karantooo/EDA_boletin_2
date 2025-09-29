# Boletín 2 – Estructuras de Datos Avanzadas

Este proyecto corresponde al **Boletín 2** del curso *Estructuras de Datos y Algoritmos Avanzados*.  
El objetivo es **comparar distintas implementaciones de heaps** y evaluar su desempeño en operaciones de inserción y extracción mínima:

- **Binary Heap**  
- **Binomial Heap**  
- **Fibonacci Heap**  

Se incluyen tanto el **informe en PDF** como el **código en C++** y los **datasets generados**.

---

## ⚙️ Requisitos

- **Compilador C++17 o superior** (probado con `g++` en Linux).

---

## ▶️ Cómo ejecutar

1. **Compilar el proyecto**
   Desde la carpeta raíz, ejecutar:

   ```bash
   make
Esto generará los ejecutables correspondientes a los distintos experimentos, por ejemplo:

- `experimento_1.out`  
- `experimento_2.out`  
- `experimento_memoria.out`
### 2. Ejecutar experimentos manualmente

#### Experimento 1 (insertar y luego extraer `n` elementos):

```bash
./experimento_1.out <n>
```
Ejemplo:

```bash
./experimento_1.out 100000
```
#### Experimento 2 (operaciones con probabilidad de insert/extract):
```bash
./experimento_2.out <ratio_inserts>
```

Ejemplo:
```bash
./experimento_2.out 0.6
```
> Donde 0.6 significa un 60% de operaciones insert y un 40% de operaciones extract_min.
#### Experimento de memoria (insertar `n` elementos y medir uso de RAM):

```bash
./experimento_memoria.out <n>
```
Ejemplo:
```bash
./experimento_memoria.out 50000
```
### 3. Recrear los experimentos completos

Una vez compilados los ejecutables, se pueden usar los scripts `.sh` incluidos en el proyecto para automatizar múltiples corridas y generar los archivos `.csv`.
