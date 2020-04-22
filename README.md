- Parallel programing using MPI
- Programing language is c++
- fun projects and interesting ways to 
  increase running times of programs
  using parallel programing

**************************************************************

MPI_TimeBomb is a fun little game where a time bomb gets passed
around all the processes decrementing everytime the time bomb 
is received. Once the time one hits 0 the process that is holding
it is announced. This is a good intro as to understand how 
processes talk to each other in parallel programing.

***************************************************************

MPI_Sort Uses different processes to sort a list of intergers.
The idea of the sort works like the merge sort technique where
instead of recursively sorting sub arrays of the main array, the 
progam uses each process to sort the sub arrays individually and
then merge the sub arrays into a sorted main array.

****************************************************************
