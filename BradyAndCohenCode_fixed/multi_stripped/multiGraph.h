// multiGraph.h: Specification for [undirected] multigraph object.

#ifndef _MULTIGRAPH_H
#define _MULTIGRAPH_H

#include <cstdlib>
#include <ctime>
//#include <hash_set>
#include <backward/hash_set>

#include <map>
#include <set>

#include "time.h"

#include "multiNode.h"
#include "multiEdge.h"
#include "../util.h"

//using namespace stdext;

class multiGraph {

   typedef hash_map<int, multiNode*> node_hash_t;
   typedef hash_map<int, multiEdge*> edge_hash_t;

   typedef hash_set<int> int_hash_set;

   typedef hash_map<int, int_hash_set > intKey_ID_map_t;
   typedef map<double, int_hash_set > doubleKey_ID_map_t;

   public:

      // -------------
      // Constructors.
      // -------------

      // Create an empty graph, initializing all data members to reasonable
      // defaults.

      multiGraph();

      // Copy constructor.

      multiGraph( multiGraph& source );

      // Create a graph on n nodes with no edges.

      multiGraph( int numNodes );

      // Parametric constructor for prepackaged graphs, differentiated by the
      // 'graphType' argument:
      //    'K': Create a complete graph on numNodes nodes.

/*
      NOT YET IMPLEMENTED:

      //    'P': Create a path on numNodes nodes.
      //    'T': Create a random tree on numNodes nodes.
      //    '#', where # is an integer: Create a balanced #-ary tree on
                  numNodes nodes.
*/

      multiGraph( char graphType, int numNodes );

      // --------------
      // Destructor.
      // --------------

      ~multiGraph() {
         
         nodes.clear();
         edges.clear();
         degreeHash.clear();
      }

      // --------------------
      // Public data members.
      // --------------------

      // This graph's set of nodes.  Each is indexed by a unique integer.
      // The current index is tracked by nextNodeIndex.

      node_hash_t nodes;

      // This graph's set of edges.  Each is indexed by a unique integer.
      // The current index is tracked by nextEdgeIndex.

      edge_hash_t edges;

      // Bi-level attribute-tracking maps for nodes.
      //
      // The first level represents a particular value of the attribute
      // being tracked, and the second level is a hash_set of node IDs
      // matching this value in the tracked attribute.
      //
      // E.g.: colorHash[5] is a hash_set of node IDs (integers) representing
      // all nodes with (color == 5).

      intKey_ID_map_t degreeHash;

      // --------------------------------------------
      // Graph-level accessor & assignment functions.
      // --------------------------------------------

      // Return an edge count for the graph. [redundant, but conceptually
      // handy.]

      int edgeSize() const {

         return edges.size();
      }

      // Return a node count for the graph. [redundant, but conceptually
      // handy.]

      int nodeSize() const {

         return nodes.size();
      }

      // Report the graph's average degree.  Calculate this on-the-fly.

      double avgDegree() {

         if ( !nodes.empty() ) {

            double ret_val = ( (double) (edgeSize() * 2) / (double) nodeSize() );
            return ret_val;

         } else {

            return 0;
         }
      }

      // Report the graph's maximum degree. Calculate this on-the-fly.

      int maxDegree( void ) {

         int maxDegree = 0;

         intKey_ID_map_t::iterator iter;

         for (iter = degreeHash.begin(); iter != degreeHash.end(); iter++) {

            if ( (iter->first) > maxDegree ) {

               maxDegree = (iter->first);
            }
         }

         return maxDegree;

      }
      
      // Report the diameter [in hops].

      int hopDiameter( void );

      // -----------------------------
      // Graph manipulation functions.
      // -----------------------------

      // Add a node & return its ID, or -1 if unsuccessful.
      // [Increment nextNodeIndex.]

      int addNode( void );

      // Delete the node with the given index or pointer to a node.
      // Returns the ID of the node if successuful, or -1 if it was not

      int deleteNode( int targetNode );
      int deleteNode( multiNode* targetNode );

      // Add an edge between two named vertices & return a pointer to it.
      // If one of the nodes does not exist, return -1.

      int addEdge( int node_1, int node_2 );
      int addEdge( multiNode* node_1, multiNode* node_2 );

      // Delete _some_ edge between two specified nodes, or matching
      // a specified ID.

      void deleteEdge( int node_1, int node_2 );
      void deleteEdge( multiNode* node_1, multiNode* node_2 );

      void deleteEdge( int edgeID );

      // Delete all edges incident to the given node.

      void deleteAllIncidentEdges( int targetNode);

      // Use only when scanning input files:
      
      void setNodeIndex(int newIndex) {
         nextNodeIndex = newIndex;
      }

      void setEdgeIndex(int newIndex) {
         nextEdgeIndex = newIndex;
      }

   private:

      // Counter to keep track of next [unique] index for a new node/edge.

      int nextNodeIndex;
      int nextEdgeIndex;

      // ------------------------
      // Private Member Functions
      // ------------------------
      
      // Data initializer called by the constructors.

      void multiGraph::init() {

         nextNodeIndex = 0;
         nextEdgeIndex = 0;

      }

};

#endif
