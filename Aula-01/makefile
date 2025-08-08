# Nome do projeto
# PROJ_NAME=hello_world
# PROJ_NAME=hello_2
# PROJ_NAME=exercicio
PROJ_NAME=pingpong


# Compilador
CC=mpicc

# Número de Processos
NP=4

# Arquivo hostfile
HFILE=hostfile.txt

# Comando utilizado como target do clean
RM = rm -f

#########################
# Compilação e linkagem #
#########################
all: exeFolder $(PROJ_NAME)

run: all
	@ mpirun -np $(NP) --hostfile $(HFILE) ./exe/$(PROJ_NAME)


$(PROJ_NAME):
	@ $(CC) -o ./exe/$(PROJ_NAME) ./src/$(PROJ_NAME).c

exeFolder:
	@ mkdir -p exe

clean:
	@ $(RM) ./exe/* *~


.PHONY: all clean run exeFolder
# Se existir algum arquivo com o mesmo nome que uma tag, como clean.c ou all.c, ele ignora