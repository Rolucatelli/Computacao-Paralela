# Nome do projeto
# PROJ_NAME=checkStatus
# PROJ_NAME=probe
PROJ_NAME=exercicioReduce

# Número de Processos
NP=4

# Compilador
CC=mpicc

# Arquivo hostfile
HFILE=hostfile.txt

# Comando utilizado como target do clean
RM = rm -f

#########################
# Compilação e linkagem #
#########################
all: exeFolder $(PROJ_NAME)

run: all
	@ mpirun --oversubscribe -np $(NP) --hostfile $(HFILE) ./exe/$(PROJ_NAME)


$(PROJ_NAME):
	@ $(CC) -o ./exe/$(PROJ_NAME) ./src/$(PROJ_NAME).c -lm

exeFolder:
	@ mkdir -p exe

clean:
	@ $(RM) ./exe/* *~


.PHONY: all clean run exeFolder
# Se existir algum arquivo com o mesmo nome que uma tag, como clean.c ou all.c, ele ignora