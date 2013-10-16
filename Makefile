# default target "all" builds the executable file 'runmaze' and the
# executable 'sort'
all : runmaze sort sort2 sort3

# runmaze is dependent on the following .o (object) files
# add any new .o files to the list
# build it using g++
runmaze : runmaze.o MazeRunner.o RandomMazeRunner.o Maze.o SquareMaze.o VisualizeSquareMazeRunner.o GPKernel.o BFMazeRunner.o
	g++ -o runmaze -g runmaze.o MazeRunner.o RandomMazeRunner.o Maze.o SquareMaze.o VisualizeSquareMazeRunner.o GPKernel.o BFMazeRunner.o -L/usr/X11R6/lib -lX11

# runmaze.o is dependant on one .cpp file, and several .h files
# IMPORTANT: when you #include a new .h file, be sure to add it here.  Otherwise, the 'make' command may not re-build your program even though you changed a file.
# -Wall: make the compiler display all warnings it knows about; hopefully it'll catch something for us
# -c: only compile, don't link (we do that above)
# -g: put in debug information, for those of you who are using debuggers (like gdb or xxgdb)
runmaze.o : runmaze.cpp Maze.h SquareMaze.h MazeRunner.h RandomMazeRunner.h VisualizeSquareMazeRunner.h BFMazeRunner.h DHeap.h DHeap.cpp
	g++ -Wall -c -g runmaze.cpp

# and same idea for other .o files

BFMazeRunner.o : BFMazeRunner.cpp BFMazeRunner.h
	g++ -Wall -c -g BFMazeRunner.cpp

MazeRunner.o : MazeRunner.cpp Maze.h MazeRunner.h
	g++ -Wall -c -g MazeRunner.cpp

RandomMazeRunner.o : RandomMazeRunner.cpp Maze.h MazeRunner.h RandomMazeRunner.h
	g++ -Wall -c -g RandomMazeRunner.cpp

SquareMaze.o : SquareMaze.cpp Maze.h SquareMaze.h
	g++ -Wall -c -g SquareMaze.cpp

Maze.o : Maze.cpp Maze.h
	g++ -Wall -c -g Maze.cpp

VisualizeSquareMazeRunner.o : VisualizeSquareMazeRunner.cpp VisualizeSquareMazeRunner.h Maze.h SquareMaze.h GPKernel.h
	g++ -Wall -c -g VisualizeSquareMazeRunner.cpp

GPKernel.o : GPKernel.c GPKernel.h
	gcc -Wall -c -g GPKernel.c

# sort is dependent on the following .o (object) files. Note that no
# templated files have .o files here. That's because their code is
# included in every file that includes their .h files.  
# add any new .o files to the list 
# build it using g++
sort : sort.o
	g++ -o sort -g sort.o

sort.o : sort.cpp PQueue.h DHeap.h DHeap.cpp
	g++ -Wall -c -g sort.cpp

sort2.o : sort2.cpp PQueue.h DHeap.h DHeap.cpp
	g++ -Wall -c -g sort2.cpp

sort2 : sort2.o DHeap.h DHeap.cpp PQueue.h
	g++ -o sort2 -g sort2.o

sort3.o : sort3.cpp DHeap.h DHeap.cpp PQueue.h
	g++ -Wall -c -g sort3.cpp

sort3 : sort3.o DHeap.h DHeap.cpp PQueue.h
	g++ -o sort3 -g sort3.o

# 'clean' removes any files that were created via the Makefile, leaving you with only your original source code files
clean :
	rm *.o
	rm runmaze
	rm sort

