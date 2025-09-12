/*=============================================================
 *             UNIFAL - Universidade Federal de Alfenas.
 *               BACHARELADO EM CIENCIA DA COMPUTACAO.
 * Trabalho..: 02 Contagem Estrelas
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

int is_star(int pixel)
{
    return pixel > 100;
}

void paint_star(int *photo, int size, int x, int y)
{

    photo[x * size + y] = -1;
    if (is_star(photo[(x + 1) * size + y]))
    {
        paint_star(photo, size, x + 1, y);
    }
    if (is_star(photo[(x)*size + y + 1]))
    {
        paint_star(photo, size, x, y + 1);
    }
    if (x > 0 && is_star(photo[(x - 1) * size + y]))
    {
        paint_star(photo, size, x - 1, y);
    }
    if (y > 0 && is_star(photo[(x)*size + y - 1]))
    {
        paint_star(photo, size, x, y - 1);
    }
}

int main()
{
    MPI_Init(NULL, NULL);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int *photo;

    if (world_rank == ROOT_PROCESS)
    {
        char file_path[256], img_type[4];
        int rows, columns, grey_level;
        // printf("Digite o nome da foto: ");
        // scanf("%s", file_path);
        // FILE *file = fopen(file_path, "r");
        FILE *file = fopen("./img/estrelas3.pgm", "r");

        fscanf(file, "%s", img_type);
        fscanf(file, "%d %d", &rows, &columns);
        fscanf(file, "%d", &grey_level);

        photo = (int *)malloc(sizeof(int) * rows * columns);

        /*----------------------------------------------*/
        // Lendo a foto
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < columns; j++)
            {
                fscanf(file, "%d", &(photo[i * rows + j]));
            }
        }

        // print_matrix(photo, rows);
        /*----------------------------------------------*/
        /*----------------------------------------------*/
        // Contando estrelas
        int star_count = 0;
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < columns; j++)
            {
                if (is_star(photo[i * rows + j]))
                {
                    paint_star(photo, rows, i, j);
                    star_count++;
                }
            }
        }
        printf("Star Count: %d\n", star_count);
        /*----------------------------------------------*/
    }
    MPI_Barrier(MPI_COMM_WORLD);
    /*----------------------------------------------*/

    MPI_Finalize();
    return 0;
}
