#include <mpi.h>
#include <stdio.h>

#define PING_PONG_LIMIT 1000000

int main()
{
    int my_rank, comm_size, next, prev;
    int count = 0;

    MPI_Init(NULL, NULL);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);

    next = (my_rank + 1) % comm_size;
    prev = (my_rank + comm_size - 1) % comm_size;

    if (my_rank == 0)
    {
        MPI_Send(&count, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
    }

    do
    {
        MPI_Recv(&count, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printf("Process %d recieved %d from %d\n", my_rank, count, prev);
        count++;
        MPI_Send(&count, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
    } while (count < PING_PONG_LIMIT - comm_size + 2);

    MPI_Finalize();
    return 0;
}
