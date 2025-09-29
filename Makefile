# =========================
#  Compilador y Flags
# =========================
CXX := g++

# Includes + warnings + opciones (ajusta a gusto)
CXXFLAGS_BASE        := -I ./include -Wall
CXXFLAGS_WARNINGS    := -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion -Wfloat-equal
CXXFLAGS_OPTIMIZATIONS := -O2 -march=native -mtune=native
CXXFLAGS_DEBUGGING   := -g # -ggdb

# Perfiles
CXXFLAGS_RELEASE := $(CXXFLAGS_BASE) $(CXXFLAGS_OPTIMIZATIONS) # $(CXXFLAGS_WARNINGS)
CXXFLAGS_DEBUG   := $(CXXFLAGS_BASE) $(CXXFLAGS_DEBUGGING)     # $(CXXFLAGS_WARNINGS)

# Perfil por defecto (cambia a RELEASE si quieres)
CXXFLAGS := $(CXXFLAGS_DEBUG)

# =========================
#  Rutas y patrones
# =========================
SRC_DIR := ./src
OBJ_DIR := ./build

# Solo toma como "experimentos" los archivos que matchean este patrón
EXPERIMENT_SOURCES := $(wildcard $(SRC_DIR)/experimento_*.cpp)
EXPERIMENT_NAMES   := $(notdir $(basename $(EXPERIMENT_SOURCES)))

# NUEVO (mínimo cambio): también tomar "script_*.cpp"
SCRIPT_SOURCES := $(wildcard $(SRC_DIR)/script_*.cpp)
SCRIPT_NAMES   := $(notdir $(basename $(SCRIPT_SOURCES)))

# Ejecutables: experimento_1.out, experimento_2.out, script_1.out, ...
TARGETS := $(addsuffix .out,$(EXPERIMENT_NAMES) $(SCRIPT_NAMES))

# Objetos correspondientes: build/experimento_1.o, build/script_1.o, ...
EXPERIMENT_OBJECTS := $(addprefix $(OBJ_DIR)/,$(addsuffix .o,$(EXPERIMENT_NAMES) $(SCRIPT_NAMES)))

# =========================
#  Phonies
# =========================
.PHONY: all clean run help \
        run1 run2 debug1 debug2 memoria1 memoria2

# =========================
#  Reglas principales
# =========================
# Compila todos los experimentos y scripts
all: $(TARGETS)

# Enlaza cada ejecutable a partir de su .o
# Regla genérica: target "X.out" depende de "build/X.o"
%.out: $(OBJ_DIR)/%.o | $(OBJ_DIR)
	@echo "Linkeando $@ ..."
	@$(CXX) $< -o $@ $(CXXFLAGS)

# Compila el .o de cada fuente: build/X.o desde src/X.cpp
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@echo "Compilando $< ..."
	@$(CXX) -c $< -o $@ $(CXXFLAGS)

# Crea el directorio build si no existe
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# =========================
#  Helpers de ejecución
# =========================
# Ejecuta todos (sin args)
run: $(TARGETS)
	@echo "Ejecutando todos los experimentos (sin argumentos) ..."
	@for exe in $(TARGETS); do echo "> $$exe"; ./$$exe || exit $$?; echo ""; done

# Atajos específicos (si existen esos archivos)
run1: experimento_1.out
	@./$<

run2: experimento_2.out
	@./$<

# Debug rápido con gdb (abre gdb con el ejecutable)
debug1: experimento_1.out
	@gdb ./$<

debug2: experimento_2.out
	@gdb ./$<

# Valgrind en específico (modifica flags a gusto)
memoria1: experimento_1.out
	@valgrind --leak-check=full --track-origins=yes ./$<

memoria2: experimento_2.out
	@valgrind --leak-check=full --track-origins=yes ./$<

# =========================
#  Limpieza
# =========================
clean:
	@rm -rf $(OBJ_DIR) *.out

# =========================
#  Ayuda
# =========================
help:
	@echo "Targets disponibles:"
	@echo "  all           -> compila todos los experimento_*.cpp y script_*.cpp en src/"
	@echo "  run           -> ejecuta todos los .out (sin argumentos)"
	@echo "  run1/run2     -> ejecuta experimento_1.out / experimento_2.out"
	@echo "  debug1/2      -> abre gdb con experimento_1.out / experimento_2.out"
	@echo "  memoria1/2    -> ejecuta con valgrind experimento_1.out / experimento_2.out"
	@echo "  clean         -> elimina build/ y *.out"
	@echo ""
	@echo "Sugerencia: pasa argumentos así -> make run1 ARGS=\"100000\""
