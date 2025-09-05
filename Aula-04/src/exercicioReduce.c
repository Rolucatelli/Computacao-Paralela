/*=============================================================
 *             UNIFAL - Universidade Federal de Alfenas.
 *               BACHARELADO EM CIENCIA DA COMPUTACAO.
 * Trabalho..: 02 Mestre e Escravos
 * Professor.: Paulo Alexandre Bressan
 * Aluno.....: Rodrigo Luís Gasparino Lucatelli
 * Data......: 27/08/2025
 *=============================================================*/

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>

/**
 * The Max size that a vector can be
 *
 * The operation made is:
 * v_size = 50 + (rand() % MAX_VECTOR_SIZE) + 1;
 *
 * This is done so the vector has a guaranteed minimum size
 */
#define MAX_VECTOR_SIZE 1024

// The biggest number that can be generated to fill the vectors
#define MAX_NUMBER 256

// The amount of tasks the sistem has to process
#define PROCESS_AMOUNT 100000

#define ROOT_PROCESS 0

#define DEBUG_STOP  \
    MPI_Finalize(); \
    return 0;

/// @brief Prints the first 3 and the last 3 numbers of VECTOR
/// @param vector A vector of numbers
/// @param size Size of VECTOR
void *print_vect(int *vector, int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("%d, ", vector[i]);
    }
    printf("\n");
}

int main()
{
    MPI_Init(NULL, NULL);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Status status;

    int first_size = MAX_VECTOR_SIZE, *first_vect, recv_size, *recv_vect;

    if (world_rank == ROOT_PROCESS)
    {
        srand(time(NULL));
        first_vect = (int *)calloc(sizeof(int), first_size);
        for (int i = 0; i < first_size; i++)
            first_vect[i] = (rand() % MAX_NUMBER);
        printf("%d: first_vector: ", world_rank);
        print_vect(first_vect, first_size);
    }
    recv_size = (int)ceil(first_size / world_size);
    recv_vect = (int *)calloc(sizeof(int), recv_size);
    MPI_Scatter(first_vect, recv_size, MPI_INT, recv_vect, recv_size, MPI_INT, ROOT_PROCESS, MPI_COMM_WORLD);

    int *send_vect = (int *)calloc(sizeof(int), recv_size);
    MPI_Reduce(recv_vect, send_vect, recv_size, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    while (recv_size > 1)
    {
        // Realocando recv_vect
        recv_size = (int)ceil(recv_size / world_size);
        free(recv_vect);
        recv_vect = (int *)calloc(sizeof(int), recv_size);
        MPI_Scatter(first_vect, recv_size, MPI_INT, recv_vect, recv_size, MPI_INT, ROOT_PROCESS, MPI_COMM_WORLD);

        free(send_vect);
        send_vect = (int *)calloc(sizeof(int), recv_size);
        MPI_Reduce(recv_vect, send_vect, recv_size, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
    }

    if (world_rank == ROOT_PROCESS)
    {
        printf("Max Value: %d\n", send_vect[0]);
    }

    MPI_Finalize();
    return 0;
}
