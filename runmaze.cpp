/*
   runmaze.C: contains 'main' function.
 */

#include <iostream>
#ifdef WIN32
#ifdef _DEBUG
#include <conio>
#endif
#endif
#include <cstring>
#include <cstdlib>

#include "Maze.h"
#include "SquareMaze.h"
#include "MazeRunner.h"
#include "RandomMazeRunner.h"
#include "VisualizeSquareMazeRunner.h"

#include "BFMazeRunner.h"
#include "PQueue.h"
#include "UnsLLPQueue.h"
#include "Compare.h"
#include "DHeap.h"

int main (int argc,char *argv[])
{

    SquareMaze *maze;

    bool showVisualization=false;
    bool showSolutionMode=false;
    double visualizationPauseInSec=0.1;

    // process command-line args
    // 221 STUDENTS: you may safely ignore (or even delete) the implementation
    // processing Command Line Arguments.  We will test your program
    // with no command-line arguments.  (You are free to leave the call
    // here and, e.g., use the visualization for your debugging,
    // however!)
    int a;
    a=1;
    while (a<argc) {

	if (strcmp(argv[a],"-h")==0 || strcmp(argv[a],"--help")==0 || strcmp(argv[a],"-?")==0) {
	    cout << "runmaze usage:\nrunmaze [-p <secs>] [-v] [-s]\n";
	    cout << "-v : show visualization";
	    cout << "-p <sec> : pause for <sec> seconds each time a new node is visited.  Only valid with -v (visualization).  Default pause time is " << visualizationPauseInSec << "\n";
	    cout << "-s : \"Show solution Mode\".  The input to the program will be a maze, followed by one or more solutions.  Show each solution.\n";
	    cout << "NOTE: visualization is not implemented under Windows\n";
	    return 0;
	}
	if (strcmp(argv[a],"-p")==0) {
	    a++;
	    if (a>=argc) {
		cout << "-p flag given, but <sec> did not follow.  Run with -h flag for help\n";
		exit(1);
	    }
	    visualizationPauseInSec=atof(argv[a]);
	}
	if (strcmp(argv[a],"-v")==0) {
	    showVisualization=true;
	}
	if (strcmp(argv[a],"-s")==0) {
	    showSolutionMode=true;
	}
	a++;
    }

    if ((showSolutionMode) && !showVisualization) {
	cout << "WARNING: -s flag given without -v flag.  This has no effect.\n";
    }


    maze=new SquareMaze(cin);
    //maze->DebugDump(cout);

#ifndef WIN32 // this isn't implemented on Windows
    VisualizeSquareMazeRunner *visualizer=NULL;
    if (showVisualization) {
	visualizer=new VisualizeSquareMazeRunner(maze);
	visualizer->setPauseTime(visualizationPauseInSec);

	if (showSolutionMode) {
	    while (!cin.eof()) {
		char solutionAlg[256];
		cin.getline(solutionAlg,256);
		if (cin.eof()) {
		    break;
		}
		cout << " Printing solution done by " << solutionAlg << " algorithm\n";
		int x,y;
		visualizer->startSolutionPath();
		while (SquareMaze::SquareMazeNode::readXY(cin,&x,&y)) {
		    visualizer->addNextInSolutionPath(x,y);
		    //cout << "(" << x << "," << y << ")\n";
		}
		visualizer->doneSolutionPath();

		cout << "Click mouse in solution display window to continue...\n";
		cout << flush;
		visualizer->waitForMouseClick();

	    }
	    delete visualizer;
	    delete maze;
	    return 0;
	}
    }
#endif
	// run bestFristMazeRunner using DHeap and SquareMazeNodeCompare
    MazeRunner *bestFirstMazeRunner;
    DHeap<MazeNode *,SquareMazeNodeCompare >  priorityQ;
    cout << "-----running DHeap through Best First MazeRunner" << endl;
    bestFirstMazeRunner = new BFMazeRunner(&priorityQ);
    bestFirstMazeRunner->solveMaze(maze,cout);
    delete bestFirstMazeRunner;		
	// run bestFirstMazeRunner using supplied UnsLLPQueue and 
    	// SquareMazeNodeCompare.
    maze->reinitializeVisitationStateAndInfo();
    cout << "-----running UnsLLPQueue through Best First MazeRunner" << endl;
    PriorityQueue<MazeNode *,SquareMazeNodeCompare > *priorityQ2;
    priorityQ2 = new UnsLLPQueue<MazeNode *,SquareMazeNodeCompare >();
    bestFirstMazeRunner = new BFMazeRunner(priorityQ2);
    bestFirstMazeRunner->solveMaze(maze,cout);
    delete priorityQ2;
    delete bestFirstMazeRunner;
	// run RandomMazeRunner
    maze->reinitializeVisitationStateAndInfo();

    cout << "-----running RandomMazeRunner" << endl;

    MazeRunner *randomMazeRunner;
    randomMazeRunner=new RandomMazeRunner;

    randomMazeRunner->solveMaze(maze,cout);

    // clean up, in case we need to run another maze runner algorithm (like, oh say BFS)
    maze->reinitializeVisitationStateAndInfo();

#ifdef WIN32
#ifdef _DEBUG
    cout << flush;
    // convenient thing for the MSDEV debugger, so it doesn't exit the program too quickly
    cout << "Press a key to exit program";
    getch();
#endif
#endif


    if (visualizer != NULL)
	delete visualizer;
    delete randomMazeRunner;
    delete maze;
    return 0;
}
