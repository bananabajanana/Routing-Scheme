// multiEdge.h:  Specification for the [undirected] multiGraph's edge object.

#ifndef _MULTIEDGE_H
#define _MULTIEDGE_H

#include "multiNode.h"

using namespace std;

class multiNode;

class multiEdge {

   public:

      // -------------
      // Constructor.
      // -------------

      // Receives an ID label, a creation time, and endpoint IDs,
      // and enables defaults for all data members.

      multiEdge( int, int, int );
      
      // Copy constructor.

      multiEdge( multiEdge& source );

      // -------------------
      // Accessor functions.
      // -------------------

      // Return or assign values of various data members.

      int getId() const {
         return id;
      }

      // Return pointers to specified endpoint nodes.

      int getNode1() const {
         return node1;
      }

      int getNode2() const {
         return node2;
      }

   private:

      // -------------
      // Data members.
      // -------------

      // ID label for the current edge.

      int id;

      // Pointers to endpoint nodes.

      int node1;
      int node2;

};

#endif
