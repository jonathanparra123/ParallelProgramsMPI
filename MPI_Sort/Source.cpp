#include<iostream>
#include<time.h>
#include<stdlib.h>
#include<mpi.h>
#include<algorithm>

using namespace std;

// function that prints an array
void PrintArray(int size, int array[]) {

    for (int i = 0; i < size; i++) {
        cout << array[i] << endl;
    }
};

int main(int argc, char **argv) {

    int rank, size;
    int const size_data = 12;
    int data_array[size_data];

    srand(time(NULL));

    //gives the array a list of random numbers
    for (int i = 0; i < size_data; i++) {
        data_array[i] = rand() % 200;
    }

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int size_sub = size_data / (size - 1);
    int const size_sub2 = size_data / (size - 1);

    //master process that sends out sub arrays of main array to each process
    if (rank == 0) {
    
        cout << "unsorted array: " << endl;

        PrintArray(size_data, data_array);
    
        for (int i = 1; i < size; i++) {
        
            size_sub = size_sub*i;

            MPI_Send(data_array, size_data, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&size_sub, 1, MPI_INT, i, 0, MPI_COMM_WORLD);

            size_sub = size_data / (size - 1);

            MPI_Recv(data_array, size_data, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        cout << "Sorted array: " << endl;

        //after each individual sub array is sorted we can do a merge sort on all sub arrays to sort main array
        sort(data_array, data_array + size_data);
        PrintArray(size_data, data_array);
    }
    else
    {
        MPI_Recv(data_array, size_data, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&size_sub, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        int temp_array[size_sub2];
        int count = 0;
        
        //receives and sorts each sub array
        for (int j = size_sub - size_sub2; j < size_sub; j++) {
        
            temp_array[count] = data_array[j];

            count++;
        }

        sort(temp_array, temp_array + size_sub);

        count = 0;

        for (int k = size_sub - size_sub2; k < size_sub; k++) {
        
            data_array[k] = temp_array[count];

            count++;
        }

        //sends sub array to main process
        MPI_Send(data_array, size_data, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}