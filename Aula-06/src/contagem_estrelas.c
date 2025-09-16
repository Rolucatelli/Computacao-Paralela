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
    printf("%d %d %d %d %d ... %d %d %d %d %d\n",
           vector[0], vector[1], vector[2], vector[3], vector[4],
           vector[size - 5], vector[size - 4], vector[size - 3], vector[size - 2], vector[size - 1]);
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

void paint_star(int *photo, int rows, int columns, int i, int j)
{

    // printf("Painting Star at i:%d j:%d\n", i, j);
    photo[i * columns + j] = -1;
    if (i + 1 < rows && photo[(i + 1) * columns + j] > STAR_TRESHOLD)
    {
        paint_star(photo, rows, columns, i + 1, j);
    }

    if (j + 1 < columns && photo[(i)*columns + j + 1] > STAR_TRESHOLD)
    {
        paint_star(photo, rows, columns, i, j + 1);
    }

    if (i > 0 && photo[(i - 1) * columns + j] > STAR_TRESHOLD)
    {
        paint_star(photo, rows, columns, i - 1, j);
    }

    if (j > 0 && photo[(i)*columns + j - 1] > STAR_TRESHOLD)
    {
        paint_star(photo, rows, columns, i, j - 1);
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
    int count = 0;

    if (world_rank == ROOT_PROCESS)
    {
        char file_path[256], img_type[4];
        // printf("Digite o nome da foto: ");
        // scanf("%s", file_path);
        // FILE *file = fopen(file_path, "r");

        /*
            Estrelas1: 27
            Estrelas2: 81
            Estrelas3: 113
        */
        FILE *file = fopen("./img/estrelas2.pgm", "r");

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
        //         if (photo[i * rows + j] > STAR_TRESHOLD)
        //         {

        //             printf("%d: Star Found at %d (i:%d j:%d)!\n", world_rank, (i * columns + j), i, j);
        //             paint_star(photo, rows, columns, i, j);
        //             star_count++;
        //         }
        //     }
        // }
        // printf("Star Count: %d\n", star_count);
        // DEBUG_STOP
        /*----------------------------------------------*/
        /*----------------------------------------------*/
        // Enviando a foto para os outros processos
        int size_per_process = (int)(rows / world_size);
        size_per_process += (int)(size_per_process / (world_size - 1));
        int remain = size_per_process % world_size;
        for (int i = 1; i < world_size; i++)
        {
            // printf("i:%d, size_per_process:%d, remain:%d\n", i, size_per_process, remain);

            int start_vect = (size_per_process + (remain >= 0 ? 1 : 0)) * rows;
            int size_vect = (size_per_process + (remain > 0) + (i < world_size - 1)) * columns;
            MPI_Send(&(photo[(i - 1) * start_vect]), size_vect, MPI_INT, i, 1, MPI_COMM_WORLD);
            MPI_Send(&columns, 1, MPI_INT, i, 2, MPI_COMM_WORLD);
            MPI_Send(&rows, 1, MPI_INT, i, 3, MPI_COMM_WORLD);
            remain--;
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

        photo = (int *)calloc(sizeof(int), v_size);
        MPI_Recv(photo, v_size, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&columns, 1, MPI_INT, 0, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&rows, 1, MPI_INT, 0, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // printf("%d: Row:%d, Col:%d, V_size:%d\n", world_rank, (int)ceil(v_size / columns), columns, v_size);
        /*----------------------------------------------*/
        /*----------------------------------------------*/
        // Contagem de estrelas
        int row = (int)ceil(v_size / columns);
        // preview_vect(photo, v_size);
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < columns; j++)
            {
                if ((photo[i * columns + j]) > STAR_TRESHOLD)
                {
                    paint_star(photo, row, columns, i, j);
                    if (world_rank == ROOT_PROCESS + 1 || i > 0)
                    {
                        printf("%d: Star Found at %d (i:%d j:%d)!\n", world_rank, (i * columns + j) * world_rank, i, j);
                        count++;
                    }
                }
            }
        }

        // printf("%d: Count: %d\n", world_rank, count);

        /*----------------------------------------------*/
    }
    int total_count = 0;
    MPI_Reduce(&count, &total_count, 1, MPI_INT, MPI_SUM, ROOT_PROCESS, MPI_COMM_WORLD);
    if (world_rank == ROOT_PROCESS)
    {
        printf("Final Star Count: %d\n", total_count);
    }

    /*----------------------------------------------*/

    free(photo);
    MPI_Finalize();
    return 0;
}
