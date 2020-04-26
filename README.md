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

MPI_Mandelbrot creates a file with a Mandelbrot image in it.
The program decignates a part of the image to each process to 
work on. Once each individual process calculates its own portion
of the picture, it will send its portion to the main process 
where the main process will put the sections together to complete
a Mandelbrot image to a ppm file.

******************************************************************

MPI_GameLife an implementation of the classic game of life program
which takes a certain cell and looks at its neighbors using
parallel programming. The cell's survivability depends on the number 
of neighbors it currently has. The program goes through iterations 
of days where we can see how the population is doing overall. I 
also made an option that shows the grid and how the cells are 
reacting with each iteration.

******************************************************************

MPI_TSP_GA is a interesting project where we tackle the traveling 
salesmen problem using a genetic algorithm approach. We start
by having each process create a unique solution at random. Once
the solutions have been created, they are send to one process
where it reviews all the different solutions and picks the most
optimal one. If the main process does not recieve a better solution
in a couple of iterations, it will send a portion of the most 
optimal solution to the other processes in order to mix in
and give birth to even better solutions using the sent portion
as a base. This genetic algorithm sends DNA of optiomal solutions
to different populations in hope of created the closes solution
to a perfect one using parallel programing.
