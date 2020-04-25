#include<iostream>
#include<stdlib.h>
#include<mpi.h>
#include<unistd.h>
#include<fstream>
#include<time.h>

using namespace std;

int main(int argc, char **argv) {

    //variables for the size of the grid
    int const grid = 1024;
    int num_days = 100;
    int buffer = 0;
    int num_days_start = 0;
    int rank, size, rows;
    int world[grid][grid];

    double time1, time2;

    srand(time(NULL));

    //populates the grid with live or dead cells
    for (int i = 0; i < grid; i++) {

        for (int j = 0; j < grid; j++) {

            int random = rand() % 5;

            if (random == 1) {

                world[i][j] = 1;
            }
            else {

                world[i][j] = 0;
            }
        }
    }

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    time1 = MPI_Wtime();

    int grid_part = grid / (size - 1);
    int update[grid_part][grid];

    //interates number of days
    while (num_days_start < num_days) {

        num_days_start++;

        //main process sends sections of grid to different processes
        if (rank == 0) {
    
	   //display current day	
      //cout << "day: " << num_days_start << endl;

            for (int i = 0; i < size; i++) {

                rows = (i * grid_part);

                MPI_Send(&rows, 1, MPI_INT, i, 0, MPI_COMM_WORLD);

		//orginizes proccess
       //MPI_Recv(&buffer, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        }
        else {

            //recieves row 
            MPI_Recv(&rows, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            int count = 1;
            int fate = 0;

            //checks neighboring cells of current cells
            for (int i = (rows - grid_part); i < rows; i++) {

                for (int j = 0; j < grid; j++) {

                    if (i - 1 < (rows - grid_part)) {

                        if (j - 1 < 0) {

                            fate = (world[i + 1][j] + world[i][j + 1] + world[i + 1][j + 1]);
                        }
                        else {

                            fate = (world[i][j - 1] + world[i + 1][j] + world[i][j + 1] + world[i + 1][j + 1] + world[i + 1][j - 1]);
                        }
                    }
                    else if (j - 1 < 0) {

                        if (i + 1 > rows) {

                            fate = (world[i - 1][j] + world[i][j + 1] + world[i - 1][j + 1]);
                        }
                        else {

                            fate = (world[i - 1][j] + world[i + 1][j] + world[i][j + 1] + world[i + 1][j + 1] + world[i - 1][j + 1]);
                        }
                    }
                    else if (i + 1 > rows) {

                        if (j + 1 > grid) {

                            fate = (world[i - 1][j] + world[i][j - 1] + world[i - 1][j - 1]);
                        }
                        else {

                            fate = (world[i - 1][j] + world[i][j - 1] + world[i][j + 1] + world[i - 1][j - 1] + world[i - 1][j + 1]);
                        }
                    }
                    else if (j + 1 > grid) {

                        if (i - 1 < (rows - grid_part)) {

                            fate = (world[i][j - 1] + world[i + 1][j] + world[i + 1][j - 1]);
                        }
                        else {

                            fate = (world[i - 1][j] + world[i][j - 1] + world[i + 1][j] + world[i - 1][j - 1] + world[i + 1][j - 1]);
                        }
                    }
                    else {
                        fate = (world[i - 1][j] + world[i][j - 1] + world[i + 1][j] + world[i][j + 1] + world[i - 1][j - 1] + world[i + 1][j + 1] + world[i - 1][j + 1] + world[i + 1][j - 1]);
                    }

		    //decides fate of current cell
                    if (fate < 2) { update[count][j] = 0; }
                    else if (fate == 2) {

                        if (world[i][j] == 0) {

                            update[count][j] = 0;
                        }
                        else {
                            update[count][j] = 1;
                        }
                    }
                    else if (fate == 3) {

                        if (world[i][j] == 0) {

                            update[count][j] = 1;
                        }
                        else {

                            update[count][j] = 1;
                        }
                    }
                    else if (fate > 3) {

                        update[count][j] = 0;
                    }
                }

                count++;
            }

            count = 1;

            //updates current world
            for (int i = (rows - grid_part) + 1; i < rows - 1; i++) {

                for (int j = 1; j < grid - 1; j++) {

                    world[i][j] = update[count][j];
                }

                count++;
            }
	
	    //option to display world
           /* for (int i = (rows - grid_part); i < rows; i++) {

                for (int j = 0; j < grid; j++) {

                    if (world[i][j] == 0) {

                        cout << " ";

                    }
                    else {

                        cout << " *";

                    }

                }

                cout << endl;

            }*/
            //excutes proccess in order
            //MPI_Send(&buffer, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if(!rank){

	    time2 = MPI_Wtime();

	    cout << "time: " << time2-time1 << endl;
    }

    MPI_Finalize();

    return 0;
}
