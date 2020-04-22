#include<iostream>
#include<time.h>
#include<stdlib.h>
#include<mpi.h>

using namespace std;

int main(int argc, char **argv) {

    int rank, size, time_bomb, des;

    srand(time(NULL));

    //initializes MPI 
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    //sends out first bomb to a random process
    if (rank == 0) {
    
        time_bomb = rand() % 10 + 1;

        cout << "starting timer: " << time_bomb << endl;

        MPI_Send(&time_bomb, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    while (true) {
    
        //recieves bomb from another source
        MPI_Recv(&time_bomb, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        cout << " process: " << rank << " recieving timer at: " << time_bomb << endl;

        //if timer turns to 0 it breaks out of loop and declares looser
        if (time_bomb == 0) {cout << "losing process: " << rank << endl; break;}
            des = rand() % size;

            time_bomb--;

            MPI_Send(&time_bomb, 1, MPI_INT, des, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}