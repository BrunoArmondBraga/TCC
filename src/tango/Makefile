# Nome do compilador
CXX = g++

# Flags de compilação
CXXFLAGS =

# Nome do arquivo fonte e do executável
SRC = Tango.cpp
OUT = tan

# Regra padrão
all: $(OUT)

# Como compilar
$(OUT): $(SRC)
	@$(CXX) $(CXXFLAGS) -o $(OUT) $(SRC)

# Limpar arquivos gerados
clean:
	@rm -f $(OUT)

# Compilar do zero e executar
run:
	@$(MAKE) --no-print-directory clean
	@$(MAKE) --no-print-directory all
	@./$(OUT)
