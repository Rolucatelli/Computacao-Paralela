/*=============================================================
 *             UNIFAL - Universidade Federal de Alfenas.
 *               BACHARELADO EM CIENCIA DA COMPUTACAO.
 * Trabalho..: Multiplicação de Matrizes
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

#define VECTOR_SIZE 1024

// The biggest number that can be generated to fill the matrixes
#define MAX_NUMBER 256

#define WORLD_SIZE 8

#define MATRIX_SIZE VECTOR_SIZE *VECTOR_SIZE
// 1024 * 128
#define SIZE_PER_PROCESS ((int)(MATRIX_SIZE) / WORLD_SIZE)

#define ROOT_PROCESS 0

#define DEBUG_STOP  \
    MPI_Finalize(); \
    return 0;

void print_matrix(int *matrix, int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            printf("%4d ", matrix[i * size + j]);
        }
        printf("\n");
    }
}
void fprint_matrix(FILE *file, int *matrix, int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            fprintf(file, "%4d ", matrix[i * size + j]);
        }
        fprintf(file, "\n");
    }
}

int main()
{
    MPI_Init(NULL, NULL);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    if (world_size != WORLD_SIZE)
    {
        if (world_rank == ROOT_PROCESS)
        {
            printf("The number of process must be %d\n", WORLD_SIZE);
        }
        MPI_Finalize();
        return 1;
    }

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
        int *temp_matrix = (int *)calloc(sizeof(int), MATRIX_SIZE);

        for (int i = 0; i < VECTOR_SIZE; i++)
        {
            for (int j = 0; j < VECTOR_SIZE; j++)
            {
                temp_matrix[i * VECTOR_SIZE + j] = (rand() % MAX_NUMBER);
            }
        }
        /*----------------------------------------------*/
        // Salvando matrizes A e B em arquivo
        FILE *mat_A = fopen("./matrix_A.txt", "w");
        fprint_matrix(mat_A, temp_matrix, VECTOR_SIZE);
        fclose(mat_A);

        FILE *mat_B = fopen("./matrix_B.txt", "w");
        fprint_matrix(mat_B, matrix_B, VECTOR_SIZE);
        fclose(mat_B);

        /*----------------------------------------------*/

        /*----------------------------------------------*/
        // Calculando Matriz A Transposta
        for (int i = 0; i < VECTOR_SIZE; i++)
        {
            for (int j = 0; j < VECTOR_SIZE; j++)
            {
                matrix_A[i * VECTOR_SIZE + j] = temp_matrix[j * VECTOR_SIZE + i];
            }
        }
        free(temp_matrix);
        /*----------------------------------------------*/
    }

    /*----------------------------------------------*/
    // Passando os vetores para os processos
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Scatter(matrix_A, SIZE_PER_PROCESS, MPI_INT, vector_A, SIZE_PER_PROCESS, MPI_INT, ROOT_PROCESS, MPI_COMM_WORLD);
    MPI_Scatter(matrix_B, SIZE_PER_PROCESS, MPI_INT, vector_B, SIZE_PER_PROCESS, MPI_INT, ROOT_PROCESS, MPI_COMM_WORLD);
    /*----------------------------------------------*/

    /*----------------------------------------------*/
    // Calculando Matrizes D
    int *matrix_D = (int *)calloc(sizeof(int), MATRIX_SIZE);
    int line_count = (int)VECTOR_SIZE / world_size;
    for (int i = 0; i < line_count; i++)
    {
        for (int j = 0; j < VECTOR_SIZE; j++)
        {
            for (int k = 0; k < VECTOR_SIZE; k++)
            {
                matrix_D[j * VECTOR_SIZE + k] += vector_A[i * VECTOR_SIZE + j] * vector_B[i * VECTOR_SIZE + k];
            }
        }
    }

    /*----------------------------------------------*/
    int *matrix_C = (int *)calloc(sizeof(int), MATRIX_SIZE);

    MPI_Reduce(matrix_D, matrix_C, MATRIX_SIZE, MPI_INT, MPI_SUM, ROOT_PROCESS, MPI_COMM_WORLD);
    if (world_rank == ROOT_PROCESS)
    {
        free(matrix_A);
        free(matrix_B);
        FILE *mat_C = fopen("./matrix_C.txt", "w");
        fprint_matrix(mat_C, matrix_C, VECTOR_SIZE);
        fclose(mat_C);
        print_matrix(matrix_C, VECTOR_SIZE);
    }

    free(matrix_C);
    free(matrix_D);
    free(vector_A);
    free(vector_B);
    MPI_Finalize();
    return 0;
}
