// multiNode.cc:  Method definitions for the [undirected] multiGraph's
// node object.

#include "multiNode.h"

// -------------
// Constructors.
// -------------

// Receives an ID parameter and a creation time as its first and second
// arguments, and enables defaults for all other data members.

multiNode::multiNode( int ID, int ctime ) {

   id = ID;
   creationTime = ctime;
   updateTime = creationTime;

   color = 0;
   weight = 0;

   degreeOffset = 0;
}

// Copy constructor.  Does not transfer any information about adjacent edges;
// that's the graph's job.

multiNode::multiNode( multiNode& source ) {
   
   id = source.id;
   
   creationTime = source.creationTime;
   updateTime = source.updateTime;

   color = source.color;
   weight = source.weight;

   degreeOffset = 0;
   
   incidentEdges.clear();
   adjacentVertices.clear();
}

// -------------------
// Accessor functions.
// -------------------

// Fill the given vector with all IDs of all edges incident to this
// node.  Be sure to clear the vector first.

void multiNode::getEdges( vector<int>& allEdges ) {

   allEdges.clear();

   hash_set<int>::iterator iter;

   iter = incidentEdges.begin();
   
   while ( iter != incidentEdges.end() ) {
   
      allEdges.push_back( *iter );
      iter++;
   }
}

// Fill the given vector with all IDs of all nodes adjacent to this
// node.  Be sure to clear the vector first.

void multiNode::getNeighbors( vector<int>& allNodes ) {

   allNodes.clear();
   
   hash_set<int>::iterator iter;
   
   if ( adjacentVertices.size() != 0 ) {
      
      iter = adjacentVertices.begin();

      while( iter != adjacentVertices.end() ) {

         allNodes.push_back( *iter );
         iter++;
      }
   }
}

// Add a neighbor to the list.

bool multiNode::addNeighbor( int newNeighborID ) {

   bool ret_val;

   if ( newNeighborID == id ) {

      // That's us.  Somebody added a loop.
      // Increment degree-offset.

      degreeOffset++;
   }

   if ( hasNeighbor( newNeighborID ) ) {

      ret_val = false;

   } else {

      adjacentVertices.insert(newNeighborID);
      ret_val = true;
   }

   return ret_val;
}

// Add an incident edge to the list.

bool multiNode::addEdge( int newEdgeID ) {

   bool ret_val;

   if ( hasEdge( newEdgeID ) ) {

      ret_val = false;

   } else {

      incidentEdges.insert(newEdgeID);
      ret_val = true;
   }

   return ret_val;
}

// Check to see whether this node is adjacent to the specified edge.

bool multiNode::hasEdge( int targetEdgeID ) {

   bool ret_val;

   hash_set<int>::iterator iter = incidentEdges.find(targetEdgeID);

   if ( iter != incidentEdges.end() ) {
      ret_val = true;
   } else {
      ret_val = false;
   }

   return ret_val;
}

// Check to see if this node is adjacent to the specified node.

bool multiNode::hasNeighbor( int targetNodeID ) {


   bool ret_val;

   hash_set<int>::iterator iter = adjacentVertices.find(targetNodeID);

   if ( iter != adjacentVertices.end() ) {
      ret_val = true;
   } else {
      ret_val = false;
   }

   return ret_val;
}

void multiNode::decrementDegreeOffset() {
   degreeOffset--;
}
