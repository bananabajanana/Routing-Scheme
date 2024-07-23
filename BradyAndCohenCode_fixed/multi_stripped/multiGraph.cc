// multiGraph.h: Method definitions for [undirected] multigraph object.

#include "multiGraph.h"
#include <queue>

#define WHITE 0
#define GRAY 1
#define BLACK 2

typedef hash_map<int, multiNode*> node_hash_t;
typedef hash_map<int, multiEdge*> edge_hash_t;

typedef hash_set<int> int_hash_set;

typedef hash_map<int, int_hash_set > intKey_ID_map_t;
typedef map<double, int_hash_set > doubleKey_ID_map_t;

// -------------
// Constructors.
// -------------

multiGraph::multiGraph() {
   init();
}

// Copy constructor.

multiGraph::multiGraph( multiGraph& source ) {
   
   init();
   
   // Iterate through all nodes in the source graph.
   
   node_hash_t::iterator node_iter = source.nodes.begin();
   
   while ( node_iter != source.nodes.end() ) {
      
      int i = node_iter->first;
      
      // Make copies using multiNode's copy constructor.
      
      multiNode* tempNodePtr = new multiNode( *(source.nodes[i]) );
      nodes[i] = tempNodePtr;
      
      node_iter++;
   }
   
   // Iterate through all edges in the source graph.
   
   edge_hash_t::iterator edge_iter = source.edges.begin();
   
   while ( edge_iter != source.edges.end() ) {
      
      int i = edge_iter->first;
      
      // Make copies using multiEdge's copy constructor.
      
      multiEdge* tempEdgePtr = new multiEdge( *(source.edges[i]) );
      edges[i] = tempEdgePtr;
      
      // Make some temp pointers for clarity...
      
      multiEdge* newEdge = edges[i];
      
      multiNode* node_1 = nodes[newEdge->getNode1()];
      multiNode* node_2 = nodes[newEdge->getNode2()];
      
      // Notify adjacent vertices about all necessary info.
      
      node_1->addEdge( newEdge->getId() );

      if ( node_1->getId() != node_2->getId() ) {
         node_2->addEdge( newEdge->getId() );
      }

      node_1->addNeighbor( node_2->getId() );

      if ( node_1->getId() != node_2->getId() ) {
         node_2->addNeighbor( node_1->getId() );
      }
      
      edge_iter++;
   }
   
   // Update the degree hash with the current degrees.
   
   node_iter = nodes.begin();
   
   while ( node_iter != nodes.end() ) {
      
      int i = node_iter->first;
      multiNode* currentNode = node_iter->second;
      
      degreeHash[currentNode->getDegree()].insert(i);
      
      node_iter++;
   }
   
   nextNodeIndex = source.nextNodeIndex;
   nextEdgeIndex = source.nextEdgeIndex;
}

multiGraph::multiGraph( int numNodes ) {

   init();

   // Create numNodes new nodes.

   for (int i = 0; i < numNodes; i++) {
      addNode();
   }
}

multiGraph::multiGraph( char graphType, int numNodes ) {

   init();

   // Create numNodes new nodes.

   for (int i = 0; i < numNodes; i++) {
      addNode();
   }

   // ---------------------------------------
   // Create the specified prepackaged graph.
   // ---------------------------------------

   switch( graphType ) {

   // ---------------
   // Complete graph:
   // ---------------

   case 'K':

      int i, j;

      // Add all necessary edges.

      for (i = 0; i < nextNodeIndex; i++) {

         for (j = i + 1; j < nextNodeIndex; j++) {

            addEdge( nodes[i], nodes[j] );
         }
      }
      break;

   default:
      break;

   } // end switch ( graphType )
}

// --------------------------------------------
// Graph-level accessor & assignment functions.
// --------------------------------------------

// Report the diameter [in hops].

int multiGraph::hopDiameter() {
   
   int maxDistance = 0;
   
   // For each node [*node_iter],
   
   node_hash_t::iterator node_iter = nodes.begin();
   
   while ( node_iter != nodes.end() ) {
      
      // Grow a BFS from [*node_iter] and track the maximum distance
      // to any node u from [*node_iter].
      
      queue<int> Q;

      hash_map<int, int> color;
      hash_map<int, int> distance;
      
      vector<int> neighborList;
      vector<int>::iterator neighborIter;
      
      // Initialize data structures for BFS.
      
      node_hash_t::iterator inner_node_iter = nodes.begin();

      while ( inner_node_iter != nodes.end() ) {
         
         color[inner_node_iter->first] = WHITE;
         distance[inner_node_iter->first] = -1;
         
         inner_node_iter++;
      }
      
      color[node_iter->first] = GRAY;
      distance[node_iter->first] = 0;
      Q.push(node_iter->first);
      
      // For each neighbor of each gray [i.e., seen-so-far-but-not-finished]
      // vertex,
      
      while ( !(Q.empty()) ) {
         
         int current = Q.front();
         
         nodes[current]->getNeighbors(neighborList);
         
         neighborIter = neighborList.begin();
         
         // Mark newly-discovered neighbors...
         
         while( neighborIter != neighborList.end() ) {
            
            if ( color[*neighborIter] == WHITE ) {
               
               color[*neighborIter] = GRAY;
               distance[*neighborIter] = distance[current] + 1;
               Q.push(*neighborIter);
               
               // If we've got a new largest-distance, update maxDistance.
               
               if ( distance[*neighborIter] > maxDistance ) {
                  maxDistance = distance[*neighborIter];
               }
            }
            
            neighborIter++;
         }
         
         // ...then turn the current [local] source black & remove it
         // from consideration.
         
         color[current] = BLACK;
         Q.pop();
         
      } // end while( !(Q.empty()) )
      
      node_iter++;

   } // end while ( iterating over all nodes - outer loop )
   
   return maxDistance;
}


// -----------------------------
// Graph manipulation functions.
// -----------------------------

// Add a node & return its ID, or -1 if unsuccessful.
// [Increment nextNodeIndex.]

int multiGraph::addNode() {
   
   if (nodeSize() < INT_MAX) {

      multiNode* newNode = new multiNode( nextNodeIndex );
      nodes[nextNodeIndex] = newNode;
      nextNodeIndex++;

      // Add our new node to the various tracking lists.

      degreeHash[0].insert(newNode->getId());

      return newNode->getId();

   } else {

      return -1;
   }
}

// Delete the node with the given index.
// Return the ID of the node if successful, -1 if not.

int multiGraph::deleteNode( int targetNode ) {

   int return_val = -1;

   node_hash_t::iterator iter;

   // Convert from an ID to an iterator.

   iter = nodes.find( targetNode );

   // Make sure this node actually exists, then call deleteNode(node*).

   if (iter != nodes.end()) {

      return_val = deleteNode( (iter->second) );
   }

   return return_val;
}

// Delete the specified node.
// Return the ID of the node if successful, -1 if not.
// Note: order of actions is CRITICAL in this function.

int multiGraph::deleteNode( multiNode* targetNode ) {

   int return_val;

   return_val = targetNode->getId();

   // Remove all the incident edges from the edge collection and from memory.

   deleteAllIncidentEdges( targetNode->getId() );

   // Remove the node's ID from the tracking lists.

   int targetDegree = targetNode->getDegree();

   degreeHash[targetDegree].erase(targetNode->getId());

   if ( degreeHash[targetDegree].empty() ) {
      degreeHash.erase(targetDegree);
   }

   // Remove it from the graph's node collection

   nodes.erase( targetNode->getId() );

   return return_val;
}

// Add an edge between two named vertices & return its ID, or -1
// if impossible.

int multiGraph::addEdge( int node_1 , int node_2 ) {

   node_hash_t::iterator iter_1, iter_2;

   // Convert from IDs to iterators.

   iter_1 = nodes.find( node_1 );
   iter_2 = nodes.find( node_2 );

   // Make sure the nodes actually exist, then call addEdge(multiNode*, multiNode*).

   if ( (iter_1 != nodes.end()) && (iter_2 != nodes.end()) ) {

      return addEdge( (iter_1->second), (iter_2->second) );

   } else {

      return -1;
   }
}

int multiGraph::addEdge( multiNode* node_1, multiNode* node_2 ) {

   // Create a new edge object, and hook everything up accordingly.

   multiEdge* newEdge = new multiEdge( nextEdgeIndex, node_1->getId(), node_2->getId() );

   edges[nextEdgeIndex] = newEdge;

   node_1->addEdge( newEdge->getId() );

   if ( node_1->getId() != node_2->getId() ) {
      node_2->addEdge( newEdge->getId() );
   }

   node_1->addNeighbor( node_2->getId() );

   if ( node_1->getId() != node_2->getId() ) {
      node_2->addNeighbor( node_1->getId() );
   }

   nextEdgeIndex++;

   // Upadate the degree hash list to reflect the increase in degree for
   // both of the nodes.

   if ( node_1->getId() != node_2->getId() ) {

      int oldDegree1 = node_1->getDegree() - 1;
      int oldDegree2 = node_2->getDegree() - 1;

      degreeHash[oldDegree1].erase(node_1->getId());

      if ( degreeHash[oldDegree1].empty() ) {
         degreeHash.erase(oldDegree1);
      }

      degreeHash[oldDegree1 + 1].insert(node_1->getId());

      degreeHash[oldDegree2].erase(node_2->getId());

      if ( degreeHash[oldDegree2].empty() ) {
         degreeHash.erase(oldDegree2);
      }

      degreeHash[oldDegree2 + 1].insert(node_2->getId());

   } else {

      // This is a self-loop.

      int oldDegree = node_1->getDegree() - 2;

      degreeHash[oldDegree].erase(node_1->getId());

      if ( degreeHash[oldDegree].empty() ) {
         degreeHash.erase(oldDegree);
      }

      degreeHash[oldDegree + 2].insert(node_1->getId());
   }
   
   return newEdge->getId();

}

// Delete _some_ edge between two specified nodes.

void multiGraph::deleteEdge( int node_1, int node_2 ) {

   node_hash_t::iterator iter_1, iter_2;

   // Convert from IDs to iterators...

   iter_1 = nodes.find( node_1 );
   iter_2 = nodes.find( node_2 );

   // Make sure the nodes actually exist, then call deleteEdge(node*, node*).

   if ( (iter_1 != nodes.end()) && (iter_2 != nodes.end()) ) {
      
      deleteEdge( (iter_1->second), (iter_2->second) );
   }

}

// Delete _some_ edge between two specified nodes.

void multiGraph::deleteEdge( multiNode* node_1, multiNode* node_2 ) {

   // If the nodes are actually adjacent,

   if ( node_1->hasNeighbor(node_2->getId()) ) {

      // If this is a loop we're zapping, fix the node's degreeOffset.

      if ( node_1->getId() == node_2->getId() ) {
         node_1->decrementDegreeOffset();
      }

      // Find the ID of _some_ connecting edge.

      bool found = false;
      int edgeID = -1;
      int secondEdgeID = -1;

      vector<int> node1Edges;
      node_1->getEdges(node1Edges);

      vector<int>::iterator iter = node1Edges.begin();

      while ( (iter != node1Edges.end()) && !(found) ) {
         
         // If we're deleting a loop, we want to make sure we remove a loop and not some other edge.
         
         if ( node_1->getId() == node_2->getId() ) {
            
            if ( edges[*iter]->getNode1() == edges[*iter]->getNode2() ) {
               
               // We've got a loop.  Tag it.
   
               found = true;
               edgeID = *iter;
            }
            
         } else {
            
            // Look for any edge in node_1's list which is adjacent to node_2.
            
            if ( (edges[*iter]->getNode1() == node_2->getId()) || (edges[*iter]->getNode2() == node_2->getId()) ) {
   
               found = true;
               edgeID = *iter;
            }
         }
         iter++;
      }

      // Remove the edge from the edges collection.

      edges.erase(edgeID);

      // Find out if node_1 and node_2 are still friends.

      iter = node1Edges.begin();

      while ( (iter != node1Edges.end()) && !(found) ) {

         // (We don't want to be querying a deleted edge.)

         if ( *iter != edgeID ) {
            
            // If the deleted edge was a loop, we want to check for other loops.
            
            if ( node_1->getId() == node_2->getId() ) {
               
               if ( (edges[*iter]->getNode1() == node_2->getId()) && (edges[*iter]->getNode2() == node_2->getId()) ) {
                  
                  found = true;
                  secondEdgeID = *iter;
               }
               
            } else {
               
               // Not a loop: look for any edge adjacent to node_1 whose other endpoint is node_2.
               
               if ( (edges[*iter]->getNode1() == node_2->getId()) || (edges[*iter]->getNode2() == node_2->getId()) ) {
                  
                  found = true;
                  secondEdgeID = *iter;
               }
            }
         }

         iter++;
      }

      // If they're no longer friends,

      if ( secondEdgeID == -1 ) {

         // Let the nodes know this disastrous rupture in their friendship
         // has taken place.

         node_1->removeNeighbor(node_2->getId());
         node_2->removeNeighbor(node_1->getId());
      }
      
      // If this isn't a loop,

      if ( node_1->getId() != node_2->getId() ) {
	 
	      // Kill the originally-zapped edge in the nodes' adjacency lists.

         node_1->removeEdge(edgeID);
      	node_2->removeEdge(edgeID);

         // Update the degree hash list to reflect the decrease in degree
         // for both of the nodes.

         int oldDegree1 = node_1->getDegree() + 1;
         int oldDegree2 = node_2->getDegree() + 1;

	      degreeHash[oldDegree1].erase(node_1->getId());
         
      	if ( degreeHash[oldDegree1].empty() ) {
      	   degreeHash.erase(oldDegree1);
      	}
   
      	degreeHash[oldDegree2].erase(node_2->getId());
         
      	if ( degreeHash[oldDegree2].empty() ) {
      	   degreeHash.erase(oldDegree2);
      	}
         
      	degreeHash[oldDegree1 - 1].insert(node_1->getId());
      	degreeHash[oldDegree2 - 1].insert(node_2->getId());
         
      } else { // [i.e., if the nodes are the same and this is a loop]
	      
	      // Kill the originally-zapped edge in the node's adjacency list.

         node_1->removeEdge(edgeID);

         // Update the degree hash list to reflect the decrease in degree.

         int oldDegree = node_1->getDegree() + 2;
   
      	degreeHash[oldDegree].erase(node_1->getId());
         
      	if ( degreeHash[oldDegree].empty() ) {
      	   degreeHash.erase(oldDegree);
      	}
         
      	degreeHash[oldDegree - 2].insert(node_1->getId());
         
      } // end if/else check for loop-ness
      
   } // end if ( nodes are actually adjacent )
}

// Delete all edges incident to a given node.

void multiGraph::deleteAllIncidentEdges( int targetNode ) {

   node_hash_t::iterator nodeIter = nodes.find(targetNode);

   // If the node actually exists...

   if ( nodeIter != nodes.end() ) {

      multiNode* targetNodePtr = nodeIter->second;

      // Get all of the node's incident edges

      vector<int> targetEdges;
      targetNodePtr->getEdges( targetEdges );

      // Delete them one by one.

      int edgeID;

      vector<int>::iterator iter;

      for( iter = targetEdges.begin(); iter != targetEdges.end(); iter++ ) {

         edgeID = *iter;

         deleteEdge( edgeID );
      }

   } // end if (node exists)
}

// Delete a specified edge.

void multiGraph::deleteEdge( int edgeID ) {

      // Identify our ill-fated edge's endpoints.

      multiNode* node_1 = nodes[edges[edgeID]->getNode1()];
      multiNode* node_2 = nodes[edges[edgeID]->getNode2()];

      // If this is a loop we're zapping, fix the node's degreeOffset.

      if ( node_1->getId() == node_2->getId() ) {
         node_1->decrementDegreeOffset();
      }

      // Remove the edge from the edges collection.

      edges.erase(edgeID);

      // Find out if node_1 and node_2 are still friends.

      vector<int> node1Edges;
      node_1->getEdges(node1Edges);

      vector<int>::iterator iter = node1Edges.begin();

      int secondEdgeID = -1;
      bool found = false;

      while ( (iter != node1Edges.end()) && !(found) ) {

         // (We don't want to be querying a deleted edge.)

         if ( *iter != edgeID ) {

            if ( (edges[*iter]->getNode1() == node_2->getId())
                  || (edges[*iter]->getNode2() == node_2->getId()) ) {

               found = true;
               secondEdgeID = *iter;
            }
         }

         iter++;
      }

      // If they're no longer friends,

      if ( secondEdgeID == -1 ) {

         // Let the nodes know this disastrous rupture in their friendship
         // has taken place.

         node_1->removeNeighbor(node_2->getId());
         node_2->removeNeighbor(node_1->getId());
      }

      // Kill the originally-zapped edge in the nodes' adjacency lists.

      node_1->removeEdge(edgeID);
      node_2->removeEdge(edgeID);

      // If this is not a loop,
      
      if ( node_1->getId() != node_2->getId() ) {
      
         // Update the degree hash list to reflect the decrease in degree
         // for both of the nodes.

         int oldDegree1 = node_1->getDegree() + 1;
         int oldDegree2 = node_2->getDegree() + 1;
   
         degreeHash[oldDegree1].erase(node_1->getId());

         if ( degreeHash[oldDegree1].empty() ) {
            degreeHash.erase(oldDegree1);
         }

         degreeHash[oldDegree2].erase(node_2->getId());

         if ( degreeHash[oldDegree2].empty() ) {
            degreeHash.erase(oldDegree2);
         }

         degreeHash[oldDegree1 - 1].insert(node_1->getId());
         degreeHash[oldDegree2 - 1].insert(node_2->getId());
      
      } else { // [i.e., if this _is_ a loop]
      
         // Update the degree hash list to reflect the decrease in degree.

         int oldDegree = node_1->getDegree() + 2;
   
         degreeHash[oldDegree].erase(node_1->getId());

         if ( degreeHash[oldDegree].empty() ) {
            degreeHash.erase(oldDegree);
         }

         degreeHash[oldDegree - 2].insert(node_1->getId());

      } // end if ( this is a loop)
}
