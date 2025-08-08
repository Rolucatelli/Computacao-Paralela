/* hello_mpi.c */
#include <mpi.h>
#include <stdio.h>
#include <string.h>

const int MAX_STRING = 100;

int main()
{
    char greeting[MAX_STRING];
    int my_rank, comm_size;

    MPI_Init(NULL, NULL); // inicializa o ambiente MPI

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);   // id do processo
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size); // número total de processos

    if (my_rank != 0)
    {
        /* create message*/
        sprintf(greeting, "Greetings from process %d of %d", my_rank, comm_size - 1);
        // Send message to process 0
        MPI_Send(greeting, strlen(greeting) + 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }
    else
    {
        printf("Greetings from process %d of %d\n", my_rank, comm_size - 1);
        for (int q = 1; q < comm_size; q++)
        {
            MPI_Recv(greeting, MAX_STRING, MPI_CHAR, q, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("%s\n", greeting);
        }
    }

    MPI_Finalize(); // finaliza o ambiente MPI
    return 0;
}
