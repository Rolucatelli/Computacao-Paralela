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
 * v_size = 50 + (rand() % VECTOR_SIZE) + 1;
 *
 * This is done so the vector has a guaranteed minimum size
 */
#define VECTOR_SIZE 1024
#define MATRIX_SIZE 1024 * 1024
#define SIZE_PER_PROCESS (int)(MATRIX_SIZE) / world_size

// The biggest number that can be generated to fill the vectors
#define MAX_NUMBER 256

// The amount of tasks the sistem has to process
#define PROCESS_AMOUNT 100000

#define ROOT_PROCESS 0

#define DEBUG_STOP  \
    MPI_Finalize(); \
    return 0;

void preview_vect(int *vector, int size)
{
    printf("%d, %d, %d, ..., %d, %d, %d\n",
           vector[0], vector[1], vector[2],
           vector[size - 3], vector[size - 2], vector[size - 1]);
}

void print_vect(int *vector, int size)
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
    int *vector_A = (int *)calloc(sizeof(int), (SIZE_PER_PROCESS));
    int *vector_B = (int *)calloc(sizeof(int), (SIZE_PER_PROCESS));
    int *matrix_B, *matrix_A;

    if (world_rank == ROOT_PROCESS)
    {
        srand(time(NULL));

        matrix_B = (int *)calloc(sizeof(int), MATRIX_SIZE);

        for (int i = 0; i < VECTOR_SIZE; i++)
        {
            for (int j = 0; j < VECTOR_SIZE; j++)
            {
                matrix_B[i * VECTOR_SIZE + j] = (rand() % MAX_NUMBER);
            }
        }

        matrix_A = (int *)calloc(sizeof(int), MATRIX_SIZE);

        for (int i = 0; i < VECTOR_SIZE; i++)
        {
            for (int j = 0; j < VECTOR_SIZE; j++)
            {
                matrix_A[i * VECTOR_SIZE + j] = (rand() % MAX_NUMBER);
            }
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Scatter(matrix_A, SIZE_PER_PROCESS, MPI_INT, vector_A, SIZE_PER_PROCESS, MPI_INT, ROOT_PROCESS, MPI_COMM_WORLD);
    MPI_Scatter(matrix_B, SIZE_PER_PROCESS, MPI_INT, vector_B, SIZE_PER_PROCESS, MPI_INT, ROOT_PROCESS, MPI_COMM_WORLD);

    for (int i = 0; i < SIZE_PER_PROCESS; i++)
    {
        vector_B[i] = vector_B[i] + vector_A[i];
    }

    MPI_Gather(vector_B, SIZE_PER_PROCESS, MPI_INT, matrix_B, SIZE_PER_PROCESS, MPI_INT, ROOT_PROCESS, MPI_COMM_WORLD);
    preview_vect(vector_B, SIZE_PER_PROCESS);
    if (world_rank == ROOT_PROCESS)
    {
        getchar();
        getchar();
        print_vect(matrix_B, MATRIX_SIZE);
    }

    MPI_Finalize();
    return 0;
}
