/* hello_mpi.c */
#include <mpi.h>
#include <stdio.h>

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
            MPI_Send(&count, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            printf("Starting Ping\n");
        }

        while (count < 100)
        {
            MPI_Recv(&count, 1, MPI_INT, (my_rank + 2 - 1) % 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Process %d: %d\n", my_rank, count);
            count++;
            MPI_Send(&count, 1, MPI_INT, (my_rank + 1) % 2, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize(); // finaliza o ambiente MPI
    return 0;
}
