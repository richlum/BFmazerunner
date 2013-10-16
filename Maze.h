#ifndef _MAZE_H
#define _MAZE_H

#include <iostream>
using namespace std;

/*
Maze.h
Defines the abstract types 'Maze', 'MazeNode', 'MazeNodeIterator', 'MazeChangeListener'

Maze: a maze of some kind
MazeNode: a position in the maze (like a room in the maze)
        You may want to associate extra information with a MazeNode (e.g. a
        "visited" flag).  The getInfo and setInfo methods allow you to associate
        a void pointer (usually to a struct).  The RandomMazeRunner class demonstrates
        how to do this.
MazeNodeIterator: a class to get through a list of maze nodes.
        (follows Java style of iterators.  The C++ Standard Template Library (STL)
        style of iterators tend not to work so well when you use abstract base
        classes)
MazeChangeListener: a class that receives for notifications that the state
        of a Maze has changed.  This is used for the visualization class.
*/

class MazeNode; // forward declaration of this class
class MazeNodeIterator {
public:
        // virtual destructor, since we have virtual methods
        virtual ~MazeNodeIterator();

        // returns true if there's another MazeNode here
        virtual bool hasNext (void) = 0;

        // returns the current MazeNode, and moves to the next MazeNode (if any)
        // you can only call 'next' if 'hasNext' returned true
        // NOTE: the returned pointer points to something that was already allocated.  So, you do NOT have to delete it.
        virtual MazeNode * next (void) = 0;
};

class MazeNode {
protected:
        void *info;
public:
        MazeNode();

        // virtual destructor, since we have virtual methods
        virtual ~MazeNode();

        // returns true if this MazeNode is an Exit from the maze
        // (theoretically, we may want to consider mazes with more than one exit)
        virtual bool isExitNode (void) = 0;

        // returns an iterator that allows you to look at the neighbors of
        // this MazeNode.  A neighbor is a node that this directly connects
        // to (like two rooms connected by a door)
        // NOTE: the Iterator is allocated with 'new', so you must delete it
        virtual MazeNodeIterator *getNeighbors (void) = 0;

        // returns the number of neighbors that the MazeNodeIterator returned by
        // getNeighbors would yield.  This may not be much faster than just iterating
        // through them.  This function is just provided for convenience
        virtual int getNumNeighbors (void);

        // get a pointer to the extra info of this MazeNode, as a void pointer
        // you can use this to store an information, such as a "visited" flag
        // info is guaranteed to be initialized to NULL to be with
        void *getInfo (void);
        // set pointer to extra info
        void setInfo (void *newInfo);

        // for visualization
        enum VisitationState {
                Visited,NotVisited,VisitInProgress
        };
protected:
        VisitationState visitationState;
public:
        VisitationState getVisitationState (void);

        // call this function as your algorithm proceeds, so its progress can be displayed
        // derived classes must implement ChangeListener-related functionality
        virtual void setVisitationState (VisitationState newVisitationState);

        // print the node (for example, as part of a solution to the maze)
        virtual void print (ostream& out) = 0;
};

class MazeChangeListener {
public:
        // virtual destructor, since we have virtual methods
        virtual ~MazeChangeListener();

        virtual void visitationStateChanged (MazeNode *changedNode) = 0;
};

class Maze {
protected:
        MazeChangeListener *changeListener;
public:
        Maze ();

        // virtual destructor, since we have virtual methods
        virtual ~Maze();

        // returns a pointer to the MazeNode corresponding to the Start position in the maze
        // NOTE: the returned pointer points to something that was already allocated.  So, you do NOT have to delete it.
        virtual MazeNode *getStartMazeNode (void) = 0;

        // change the MazeChangeListener for this class.  We only support one listener
        // per maze to make things simpler
        void setChangeListener (MazeChangeListener *newChangeListener);

        // retrieves a MazeNodeIterator that allows us to look at every square in the
        // maze
        virtual MazeNodeIterator *getAllMazeNodes (void) = 0;

        // returns the number of nodes in the maze, as a convenience
        virtual int getNumNodes (void) = 0;

        // returns the highest possible number of neighbors that any node may has
        // in this particular maze
        // the number returned may not be a tight bound for some mazes, but
        // for any maze, it's guaranteed to be less than getNumNodes()
        // for a square maze, it'd be at most 4 (but you can't assume this)
        virtual int getMaxNeighborsForNode (void) = 0;

	// for every node in the maze, re-initialize Visitation
	// State to NotVisited, and the info to NULL.
	// This is useful if you want to run another search algorithm
	// on the same maze.
	void reinitializeVisitationStateAndInfo (void);
};

#endif
