// multiNode.h:  Specification for the [undirected] multiGraph's node object.

#ifndef _MULTINODE_H
#define _MULTINODE_H

#include <iostream>
//#include <hash_set>
#include <backward/hash_set>
#include <vector>
#include <list>

#include "multiEdge.h"

using namespace std;

class multiEdge;

class multiNode {
   
   private:

      // -------------
      // Data members.
      // -------------

      // ID of the node.  Managed entirely by the containing graph object.

      int id;

      // Corrector for loops.

      int degreeOffset;

      // hash_set whose keys are IDs of adjacent edges.  Values are just
      // dummy values.

      hash_set<int> incidentEdges;

      // hash_set whose keys are IDs of adjacent vertices.  Values are just
      // dummy values.

      hash_set<int> adjacentVertices;
   
   public:

      // -------------
      // Constructors.
      // -------------

      // Receives an ID parameter and a creation time as its first and second
      // arguments, and enables defaults for all other data members.

      multiNode( int );

      // Copy constructor.

      multiNode( multiNode& source );

      // --------------
      // Destructor.
      // --------------

      ~multiNode() {
         
         incidentEdges.clear();
         adjacentVertices.clear();
         
         delete &incidentEdges;
         delete &adjacentVertices;
      }

      // -------------------
      // Accessor functions.
      // -------------------

      // Return or assign values of various data members.

      int getId() const {
         return id;
      }
      
      // Should only be used in scanning input files.
      
      void setId( int idArg ) {
         id = idArg;
      }

      int getDegree() const {
         return this->incidentEdges.size() + degreeOffset;
      }

      // Fill the given vector with all IDs of all edges incident to this
      // node.

      void getEdges( vector<int>& );

      // Fill the given vector with all IDs of all nodes adjacent to this
      // node.

      void getNeighbors( vector<int>& );

      // Drop a neighbor from the adjacency list.

      void removeNeighbor( int oldNodeID ) {

         // If the edge exists,

         if ( hasNeighbor( oldNodeID ) ) {

            // ...remove the adjacency entry.
            adjacentVertices.erase( oldNodeID );
         }
      }

      // Disconnect an edge from this node.

      void removeEdge( int oldEdgeID ) {

         // If the edge exists,

         if ( hasEdge( oldEdgeID ) ) {

            // ...remove it.
            this->incidentEdges.erase( oldEdgeID );
         }
      }

      // Connect a node to this node, referenced by the ID of the node
      // to which we want to connect.

      bool addNeighbor( int );

      // Connect an edge to this node, referenced by the ID of the edge
      // to which we want to connect.

      bool addEdge( int );

      // Check to see whether this node is adjacent to the specified edge.

      bool hasEdge( int );

      // Check to see if this node is adjacent to the specified node.

      bool hasNeighbor( int );

      // Decrement the current degreeOffset, which tracks self-loops.

      void decrementDegreeOffset();

};

#endif
