/* hello_mpi.c */
#include <mpi.h>
#include <stdio.h>

#define PING_PONG_LIMIT 1000000

/* Send
real    0m6,147s
user    0m7,278s
sys     0m2,790s
*/

/* Ssend
real    0m6,247s
user    0m7,573s
sys     0m2,827s
*/

int main()
{
    int my_rank, comm_size;
    int count = 0;

    MPI_Init(NULL, NULL); // inicializa o ambiente MPI

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);   // id do processo
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size); // número total de processos

    if (my_rank <= 1)
    {
        if (my_rank == 0)
        {
            MPI_Ssend(&count, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            printf("Starting Ping\n");
        }

        while (count < PING_PONG_LIMIT)
        {
            MPI_Recv(&count, 1, MPI_INT, (my_rank + 2 - 1) % 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Process %d: %d\n", my_rank, count);
            count++;
            MPI_Ssend(&count, 1, MPI_INT, (my_rank + 1) % 2, 0, MPI_COMM_WORLD);
        }
        if (my_rank == 0)
        {
            MPI_Recv(&count, 1, MPI_INT, (my_rank + 2 - 1) % 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            printf("Ending Ping\n");
        }
    }

    MPI_Finalize(); // finaliza o ambiente MPI
    return 0;
}
