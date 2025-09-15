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

#define STAR_TRESHOLD 100

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

void paint_star(int *photo, int size, int x, int y)
{

    photo[x * size + y] = -1;
    if (photo[(x + 1) * size + y] > STAR_TRESHOLD)
    {
        paint_star(photo, size, x + 1, y);
    }
    if (photo[(x)*size + y + 1] > STAR_TRESHOLD)
    {
        paint_star(photo, size, x, y + 1);
    }
    if (x > 0 && photo[(x - 1) * size + y] > STAR_TRESHOLD)
    {
        paint_star(photo, size, x - 1, y);
    }
    if (y > 0 && photo[(x)*size + y - 1] > STAR_TRESHOLD)
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
    int rows, columns, grey_level;

    if (world_rank == ROOT_PROCESS)
    {
        char file_path[256], img_type[4];
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
        // int star_count = 0;
        // for (int i = 0; i < rows; i++)
        // {
        //     for (int j = 0; j < columns; j++)
        //     {
        //         if (is_star(photo[i * rows + j]))
        //         {
        //             paint_star(photo, rows, i, j);
        //             star_count++;
        //         }
        //     }
        // }
        // printf("Star Count: %d\n", star_count);
        /*----------------------------------------------*/
        /*----------------------------------------------*/
        // Enviando a foto para os outros processos
        const int size_per_process = (int)(columns * rows / (world_size - 1)) + 1;
        for (int i = 1; i < world_size; i++)
        {
            MPI_Send(&(photo[(i - 1) * size_per_process]), size_per_process + (i < world_size - 1 ? columns : 0), MPI_INT, i, 1, MPI_COMM_WORLD);
            MPI_Send(&columns, 1, MPI_INT, i, 2, MPI_COMM_WORLD);
            MPI_Send(&rows, 1, MPI_INT, i, 3, MPI_COMM_WORLD);
        }

        /*----------------------------------------------*/
    }
    else
    {
        /*----------------------------------------------*/
        // Recebendo foto
        MPI_Status status;
        MPI_Probe(0, 1, MPI_COMM_WORLD, &status);

        int v_size;
        MPI_Get_count(&status, MPI_INT, &v_size);

        photo = (int *)malloc(sizeof(int) * v_size);
        MPI_Recv(photo, v_size, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&columns, 1, MPI_INT, 0, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&rows, 1, MPI_INT, 0, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        /*----------------------------------------------*/
        /*----------------------------------------------*/
        // Contagem de estrelas
        int count = 0;
        for (int i = 0; i < v_size; i++)
        {
            if (photo[i] > STAR_TRESHOLD)
            {
                paint_star(photo, v_size, i, i % columns);
                if (world_rank != ROOT_PROCESS + 1 && i / columns < 0)
                {
                    /* code */
                }
                else
                {
                    count++;
                }
            }
        }

        /*----------------------------------------------*/
    }

    /*----------------------------------------------*/

    free(photo);
    MPI_Finalize();
    return 0;
}
