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

      multiEdge( int, int, int, int );
      
      // Copy constructor.

      multiEdge( multiEdge& source );

      // -------------------
      // Accessor functions.
      // -------------------

      // Return or assign values of various data members.

      int getId() const {
         return id;
      }

      int getCreationTime() const {
         return creationTime;
      }

      int getColor() const {
         return color;
      }

      void setColor( int colorArg ) {
         color = colorArg;
      }

      double getWeight() const {
         return weight;
      }

      void setWeight( double weightArg ) {
         weight = weightArg;
      }

      int getUpdateTime() const {
         return updateTime;
      }

      void setUpdateTime( int updateArg ) {
         updateTime = updateArg;
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

      // Current edge's color class.  It's a discrete concept, so we use
      // an int.

      int color;

      // Current edge's weight.  Totally arbitrary concept, hence the double.

      double weight;

      // Creation time, in the context of the graph containing the current
      // edge.

      int creationTime;

      // Time of last update, i.e. color change, weight change, etc.
      // Defaults to creationTime.

      int updateTime;
};

#endif
