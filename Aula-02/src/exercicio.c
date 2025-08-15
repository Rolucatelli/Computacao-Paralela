/* hello_mpi.c */
#include <mpi.h>
#include <stdio.h>
#include <string.h>

const int MAX_STRING = 100;

/* Ssend
real    0m8,745s
user    0m43,060s
sys     0m22,659s
*/

/* Send
real    0m8,349s
user    0m41,283s
sys     0m21,757s
*/

int main()
{
    char greeting[MAX_STRING], input[MAX_STRING];
    int my_rank, comm_size;

    MPI_Init(NULL, NULL); // inicializa o ambiente MPI

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);   // id do processo
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size); // número total de processos

    /* create message*/
    sprintf(greeting, "Greetings (Ssend) from process %d", my_rank);
    if (my_rank == 0)
    {
        MPI_Ssend(greeting, strlen(greeting) + 1, MPI_CHAR, (my_rank + 1) % comm_size, 0, MPI_COMM_WORLD);
        MPI_Recv(input, MAX_STRING, MPI_CHAR, (my_rank + comm_size - 1) % comm_size, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    else
    {
        MPI_Recv(input, MAX_STRING, MPI_CHAR, (my_rank + comm_size - 1) % comm_size, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Ssend(greeting, strlen(greeting) + 1, MPI_CHAR, (my_rank + 1) % comm_size, 0, MPI_COMM_WORLD);
    }

    printf("Process %d: %s\n", my_rank, input);

    MPI_Finalize(); // finaliza o ambiente MPI
    return 0;
}
