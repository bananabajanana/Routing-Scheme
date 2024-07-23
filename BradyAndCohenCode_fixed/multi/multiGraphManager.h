// multiGraphManager.h: Specification for file & object controller wrapping
// [undirected] multigraph object.

#ifndef _MULTIGRAPH_MANAGER_H
#define _MULTIGRAPH_MANAGER_H

#include <iostream>
#include <fstream>
#include <assert.h>

//#include <hash_map>
#include <backward/hash_map>
#include "multiGraph.h"

typedef struct {
   
   string tagName;
   
   char attribute;
   char attribute_1;
   char attribute_2;
   char graphType;
   
   int color;
   int  ID;
   int _ID;
   int node;
   int  node_1;
   int _node_1;
   int  node_2;
   int _node_2;
   int  nodeID_1;
   int  nodeID_2;
   int nodeSize;

   short directed;
   short multi;
   
   double value;
   double value_1;
   double value_2;
   double weight;
      
} xmlTag;

class multiGraphManager {

   typedef hash_map<int, multiNode*> node_hash_t;
   typedef hash_map<int, multiEdge*> edge_hash_t;

   public:

      // -------------
      // Constructors.
      // -------------

      // Simplest constructor: Create an empty graph, init all data members
      // to reasonable defaults, open a stream for an XML output file.
      // Overwrite any preexisting file with this name.

      multiGraphManager( char* outputfilename );

      // Copy constructor.

      multiGraphManager( multiGraphManager& source );

      // Create a graph with n nodes and no edges, & open a stream for
      // an XML output file.  Overwrite any preexisting file with this name.

      multiGraphManager( int numNodes, char* outputfilename );

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

      multiGraphManager( char graphType, int numNodes, char* outputfilename );

      // --------------
      // Destructor.
      // --------------

      ~multiGraphManager() {

         delete G;

         if ( recording ) {
            pause();
         }

         if (output_file_stream.is_open()){
             output_file_stream << "</graph>\n";
             output_file_stream.close();
         }

      }

      // --------------------
      // Public data members.
      // --------------------

      node_hash_t* nodes;
      edge_hash_t* edges;

      // -----------------------------------------------------
      // Wrapped graph-object accessor & assignment functions.
      // -----------------------------------------------------

      // Return an edge count for the contained graph.

      int edgeSize( void ) {
         return G->edgeSize();
      }

      // Return a node count for the contained graph.

      int nodeSize( void ) {
         return G->nodeSize();
      }

      // Return the contained graph's current time marker.

      int getTime( void ) {
         return G->getTime();
      }

      // Increment the contained graph's time counter by one step.

      void incTime( void ) {
         G->incTime();
      }

      // Report the contained graph's average degree.

      double avgDegree( void ) {
         return G->avgDegree();
      }

      // Report the graph's maximum degree.

      int maxDegree( void ) {
         return G->maxDegree();
      }

      // Set node attributes for specified nodes.

      void setNodeColor( int nodeID, int color ) {
         G->setNodeColor(nodeID, color);
      }

      void setNodeWeight( int nodeID, double weight ) {
         G->setNodeWeight(nodeID, weight);
      }

      // Report the diameter [in hops].

      int hopDiameter( void ) {
         return G->hopDiameter();
      }

      // -------------------------------------
      // Wrapped graph manipulation functions.
      // -------------------------------------

      // Add a node to the contained graph & return its ID.

      int addNode( void );

      // Delete the node with the given index.

      void deleteNode( int targetNode );

      // Delete a random node.

      void deleteRandomNode( void );

      // Delete a random node where a given attribute [first argument; see
      // table] matches a given value [second argument].  Cast second argument
      // to int as necessary.
      //
      // Attributes:
      //
      // 'c': color:    node's color matches second argument.  (int)
      // 'H': gtweight: node's weight is >= the second argument. (double)
      // 'h': ltweight: node's weight is <= the second argument. (double)
      // 'w': weight:   node's weight is == the second argument. (double)
      //                Be careful with float comparisons; use epsilons
      //                to test equality.
      // 'G': gtdegree: node's degree is >= the second argument. (int)
      // 'g': ltdegree: node's degree is <= the second argument. (int)
      // 'd': degree:   node's degree is == the second argument. (int)
      //
      // Returns 0 if successful, -1 if no such node was found.

      int deleteRandomNode( char attribute, double value );

      // Add an edge between two named vertices & return its ID.
      // If one of the nodes does not exist, return -1.

      int addEdge( int node_1, int node_2 );

      // Add a random edge, return its ID.

      int addRandomEdge( void );

      // Add a random edge to a given node.

      int addRandomEdge( int node_1 );

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

      // Delete _some_ edge between two specified nodes.

      void deleteEdge( int node_1, int node_2 );

      // Delete a specified edge.

      void deleteEdge( int edgeID );

      // Delete a random edge.

      void deleteRandomEdge( void );

      // Delete a random edge incident to a specified node.

      void deleteRandomEdge( int targetNode );

      // Delete a random edge indicent to a node matching specified attributes,
      // as in addRandomEdge( char, double ) above.

      void deleteRandomEdge( char attribute, double value );

      // Delete a random edge between two nodes matching specified attributes,
      // as in addRandomEdge( char, double, char, double ) above.
      //
      // *****************************
      // DEPRECATED FOR THE TIME BEING
      // *****************************

      void deleteRandomEdge( char attribute_1, double value_1,
			      char attribute_2, double value_2 );

      // ------------------------------
      // GraphManager-specific methods.
      // ------------------------------

      // Pause output-recording.

      void pause( void );

      // Switch on output-recording.

      void record( void );

      // Enable extra structural info in recording graph operations.

      void enableStructureInfo( void ) {

         structureInfo = 1;
      }

      // Disable extra structural info in recording graph operations.

      void disableStructureInfo( void ) {

         structureInfo = 0;
      }

      // Process an XML input file, breaking it first into update-sequences
      // and snapshots.  Scan to last snapshot, create a graph to match, then
      // process all subsequent updates.
      //
      // Possibly to be made more flexible in future, e.g. reporting some
      // graph statistic at each snapshot, etc.

      void processFile( char* filename );

      // Write a snapshot of the current graph to the output file.

      void snapshot( void );
      
      // This should undoubtedly be private, but it's going to have to wait.

      multiGraph* G;

   private:

      const char* output_file_name;
      ofstream output_file_stream;

      int recording;
      int structureInfo;

      // Input parsing functions called by processFile().  Change argument
      // type to reflect XML parse tree, or whatever internal data structure
      // ends up representing the appropriate chunk of input data.

      void processUpdates( vector<xmlTag>& );
      void processSnapshot( vector<xmlTag>& );
};

#endif
