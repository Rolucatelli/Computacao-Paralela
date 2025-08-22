#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define OPERATIONS 4
#define MAX_VECTOR_SIZE 100
#define MAX_NUMBER 255

/*
  Tags:
    0: Halt
    1: Média
    2: Filtro (Média dos em volta)
    3: Ordenar (Bubble Sort)
    4: Desvio Padrão
    5:
*/

/**
 * @return The number of the free slave or 0 if none is free
 */
int search_free_slave(int *vector, int size)
{
    for (int i = 0; i < size; i++)
        if (!vector[i])
            return i + 1;
    return 0;
}

float calc_avarage(int *vector, int size)
{
    int sum = 0;
    for (int i = 0; i < size; i++)
        sum += vector[i];
    return sum / size;
}

void filter_vect(int *vector, int size)
{
    for (int i = 1; i < size - 1; i++)
    {
        vector[i] = (int)vector[i - 1] + vector[i] + vector[i + 1] / 3;
    }
}

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

int main(int argc, char **argv)
{
    MPI_Init(NULL, NULL);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    if (world_rank == 0)
    {
        srand(time(NULL));
        int operation, v_size;
        int *slaves = (int *)calloc(world_size, sizeof(int));
        int requests = world_size * 200;
        MPI_Status status;
        while (requests > 0)
        {
            int slave_answer = 0;
            int slave = search_free_slave(slaves, world_size);
            puts("1");
            while (slave)
            {
                puts("2");

                operation = (rand() % OPERATIONS) + 1;
                v_size = (rand() % MAX_VECTOR_SIZE) + 1;
                printf("%d %d\n", operation, v_size);
                int *vector = (int *)malloc(sizeof(int) * v_size);
                for (int i = 0; i < v_size; i++)
                {
                    vector[i] = (rand() % MAX_NUMBER) + 1;
                }

                slaves[slave - 1] = 1;

                MPI_Send(vector, v_size, MPI_INT, slave - 1, operation, MPI_COMM_WORLD);
                printf("Master sent a request with %d numbers to Slave %d\n", v_size, slave);
                requests--;
                free(vector);
            }
            MPI_Probe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
            MPI_Recv(&slave_answer, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            slaves[status.MPI_SOURCE] = slave_answer;
        }
    }
    else
    {
        // Slaves devem mandar 0 para Master se estiverem livres

        MPI_Status status;
        int tag = -1;
        do
        {
            MPI_Probe(0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            tag = status.MPI_TAG;

            int v_size = 0;

            MPI_Get_count(&status, MPI_INT, &v_size);

            int *vector = (int *)malloc(sizeof(int) * v_size);

            MPI_Recv(vector, v_size, MPI_INT, 0, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            switch (tag)
            {
            case 0:
                // Stop
                break;
            case 1:
                printf("Calculing avarage\n");
                float avg = calc_avarage(vector, v_size);
                printf("Process %d calculed the avarage %.2f\n", world_rank, avg);
                break;
            case 2:
                printf("Filtering vector\n");

                filter_vect(vector, v_size);
                printf("Process %d filtered the vector \n", world_rank);
                break;
            case 3:
                printf("Ordering vector\n");

                bubble_sort(vector, v_size);
                printf("Process %d ordered the vector \n", world_rank);
                break;
            case 4:
                printf("Calculing sd\n");

                float sd = standard_deviation(vector, v_size);
                printf("Process %d calculed the standard deviation %.2f\n", world_rank, sd);
                break;
            }

            MPI_Send(0, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

            free(vector);
        } while (tag);
    }
    MPI_Finalize();
}