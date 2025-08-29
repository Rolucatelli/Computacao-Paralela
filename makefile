# Nome do projeto
PROJ_NAME=compare_bcast

# Número de Processos
NP=12

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

create: exeFolder
	@ touch ./src/$(PROJ_NAME).c

$(PROJ_NAME): $(OBJ)
	@ $(CC) -o ./exe/$(PROJ_NAME) ./src/$(PROJ_NAME).c -lm

exeFolder: objectsFolder
	@ mkdir -p exe 

objectsFolder: srcFolder hdrFolder
	@ mkdir -p objects

srcFolder:
	@ mkdir -p src

hdrFolder:
	@ mkdir -p hdr

clean:
	@ $(RM) ./objects/* ./exe/* *~

.PHONY: all clean debug run echo
# Se existir algum arquivo com o mesmo nome que uma tag, como clean.c ou all.c, ele ignora