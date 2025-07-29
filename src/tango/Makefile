# Nome do compilador
CXX = g++

# Flags de compilação
CXXFLAGS =

# Fontes
SRC = Tango.cpp Auxiliary.cpp
OBJ = $(SRC:.cpp=.o)

# Nome do executável
OUT = tan

# Regra padrão
all: $(OUT)

# Como compilar o executável
$(OUT): $(OBJ)
	@$(CXX) $(CXXFLAGS) -o $@ $^

# Regra para compilar cada .cpp em .o
%.o: %.cpp
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpar arquivos gerados
clean:
	@rm -f $(OUT) *.o

# Compilar do zero e executar
run:
	@$(MAKE) --no-print-directory clean
	@$(MAKE) --no-print-directory all
	@./$(OUT)

# Compilar do zero e executar com terminal limpo
clear:
	clear
	@$(MAKE) --no-print-directory run
