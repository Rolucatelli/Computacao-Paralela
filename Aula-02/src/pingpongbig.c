/* hello_mpi.c */
#include <mpi.h>
#include <stdio.h>

#define PING_PONG_LIMIT 1000

/* Send

*/

/* Ssend

*/

int main()
{
    int my_rank, comm_size;
    int count = 0;

    MPI_Init(NULL, NULL); // inicializa o ambiente MPI

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);   // id do processo
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size); // número total de processos

    if (my_rank == 0)
    {
        MPI_Send(&count, 1, MPI_INT, (my_rank + 1) % comm_size, 0, MPI_COMM_WORLD);
        printf("Starting Ping\n");
    }

    while (count < PING_PONG_LIMIT)
    {
        MPI_Recv(&count, 1, MPI_INT, (my_rank + comm_size - 1) % comm_size, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printf("Process %d recieved %d\n", my_rank, count);
        count++;
        if (count > PING_PONG_LIMIT)
        {
            MPI_Abort(MPI_COMM_WORLD, 0);
        }
        MPI_Send(&count, 1, MPI_INT, (my_rank + 1) % comm_size, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize(); // finaliza o ambiente MPI
    return 0;
}
