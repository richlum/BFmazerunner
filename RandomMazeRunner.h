#ifndef _RANDOMMAZERUNNER_H
#define _RANDOMMAZERUNNER_H

using namespace std;

/*
RandomMazeRunner.h

Goes through the maze randomly, trying to get to the exit.

  The ExtraNodeInfo struct contains extra information the RandomMazeRunner
  stores in each node, mainly in order to remember its solution.

  Basically, we randomly walk around the maze, updating the nextInSolution field
  of ExtraNodeInfo as we go.  Once we reach the exit of the maze, we can
  use the nextInSolution fields to retrieve a solution from the Start to the Exit.

  Argument that the use of nextInSolution is sound (i.e. we won't mess up our
  data if we visit a MazeNode twice): Inductively, we show that at any point in
  time, you can get from the Start node to the current node along
  the nextInSolution fields.
  Base case: obviously this is true when the current node **is** the Start
  node (which is how we begin our random walk)
  Induction step: we've got a current node, and then we randomly select a
  next node (a neighbor of the current node).  We've also got a path
  from the Start node to the current node, using the nextInSolution
  fields.  Now, there are 2 cases:
  (1) the next node we selected is not in the path from the Start node.
  In this case, we just have the path from the Start node to the current
  node, plus the extra hop to the next node.
  (2) the next node we selected is already in the path from the Start
  node.
  But if the next node is already in the path, then we already have a
  (shorter) path from the Start node to it, so we're okay here too.
*/

class RandomMazeRunner : public MazeRunner {
protected:
        struct ExtraNodeInfo {
                MazeNode *nextInSolution; // next in our current solution
        };

        // helper functions
        MazeNode *pickNextNode(MazeNode *currNode);
        ExtraNodeInfo *getExtraNodeInfo(MazeNode *currNode);
        void deleteExtraNodeInfo(Maze *maze);

	   static void initClass();  // initializes general class information
	                             // should only be called once per program
	                             // currently initializes randomizer.
public:
        RandomMazeRunner ();
        ~RandomMazeRunner ();

        void solveMaze (Maze *maze,ostream& solutionOutput);
};

#endif
