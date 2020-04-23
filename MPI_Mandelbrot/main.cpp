#include<iostream>
#include<stdlib.h>
#include<mpi.h>
#include<unistd.h>
#include<fstream>

using namespace std;

//structure representig a complex number
struct Complex{

	double r;
	double i;
};

//operators to use in complex math
Complex operator + (Complex s, Complex t){

	Complex v;
	v.r = s.r + t.r;
	v.i = s.i + t.i;
	return v;
}

Complex operator * (Complex s, Complex t){

	Complex v;
	v.r = s.r*t.r - s.i*t.i;
	v.i = s.r*t.i + s.i*t.r;
	return v;
}

//controls the shade of a certain color for the mandelbrot
int red(int i){
if(i == 255){return 50;}else{return 32*( i%100);};
}

//function that returns a complex number for a pixel
int Mbrot(Complex c, int maxi){

int i = 0;
Complex z;
z = c;

while(i < maxi && z.r*z.r + z.i*z.i < 4){

	z = z*z+c;
	i++;
}

return i;
}

int main(int argc, char **argv){

int rank, size;

//variables for the size of mandelbrot
int const DIM = 512;
int dest = 1;
int row = 0;
int end = 0;
int count = 0;
int pix[DIM];

double time1, time2;

ofstream fout;
	
//complex variables
Complex c0, c1, c;

c0.r = 2;
c0.i = 2;
c1.r = -2;
c1.i = c0.i + (c1.r - c0.r);

MPI_Init(&argc, &argv);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
MPI_Comm_size(MPI_COMM_WORLD, &size);

if(rank == 0){

time1 = MPI_Wtime(); 

//writes out to file
fout.open("image.ppm");

fout << "P3" << endl;
fout << DIM << " " << DIM << endl;
fout << 255 << endl;
}

while(!end){

//process sends rows to other processes to work on 
if(!rank){	
	
MPI_Send(&row, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);

dest++;
row++;

if(dest == size){

dest = 1;
}

if(row == DIM){
	
end = 1;
}

//receives rows built from other processes and puts picture together
MPI_Recv(pix, DIM, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

for(int j = row; j < row + 1; j++ ){

 for(int k = 0; k < DIM; k++){
 
 fout << pix[k] << " ";
 fout << 0 << " ";
 fout << 0 << " "; 
 }

 fout << endl;
}
}else{

//receives assinged rows and builts part of image
MPI_Recv(&row, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

for(int j = row; j < row + 1; j++ ){

 for(int k = 0; k < DIM; k++){

 c.r = (k*(c0.r-c1.r)/DIM)+c1.r;
 c.i = (j*(c0.i-c1.i)/DIM)+c1.i;

 int pixel = Mbrot(c,255); 
	 
 pix[k] = red(pixel);
 }

 //sends parts of image to main process
 MPI_Send(pix, DIM, MPI_INT, 0, 0, MPI_COMM_WORLD);
}

count++;

if(count == (DIM/(size-1))){

end = 1;
}
}
}

//waits for all processes to end tasks
MPI_Barrier(MPI_COMM_WORLD);

fout.close();

if(rank == 0){

//times entire procedure
time2 = MPI_Wtime();

cout << "timing: " << time2 - time1 << endl;
};

MPI_Finalize();

return 0;
}
