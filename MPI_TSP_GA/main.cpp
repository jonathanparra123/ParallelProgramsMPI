#include<iostream>
#include<stdlib.h>
#include<mpi.h>
#include<unistd.h>
#include<time.h>
#include<math.h>
#include<cmath>

using namespace std;

bool Check(double a[][2], double x, double y, int s);
int Locate(double a, double b[][3]);
void Print(double a[]);

int main(int argc, char **argv){
	
int rank, size;

MPI_Init(&argc, &argv);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
MPI_Comm_size(MPI_COMM_WORLD, &size);

//double variables
double dist = 0;
double cost = 1000000000;
double sum = 0;
double t1, t2;

//int variables
int place_visit = 1;
int path = 0;
int i = 0;
int j = 0;
int number_places = 10;
int sub = 3;
int stag = 0;
int new_gen = 0;
int start_sub = 1;

srand (time(NULL));

//array with x and y coordinates. they also have id.
double map[number_places][3] = { 
  {179140, 750703, 0},
  {78270, 737081, 1},
  {577860, 689926, 2},
  {628150, 597095, 3},
  {954030, 510314, 4},
  {837880, 811285, 5},
  {410640, 846947, 6},
  {287850, 600161, 7},
  {270030, 494359, 8},
  {559020, 199445, 9}
  };
  
double visited[number_places][2] = {
{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}
};
double possible_solution[number_places+1] = {0,0,0,0,0,0,0,0,0,0,0};  
double temp[number_places+1];

//start timer to find optimal solution
t1 = MPI_Wtime();

while(true){
	
//one process checks if better path way has been found
if(!rank){

for(int i = 1; i < size; i++){
	
MPI_Recv(possible_solution,number_places+1,MPI_DOUBLE,MPI_ANY_SOURCE,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

//keps best score
if(cost > possible_solution[10]){

for(int j = 0; j < number_places+1; j++){

temp[j] = possible_solution[j];

}

cost = temp[10];

//sends best solution
MPI_Send(&new_gen,1,MPI_INT,i,0,MPI_COMM_WORLD);

}else{

//if solution stagnates for to long it will send another section of DNA
stag++;
	
MPI_Send(&new_gen,1,MPI_INT,i,0,MPI_COMM_WORLD);

}
}

if(stag >= size-1){

new_gen = 1;

for(int l = 1; l < size; l++){
	
MPI_Send(&new_gen,1,MPI_INT,l,0,MPI_COMM_WORLD);

MPI_Send(temp,number_places+1,MPI_DOUBLE,l,0,MPI_COMM_WORLD);

}

}

new_gen = 0;
stag = 0;

//once optimal solution has been found, it will stop progam 
if(cost < 2700000){

t2 = MPI_Wtime();	
	
Print(temp);
cout << "cost: " << cost << endl;
cout << "time: "<< t2-t1 << endl;
cout << "\n";
	
break;
}

}else{

//other processes are creating unique solutions at random
visited[0][0] = map[0][0];
visited[0][1] = map[0][1];

while (path < number_places-1){

j = (rand()+rank) % 9 + 1; 

if(Check(visited, map[j][0], map[j][1], place_visit)){  

//math for distance between elements
dist = sqrt(pow(map[i][0]-map[j][0],2.0) + pow(map[i][1]-map[j][1],2.0));

sum += dist;

visited[place_visit][0] = map[j][0]; 
visited[place_visit][1] = map[j][1];

place_visit++;

i = j;

path++;

bool found = false;
int itr = 1;

while(!found){

if(possible_solution[itr] == 0){
	
possible_solution[itr] = map[j][2];

found = true;

}
itr++;
}
}
}

possible_solution[10] = sum;

sum = 0;
path = 0;
place_visit = 1;

//sends unique solution
MPI_Send(possible_solution,number_places+1,MPI_DOUBLE,0,0,MPI_COMM_WORLD);

for(int k = 0; k < number_places; k++){

visited[k][0] = 0;
visited[k][1] = 0;
}

for(int c = 0; c < number_places; c++){
possible_solution[c] = 0;
}

//recieves DNA from succefull solution and mixes with each process's unique solution
MPI_Recv(&new_gen,1,MPI_INT,MPI_ANY_SOURCE,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

if(new_gen == 1){

MPI_Recv(temp,number_places+1,MPI_DOUBLE,MPI_ANY_SOURCE,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

temp[0] = 0;

i = 0;

visited[0][0] = map[0][0];
visited[0][1] = map[0][1];
int loc = 1;

//mixes DNA coming in with DNA created from process
for(int a = start_sub; a < sub+start_sub; a++){

possible_solution[a] = temp[a];

j = Locate(temp[a], map);

dist = sqrt(pow(map[i][0]-map[j][0],2.0) + pow(map[i][1]-map[j][1],2.0));

i = j;

visited[loc][0] = map[i][0];
visited[loc][1] = map[i][1];

sum += dist;

path++;

place_visit++;

loc++;

if(sub+start_sub >= number_places){
	
start_sub = 1;
}
}

start_sub++;
}

new_gen = 0;
}

MPI_Barrier(MPI_COMM_WORLD);
}

MPI_Finalize();

return 0;
}

//checks if location has been visited
bool Check(double a[][2], double x, double y, int s){

bool checking = true;

for(int i = 0; i < s; i++){

if(x == a[i][0] && y == a[i][1]){

checking = false;
}
}

return checking;
}

//locates certain element in array
int Locate(double a, double b[][3]){

int place = 0;
int flag = false;

while(!flag){

if(a == b[place][2]){ 
  flag = true;
}else{
	
place++;
}
}

return place;
}

//prints out array and path
void Print(double a[]){	
	
for(int j = 0; j < 9; j++){
	
cout << a[j] << " --> " << a[j+1] << endl;
}
};
