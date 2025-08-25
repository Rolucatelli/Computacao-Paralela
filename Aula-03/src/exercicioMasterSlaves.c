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
 * The amount of operations the system has to process
 *
 * Tags:
 *   0: Stop
 *   1: Avarage
 *   2: Filter
 *   3: Sort (Bubble Sort)
 *   4: Standard Deviation
 */
#define OPERATIONS 4

/**
 * The Max size that a vector can be
 *
 * The operation made is:
 * v_size = 50 + (rand() % MAX_VECTOR_SIZE) + 1;
 *
 * This is done so the vector has a guaranteed minimum size
 */
#define MAX_VECTOR_SIZE 500

// The biggest number that can be generated to fill the vectors
#define MAX_NUMBER 256

// The amount of tasks the sistem has to process
#define PROCESS_AMOUNT 100000

/// @brief Searchs VECTOR for free processes (0)
/// @param vector A vector containing the processes state
/// @param size Size of VECTOR
/// @return The number of the free process or -1 if none is free
int search_free_process(int *vector, int size)
{
    for (int i = 0; i < size; i++)
        if (vector[i] == 0)
            return i;
    return -1;
}

/// @brief Calculates the avarage of VECTOR
/// @param vector A vector of numbers
/// @param size Size of VECTOR
/// @return The value of the Avarage
float calc_avarage(int *vector, int size)
{
    int sum = 0;
    for (int i = 0; i < size; i++)
        sum += vector[i];
    return sum / size;
}

/// @brief Apply a smoothing filter on VECTOR
/// @param vector A vector of numbers
/// @param size Size of VECTOR
void filter_vect(int *vector, int size)
{
    for (int i = 1; i < size - 1; i++)
        vector[i] = (int)(vector[i - 1] + vector[i] + vector[i + 1]) / 3;
}

/// @brief Sorts the VECTOR using Bubble Sort
/// @param vector A vector of numbers
/// @param size Size of VECTOR
void bubble_sort(int *vector, int size)
{
    int i, j;
    for (i = 0; i < size - 1; i++)
        for (j = 0; j < size - i - 1; j++)
            if (vector[j] > vector[j + 1])
            {
                int temp = vector[j];
                vector[j] = vector[j + 1];
                vector[j + 1] = temp;
            }
}

/// @brief Calculates the standard deviation of VECTOR
/// @param vector The vector
/// @param size Size of VECTOR
/// @return The Standard Deviation
float standard_deviation(int *vector, int size)
{
    float sum = 0.0;
    float avg = calc_avarage(vector, size);
    for (int i = 0; i < size; i++)
    {
        sum += pow(vector[i] - avg, 2);
    }
    return sqrt(sum / size);
}

/// @brief Prints the first 3 and the last 3 numbers of VECTOR
/// @param vector A vector of numbers
/// @param size Size of VECTOR
void *print_vect(int *vector, int size)
{
    printf("%d, %d, %d, ..., %d, %d, %d\n",
           vector[0], vector[1], vector[2],
           vector[size - 3], vector[size - 2], vector[size - 1]);
}

int main()
{
    MPI_Init(NULL, NULL);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Status status;

    if (world_rank == 0)
    {
        srand(time(NULL));
        clock_t start, end;

        int operation, v_size;
        int *slaves = (int *)calloc(world_size, sizeof(int));
        int requests = PROCESS_AMOUNT;

        start = clock();
        while (requests > 0)
        {
            int slave_answer = 0;
            int slave = search_free_process(slaves, world_size);
            while (slave + 1)
            {
                operation = (rand() % OPERATIONS) + 1;
                v_size = 50 + (rand() % MAX_VECTOR_SIZE) + 1;
                int *vector = (int *)malloc(sizeof(int) * v_size);
                for (int i = 0; i < v_size; i++)
                    vector[i] = (rand() % MAX_NUMBER);

                slaves[slave] = 1;

                MPI_Send(vector, v_size, MPI_INT, slave, operation, MPI_COMM_WORLD);
                requests--;
                slave = search_free_process(slaves, world_size);
                free(vector);
            }
            MPI_Probe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
            MPI_Recv(&slave_answer, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("%d: has completed the task\n", status.MPI_SOURCE);
            slaves[status.MPI_SOURCE] = slave_answer;
        }
        for (int i = 0; i < world_size; i++)
            MPI_Send(&v_size, 1, MPI_INT, i, 0, MPI_COMM_WORLD);

        for (int i = 0; i < world_size; i++)
            MPI_Recv(&v_size, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        end = clock();
        sleep(0.2);
        printf("\n\n\nFinished Simulation\n\n\n");
        printf("Execution time: %f seconds\n\n", (double)(end - start) / CLOCKS_PER_SEC);
        free(slaves);
    }
    else
    {
        int tag = -1;
        do
        {
            MPI_Probe(0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            tag = status.MPI_TAG;

            int v_size;
            MPI_Get_count(&status, MPI_INT, &v_size);

            int *vector = (int *)malloc(sizeof(int) * v_size);

            MPI_Recv(vector, v_size, MPI_INT, 0, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            if (tag)
                printf("%d: received task %d\n", world_rank, tag);
            else
                printf("\n%d: finished working\n", world_rank);

            switch (tag)
            {
            case 1:
                float avg = calc_avarage(vector, v_size);
                printf("%d: calculed the avarage %.2f\n", world_rank, avg);
                break;
            case 2:
                filter_vect(vector, v_size);
                printf("%d: filtered the vector: ", world_rank);
                print_vect(vector, v_size);

                break;
            case 3:
                bubble_sort(vector, v_size);
                printf("%d: ordered the vector: ", world_rank);
                print_vect(vector, v_size);

                break;
            case 4:
                float sd = standard_deviation(vector, v_size);
                printf("%d: calculed the standard deviation %.2f\n", world_rank, sd);
                break;
            default:
                break;
            }

            // Declaring just to send something
            int msg = 0;
            MPI_Send(&msg, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

            free(vector);
        } while (tag);
    }

    MPI_Finalize();
    return 0;
}
