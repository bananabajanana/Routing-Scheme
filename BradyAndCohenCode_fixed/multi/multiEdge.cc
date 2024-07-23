// multiEdge.cc:  Method definitions for the [undirected] multiGraph's edge
// object.

#include "multiEdge.h"

// -------------
// Constructor.
// -------------

// Receives an ID label, a creation time, and endpoints,
// and enables defaults for all data members.

multiEdge::multiEdge( int ID, int ctime, int node1_arg, int node2_arg ) {

   id = ID;

   node1 = node1_arg;
   node2 = node2_arg;

   color = 0;
   weight = 0;

   creationTime = ctime;
   updateTime = creationTime;
}

// Copy constructor.  Does not notify adjacent vertices;
// that's the graph's job.

multiEdge::multiEdge( multiEdge& source ) {
   
   id = source.id;
   
   creationTime = source.creationTime;
   updateTime = source.updateTime;

   color = source.color;
   weight = source.weight;

   node1 = source.node1;
   node2 = source.node2;
}
