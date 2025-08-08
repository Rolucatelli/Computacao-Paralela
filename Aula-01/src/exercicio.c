/* hello_mpi.c */
#include <mpi.h>
#include <stdio.h>
#include <string.h>

const int MAX_STRING = 100;

int main()
{
    char greeting[MAX_STRING], input[MAX_STRING];
    int my_rank, comm_size;

    MPI_Init(NULL, NULL); // inicializa o ambiente MPI

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);   // id do processo
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size); // número total de processos

    /* create message*/
    sprintf(greeting, "Greetings from process %d", my_rank);
    MPI_Send(greeting, strlen(greeting) + 1, MPI_CHAR, (my_rank + 1) % comm_size, 0, MPI_COMM_WORLD);

    MPI_Recv(input, MAX_STRING, MPI_CHAR, (my_rank + comm_size - 1) % comm_size, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("Process %d: %s\n", my_rank, input);

    MPI_Finalize(); // finaliza o ambiente MPI
    return 0;
}
