#ifndef _MAZERUNNER_H
#define _MAZERUNNER_H

#include <iostream>
#include "Maze.h"

using namespace std;

/*
MazeRunner.h
Defines the abstract type MazeRunner - a class that knows how to solve a maze.
*/

class MazeRunner {
public:
        // virtual destructor, since we have virtual methods
        virtual ~MazeRunner();

        virtual void solveMaze (Maze *maze,ostream& solutionOutput) = 0;
};
#endif
