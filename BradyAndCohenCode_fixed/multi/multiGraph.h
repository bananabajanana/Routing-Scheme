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
using namespace __gnu_cxx; // This might be necessary for older GCC versions
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
	 colorHash.clear();
	 weightHash.clear();
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
      intKey_ID_map_t colorHash;
      doubleKey_ID_map_t weightHash;

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

      // Return the graph's current time marker.

      int getTime() const {

         return currentTime;
      }

      // Increment the time counter by one step.

      void incTime() {

         currentTime++;
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

      // Set node attributes and update the tracking hashes accordingly.

      void setNodeColor( int, int );
      void setNodeWeight( int, double );

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

      // Delete a random node.  Return node ID if successful, -1 otherwise.

      int deleteRandomNode( void );

      // Delete a random node where a given attribute matches a given value.
      // Cast second argument to int as necessary.
      //
      // Attributes:
      //
      // 'c': color:    node's color matches second argument.  (int)
      // 'H': gtweight: node's weight is >= the second argument. (double)
      // 'h': ltweight: node's weight is <= the second argument. (double)
      // 'w': weight:   node's weight is == the second argument. (double)
      // 'G': gtdegree: node's degree is >= the second argument. (int)
      // 'g': ltdegree: node's degree is <= the second argument. (int)
      // 'd': degree:   node's degree is == the second argument. (int)
      //
      // Returns deleted node ID if successful, -1 if no such node was found.

      int deleteRandomNode( char attribute, double value );

      // Add an edge between two named vertices & return a pointer to it.
      // If one of the nodes does not exist, return -1.

      int addEdge( int node_1, int node_2 );
      int addEdge( multiNode* node_1, multiNode* node_2 );

      // Add a random edge, return its ID.

      int addRandomEdge( void );

      // Add a random edge to a given node.

      int addRandomEdge( int node_1 );
      int addRandomEdge( multiNode* node_1 );

      // Add a random edge incident to a node where some attribute [first
      // argument; see table above for deleteRandomNode( char, double )]
      // matches some value [second argument].
      //
      // Returns the edge's ID if successful, -1 if it proved impossible.

      int addRandomEdge( char attribute, double value );

      // Add a random edge incident to a given node where some attribute [first
      // argument; see table above for deleteRandomNode( char, double )]
      // matches some value [second argument] with respect to the node's other
      // endpoint.
      //
      // Returns the edge's ID if successful, -1 if it proved impossible.

      int addRandomEdge( int node_1, char attribute, double value );
      int addRandomEdge( multiNode* node_1, char attribute, double value );

      // Add a random edge _between_ two vertices with specified attribute
      // constraints, as above.
      //
      // Returns the edge's ID if successful, -1 if it proved impossible.

      int addRandomEdge( char attribute_1, double value_1,
                           char attribute_2, double value_2 );

      // Add a random edge _between_ two vertices with specified attribute
      // constraints, as above.  Disallow loops and multiedges.
      //
      // Returns the edge's ID if successful, -1 if it proved impossible.

      int addRandomNewEdge( char attribute_1, double value_1,
                           char attribute_2, double value_2 );

      // Delete _some_ edge between two specified nodes, or matching
      // a specified ID.

      void deleteEdge( int node_1, int node_2 );
      void deleteEdge( multiNode* node_1, multiNode* node_2 );

      void deleteEdge( int edgeID );

      // Delete all edges incident to the given node.

      void deleteAllIncidentEdges( int targetNode);

      // Delete a random edge and return its endpoint IDs.

      vector<int> deleteRandomEdge( void );

      // Delete a random edge incident to a specified node, and return its
      // endpoint IDs.

      vector<int> deleteRandomEdge( int targetNode );
      vector<int> deleteRandomEdge( multiNode* targetNode );

      // Delete a random edge indicent to a node matching specified attributes,
      // as in addRandomEdge( char, double ) above, and return its endpoint
      // IDs.

      vector<int> deleteRandomEdge( char attribute, double value );

      // Delete a random edge between two nodes matching specified attributes,
      // as in <<addRandomEdge( char, double, char, double )>> above, and
      // return its endpoint IDs.
      //
      // *****************************
      // DEPRECATED FOR THE TIME BEING
      // *****************************

      vector<int> deleteRandomEdge( char attribute_1, double value_1,
                                    char attribute_2, double value_2 );

      // Return a random node's ID where 'attribute' matches 'value,' or -1
      // if there isn't one.
      //
      // Attributes:
      //
      // 'c': color:    node's color matches second argument.  (int)
      // 'H': gtweight: node's weight is >= the second argument. (double)
      // 'h': ltweight: node's weight is <= the second argument. (double)
      // 'w': weight:   node's weight is == the second argument. (double)
      // 'G': gtdegree: node's degree is >= the second argument. (int)
      // 'g': ltdegree: node's degree is <= the second argument. (int)
      // 'd': degree:   node's degree is == the second argument. (int)

      int randomNode( char attribute, double value );

      // Grab a random node which isn't of degree zero.

      int randomConnectedNode( char attribute, double value );

      // Grab a random node which is not a neighbor of/equal to
      // the specified node.

      int randomNonNeighbor( char attribute, double value, int nodeID );
      
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

      // The current time.  Time-steps are completely controlled by whatever
      // algorithm is currently running; they can involve multiple or single
      // graph operations, or be more complex.  This field is incremented as
      // necessary, from outside the object, via the incTime() method.
      // It starts at 0.

      int currentTime;

      // ------------------------
      // Private Member Functions
      // ------------------------
      
      // Data initializer called by the constructors.

      void init() {

         nextNodeIndex = 0;
         nextEdgeIndex = 0;
         currentTime = 0;

         // For repeatable random numbers, comment the next two lines.

         time_t rawTime;
         srand( (unsigned)time(&rawTime) );
      }

};

#endif
