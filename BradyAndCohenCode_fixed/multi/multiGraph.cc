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
      
      // Update the color & weight hashes.
      
      colorHash[nodes[i]->getColor()].insert(i);
      weightHash[nodes[i]->getWeight()].insert(i);
      
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
   
   currentTime = source.currentTime;
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

// Set node color and update the tracking hash accordingly.

void multiGraph::setNodeColor( int nodeID, int colorArg ) {

   node_hash_t::iterator iter = nodes.find(nodeID);

   if ( iter != nodes.end() ) {

      // Got the node; update the colorHash first.

      int oldColor = iter->second->getColor();

      colorHash[oldColor].erase(nodeID);
      colorHash[colorArg].insert(nodeID);

      if ( colorHash[oldColor].size() == 0 ) {
         colorHash.erase(oldColor);
      }

      // Now we can safely set the new color.

      iter->second->setColor(colorArg);
   }
}

// Set node weight and update the tracking hash accordingly.

void multiGraph::setNodeWeight( int nodeID, double weightArg ) {

   node_hash_t::iterator iter = nodes.find(nodeID);

   if ( iter != nodes.end() ) {

      // Got the node; update the weightHash first.

      double oldWeight = iter->second->getWeight();

      weightHash[oldWeight].erase(nodeID);
      weightHash[weightArg].insert(nodeID);

      if ( weightHash[oldWeight].size() == 0 ) {
         weightHash.erase(oldWeight);
      }

      // Now we can safely set the new weight.

      iter->second->setWeight(weightArg);
   }
}


// -----------------------------
// Graph manipulation functions.
// -----------------------------

// Add a node & return its ID, or -1 if unsuccessful.
// [Increment nextNodeIndex.]

int multiGraph::addNode() {
   
   if (nodeSize() < INT_MAX) {

      multiNode* newNode = new multiNode( nextNodeIndex, currentTime );
      nodes[nextNodeIndex] = newNode;
      nextNodeIndex++;

      // Add our new node to the various tracking lists.

      degreeHash[0].insert(newNode->getId());
      colorHash[0].insert(newNode->getId());
      weightHash[0].insert(newNode->getId());

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

   int targetColor = targetNode->getColor();

   colorHash[targetColor].erase(targetNode->getId());

   if ( colorHash[targetColor].empty() ) {
      colorHash.erase(targetColor);
   }

   double targetWeight = targetNode->getWeight();

   weightHash[targetWeight].erase(targetNode->getId());

   if ( weightHash[targetWeight].empty() ) {
      weightHash.erase(targetWeight);
   }

   // Remove it from the graph's node collection

   nodes.erase( targetNode->getId() );

   return return_val;
}

// Delete a random node.

int multiGraph::deleteRandomNode() {

   int nodeID = randomNode( '*', 0 );

   return deleteNode( nodeID );
}

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

int multiGraph::deleteRandomNode( char attribute, double value ) {

   int nodeID = randomNode( attribute, value );

   return deleteNode( nodeID );
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

   multiEdge* newEdge = new multiEdge( nextEdgeIndex, currentTime,
         node_1->getId(), node_2->getId() );

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

// Add a random edge.

int multiGraph::addRandomEdge() {

   // Randomly find one endpoint.

   int node_1 = randomNode( '*', 0 );

   if (node_1 != -1) {

      // Randomly find the other.

      return addRandomEdge( node_1 );

   } else {

      return -1;

   } // end if ( we found a candidate first-node )

}

 // Add a random edge to a given node.

int multiGraph::addRandomEdge( int node_1 ) {

   node_hash_t::iterator iter;

   // Convert from node ID to iterator.

   iter = nodes.find( node_1 );

   // Make sure the node actually exists.  If it does, call
   // addRandomEdge(node*).  If not, just return - you guessed it -
   // ... wait for it ...
   // ...
   //
   // MINUS ONE!

   if ( iter != nodes.end() ) {

      return addRandomEdge( (iter->second) );

   } else {

      return -1;

   } // end if ( the argument is actually a valid node ID )

}

// Add a random edge to a given node.

int multiGraph::addRandomEdge( multiNode* node_1 )
{
   // Any old hookup will do.  Our multiNodes aren't choosy.

   int node2ID = randomNode('*', 0);

   // If we found a friend,

   if ( node2ID != -1 ) {

      // Hook them up.

      return addEdge(node_1->getId(), node2ID);

   } else {

      // Otherwise, alas, we must die.

      return -1;

   } // end if ( we found a friend for node_1 )

}

// Add a random edge incident to a node where some attribute matches
// some value.
//
// Returns the edge's ID if successful, -1 if it proved impossible.

int multiGraph::addRandomEdge( char attribute, double value ) {

   int ret_val = -1;
   int connectTo = randomNode(attribute, value);

   if ( connectTo != -1 ) {
      ret_val = addRandomEdge(connectTo);
   }

   return ret_val;
}

// Add a random edge incident to a given node where some attribute matches
// some value.
//
// Returns the edge's ID if successful, -1 if it proved impossible.

int multiGraph::addRandomEdge( int node_1, char attribute, double value ) {

   int ret_val = -1;
   int connectTo = randomNode(attribute, value);

   if ( connectTo != -1 ) {
      ret_val = addEdge(node_1, connectTo);
   }

   return ret_val;
}

// Add a random edge incident to a given node where some attribute matches
// some value.
//
// Returns the edge's ID if successful, -1 if it proved impossible.

int multiGraph::addRandomEdge( multiNode* node_1, char attribute, double value ) {

   int ret_val = -1;
   int connectTo = randomNode(attribute, value);

   if ( connectTo != -1 ) {
      ret_val = addEdge(node_1->getId(), connectTo);
   }

   return ret_val;
}

// Add a random edge _between_ two random vertices with specified attribute
// constraints, as above.
//
// Returns the edge's ID if successful, -1 if it proved impossible.

int multiGraph::addRandomEdge( char attribute_1, double value_1,
                           char attribute_2, double value_2 ) {

   int connectFrom = randomNode(attribute_1, value_1);
   int connectTo = randomNode(attribute_2, value_2);

   return addEdge(connectFrom, connectTo);
}

// Add a random edge _between_ two random vertices with specified attribute
// constraints, as above.  Disallow loops and multiedges.
//
// Returns the edge's ID if successful, -1 if it proved impossible.

int multiGraph::addRandomNewEdge( char attribute_1, double value_1,
                           char attribute_2, double value_2 ) {

   int connectFrom = randomNode(attribute_1, value_1);
   int connectTo = randomNonNeighbor(attribute_2, value_2, connectFrom);

   int return_val = addEdge(connectFrom, connectTo);

   return return_val;
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

// Delete a random edge and return its endpoint IDs.

vector<int> multiGraph::deleteRandomEdge() {

   vector<int> return_vector;

   // If there's anything to delete,

   if ( !edges.empty() ) {

      // Iterate through the edges randomNum times until we
      // get to an edge to delete.

      int randomNum = abs((int) (randomNumber() % edgeSize()));

      edge_hash_t::iterator iter = edges.begin();

      int i = 0;

      while (i < randomNum) {

         iter++;
         i++;
      }

      multiEdge* ptr = (iter->second);

      return_vector.push_back(ptr->getNode1());
      return_vector.push_back(ptr->getNode2());

      deleteEdge( ptr->getId() );

   } // end if (there's anything to delete)

   return return_vector;
}

// Delete a random edge incident to a specified node.

vector<int> multiGraph::deleteRandomEdge( int targetNode ) {

   vector<int> return_vector;

   if ( !edges.empty() ) {

      node_hash_t::iterator iter;

      // Convert from ID to iterator.

      iter = nodes.find( targetNode );

      // Make sure the node actually exists  If it does, pass it
      // along to deleteRandomEdge(multiNode*).

      if ( iter != nodes.end() ) {
         return_vector = deleteRandomEdge( (iter->second) );
      }
   }

   return return_vector;
}

// Delete a random edge incident to a specified node.

vector<int> multiGraph::deleteRandomEdge( multiNode* targetNode ) {

   vector<int> return_vector;

   // Make sure there is something we can delete.

   if (!edges.empty() && targetNode->getDegree() > 0) {

      // Load a vector with this node's incident edges.

      vector<int> incidentEdges;
      targetNode->getEdges( incidentEdges );

      // Pick an index at random.

      int randomNum;

      randomNum = abs((int) (randomNumber() % incidentEdges.size()));

      // Delete the indexed edge.

      int edgeID = incidentEdges[randomNum];

      return_vector.push_back(edges[edgeID]->getNode1());
      return_vector.push_back(edges[edgeID]->getNode2());

      deleteEdge( edgeID );
   }

   return return_vector;
}

// Delete a random edge incident to a node matching specified attributes.

vector<int> multiGraph::deleteRandomEdge( char attribute, double value ) {

   vector<int> return_vector;

   int targetID = randomConnectedNode(attribute, value);

   if ( targetID == -1 ) {

      return_vector.push_back(-1);
      return_vector.push_back(-1);

   } else {

      return_vector = deleteRandomEdge(targetID);
   }

   return return_vector;
}


// Delete a random edge between two nodes matching specified attributes.
//
// *****************************
// DEPRECATED FOR THE TIME BEING
// *****************************

vector<int> multiGraph::deleteRandomEdge( char attribute_1, double value_1,
                                     char attribute_2, double value_2) {

   vector<int> return_vector;

   return_vector.push_back(-1);
   return_vector.push_back(-1);

   return return_vector;
}

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

int multiGraph::randomNode( char attribute, double value ) {

   int ret_val = -1;
   int i;
   int randomPosition;

   switch ( attribute ) {

      case '*': {

         // No attribute in particular; just return any random node.

         if ( !(nodes.empty()) ) {

            randomPosition = randomNumber() % nodeSize();

            node_hash_t::iterator generalIter = nodes.begin();

            for ( i = 0; i < randomPosition; i++ ) {
               generalIter++;
            }

            ret_val = generalIter->second->getId();
         }

         break;

      } // end case [general]

      case 'c': {

         // Color.

         if ( colorHash.find((int) value) != colorHash.end() ) {

            randomPosition = randomNumber() % colorHash[(int) value].size();

            int_hash_set::iterator colorIter = colorHash[(int) value].begin();

            for ( i = 0; i < randomPosition; i++ ) {
               colorIter++;
            }

            ret_val = *colorIter;
         }

         break;

      } // end case [color]

      case 'w': {

         // Weight.

         if ( weightHash.find(value) != weightHash.end() ) {

            randomPosition = randomNumber() % weightHash[value].size();

            int_hash_set::iterator weightIter = weightHash[value].begin();

            for ( i = 0; i < randomPosition; i++ ) {
               weightIter++;
            }

            ret_val = *weightIter;
         }

         break;

      } // end case [weight]

      case 'd': {

         // Degree.

         if ( degreeHash.find((int) value) != degreeHash.end() ) {

            randomPosition = randomNumber() % degreeHash[(int) value].size();

            int_hash_set::iterator degreeIter = degreeHash[(int) value].begin();

            for ( i = 0; i < randomPosition; i++ ) {
               degreeIter++;
            }

            ret_val = *degreeIter;
         }

         break;

      } // end case [degree]

      case 'H': {

         // Node's weight >= (value).

         if ( weightHash.size() != 0 ) {

            // First, locate all valid values, if any.

            set<double> valid_values;
            doubleKey_ID_map_t::iterator iter = weightHash.begin();

            while ( iter != weightHash.end() ) {

               if ( isGreaterEqual( iter->first, value ) ) {
                  valid_values.insert(iter->first);
               }

               iter++;
            }

            // If there _are_ any valid values,

            if ( valid_values.size() > 0 ) {

               // First, count how many nodes exist in our selected subset,
               // which matches the given criterion.

               int valid_count = 0;

               set<double>::iterator set_iter = valid_values.begin();

               while ( set_iter != valid_values.end() ) {

                  valid_count += weightHash[*set_iter].size();
                  set_iter++;
               }

               // Next, find an 'absolute' position across all valid nodes.

               randomPosition = randomNumber() % valid_count;

               // Lastly, locate the actual node to which this position
               // actually refers, and return its ID.

               set_iter = valid_values.begin();

               while ( randomPosition >= weightHash[*set_iter].size() ) {
                  randomPosition -= weightHash[*set_iter].size();
                  set_iter++;
               }

               int_hash_set::iterator target_iter
                                                = weightHash[*set_iter].begin();

               while ( randomPosition > 0 ) {
                  target_iter++;
                  randomPosition--;
               }

               ret_val = *target_iter;

            } // end if ( there are any valid values )

         } // end if ( there are any weights at all )

         break;

      } // end case [weight >= given value]

      case 'h': {

         // Node's weight <= (value).

         if ( weightHash.size() != 0 ) {

            // First, locate all valid values, if any.

            set<double> valid_values;
            doubleKey_ID_map_t::iterator iter = weightHash.begin();

            while ( iter != weightHash.end() ) {

               if ( isLessEqual( iter->first, value ) ) {
                  valid_values.insert(iter->first);
               }

               iter++;
            }

            // If there _are_ any valid values,

            if ( valid_values.size() > 0 ) {

               // First, count how many nodes exist in our selected subset,
               // which matches the given criterion.

               int valid_count = 0;

               set<double>::iterator set_iter = valid_values.begin();

               while ( set_iter != valid_values.end() ) {

                  valid_count += weightHash[*set_iter].size();
                  set_iter++;
               }

               // Next, find an 'absolute' position across all valid nodes.

               randomPosition = randomNumber() % valid_count;

               // Lastly, locate the actual node to which this position
               // actually refers, and return its ID.

               set_iter = valid_values.begin();

               while ( randomPosition >= weightHash[*set_iter].size() ) {
                  randomPosition -= weightHash[*set_iter].size();
                  set_iter++;
               }

               int_hash_set::iterator target_iter
                                                = weightHash[*set_iter].begin();

               while ( randomPosition > 0 ) {
                  target_iter++;
                  randomPosition--;
               }

               ret_val = *target_iter;

            } // end if ( there are any valid values )

         } // end if ( there are any weights at all )

         break;

      } // end case [weight <= given value]

      case 'G': {

         // Node's degree >= (value).

         if ( nodes.size() != 0 ) {

            // First, locate all valid values, if any.

            set<int> valid_values;
            intKey_ID_map_t::iterator iter = degreeHash.begin();

            while ( iter != degreeHash.end() ) {

               if ( isGreaterEqual( iter->first, value ) ) {
                  valid_values.insert(iter->first);
               }

               iter++;
            }

            // If there _are_ any valid values,

            if ( valid_values.size() > 0 ) {

               // First, count how many nodes exist in our selected subset,
               // which matches the given criterion.

               int valid_count = 0;

               set<int>::iterator set_iter = valid_values.begin();

               while ( set_iter != valid_values.end() ) {

                  valid_count += degreeHash[*set_iter].size();
                  set_iter++;
               }

               // Next, find an 'absolute' position across all valid nodes.

               randomPosition = randomNumber() % valid_count;

               // Lastly, locate the actual node to which this position
               // actually refers, and return its ID.

               set_iter = valid_values.begin();

               while ( randomPosition >= degreeHash[*set_iter].size() ) {
                  randomPosition -= degreeHash[*set_iter].size();
                  set_iter++;
               }

               int_hash_set::iterator target_iter
                                                = degreeHash[*set_iter].begin();

               while ( randomPosition > 0 ) {
                  target_iter++;
                  randomPosition--;
               }

               ret_val = *target_iter;

            } // end if ( there are any valid values )

         } // end if ( there are any nodes at all )

         break;

      } // end case [degree >= given value]

      case 'g': {

         // Node's degree <= (value).

         if ( nodes.size() != 0 ) {

            // First, locate all valid values, if any.

            set<int> valid_values;
            intKey_ID_map_t::iterator iter = degreeHash.begin();

            while ( iter != degreeHash.end() ) {

               if ( isLessEqual( iter->first, value ) ) {
                  valid_values.insert(iter->first);
               }

               iter++;
            }

            // If there _are_ any valid values,

            if ( valid_values.size() > 0 ) {

               // First, count how many nodes exist in our selected subset,
               // which matches the given criterion.

               int valid_count = 0;

               set<int>::iterator set_iter = valid_values.begin();

               while ( set_iter != valid_values.end() ) {

                  valid_count += degreeHash[*set_iter].size();
                  set_iter++;
               }

               // Next, find an 'absolute' position across all valid nodes.

               randomPosition = randomNumber() % valid_count;

               // Lastly, locate the actual node to which this position
               // actually refers, and return its ID.

               set_iter = valid_values.begin();

               while ( randomPosition >= degreeHash[*set_iter].size() ) {
                  randomPosition -= degreeHash[*set_iter].size();
                  set_iter++;
               }

               int_hash_set::iterator target_iter
                                                = degreeHash[*set_iter].begin();

               while ( randomPosition > 0 ) {
                  target_iter++;
                  randomPosition--;
               }

               ret_val = *target_iter;

            } // end if ( there are any valid values )

         } // end if ( there are any nodes at all )

         break;

      } // end case [degree <= given value]

   } // end switch (attribute)

   return ret_val;
}

// Return a random node's ID where 'attribute' matches 'value,' or -1
// if there isn't one.  Ensure that the returned node is not a neighbor
// of the node with the given ID.
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

int multiGraph::randomNonNeighbor( char attribute, double value, int nodeID ) {

   int ret_val = -1;
   int i, count;
   int randomPosition;
   bool fell_off_end, done;

   switch ( attribute ) {

      case '*': {

         // No attribute in particular; just return any random open node.

         if ( !(nodes.empty()) ) {

            // Pick a random position.
            randomPosition = randomNumber() % nodeSize();

            node_hash_t::iterator generalIter = nodes.begin();

            // Advance to the random position.

            for ( i = 0; i < randomPosition; i++ ) {
               generalIter++;
            }

            if (
                  (generalIter->first != nodeID)
               &&
                  !(generalIter->second->hasNeighbor(nodeID))
               ) {

               // If we hit a good node on the first try, no problem.

               ret_val = generalIter->second->getId();

            } else {

               // Otherwise...

               done = false;
               fell_off_end = false;

               count = 0;


               // ...cycle around the nodes which match the desired
               // value.  If we hit a good one, stop.  If we cycle all the way
               // around, set the 'done' flag and stop.

               while ( !(done)
                     &&
                        (
                           (generalIter->first == nodeID)
                        ||
                           (generalIter->second->hasNeighbor(nodeID))
                        )
                     ) {

                  if ( fell_off_end && count < randomPosition ) {

                     // If we've cycled around the end of the node list,
                     // but we haven't yet advanced to our original position,
                     // keep going, and keep track of how far we've advanced.

                     count++;

                  } else if ( fell_off_end ) {

                     // If we've cycled around the end of the node list,
                     // and we DID come back to our original position, set
                     // the 'done' flag and give up - there are no valid nodes.

                     done = true;
                  }

                  if ( !done ) {

                     generalIter++;

                     // If we've just fallen off the end of the list,

                     if ( generalIter == nodes.end() ) {

                        // Start over at the beginning.

                        fell_off_end = true;
                        generalIter = nodes.begin();
                     }
                  }

               } // end while ( we're still looking for One Good Node )

               if ( done ) {

                  // We wrapped all the way around and didn't find anything.
                  // Give up.

                  ret_val = -1;

               } else {

                  // We didn't go all the way around, but the while-loop
                  // terminated - meaning we found something, and generalIter
                  // is currently sitting on it.  Return its ID.

                  ret_val = generalIter->second->getId();
               }

            } // end if ( we hit on the first try )

         } // end if ( we have any nodes )

         break;

      } // end case [general]

      case 'c': {

         // Color.

         if ( colorHash.find((int) value) != colorHash.end() ) {

            // Pick a random position.
            randomPosition = randomNumber() % colorHash[(int) value].size();

            int_hash_set::iterator generalIter = colorHash[(int) value].begin();

            // Advance to the random position.

            for ( i = 0; i < randomPosition; i++ ) {
               generalIter++;
            }

            if (
                  (*generalIter != nodeID)
               &&
                  !(nodes[*generalIter]->hasNeighbor(nodeID))
               ) {

               // If we hit a good node on the first try, no problem.

               ret_val = *generalIter;

            } else {

               // Otherwise...

               done = false;
               fell_off_end = false;

               count = 0;

               // ...cycle around the nodes which match the desired
               // value.  If we hit a good one, stop.  If we cycle all the way
               // around, set the 'done' flag and stop.

               while ( !(done)
                     &&
                        (
                           (*generalIter == nodeID)
                        ||
                           (nodes[*generalIter]->hasNeighbor(nodeID))
                        )
                     ) {

                  if ( fell_off_end && count < randomPosition ) {

                     // If we've cycled around the end of the node list,
                     // but we haven't yet advanced to our original position,
                     // keep going, and keep track of how far we've advanced.

                     count++;

                  } else if ( fell_off_end ) {

                     // If we've cycled around the end of the node list,
                     // and we DID come back to our original position, set
                     // the 'done' flag and give up - there are no valid nodes.

                     done = true;
                  }

                  if ( !done ) {

                     generalIter++;

                     // If we've just fallen off the end of the list,

                     if ( generalIter == colorHash[(int) value].end() ) {

                        // Start over at the beginning.

                        fell_off_end = true;
                        generalIter = colorHash[(int) value].begin();
                     }
                  }

               } // end while ( we're still looking for One Good Node )

               if ( done ) {

                  // We wrapped all the way around and didn't find anything.
                  // Give up.

                  ret_val = -1;

               } else {

                  // We didn't go all the way around, but the while-loop
                  // terminated - meaning we found something, and generalIter
                  // is currently sitting on it.  Return its ID.

                  ret_val = *generalIter;
               }

            } // end if ( we hit on the first try )

         } // end if ( we have any nodes )

         break;

      } // end case [color]

      case 'w': {

         // Weight.

         if ( weightHash.find(value) != weightHash.end() ) {

            // Pick a random position.
            randomPosition = randomNumber()
                                    % weightHash[value].size();

            int_hash_set::iterator generalIter =
                                       weightHash[value].begin();

            // Advance to the random position.

            for ( i = 0; i < randomPosition; i++ ) {
               generalIter++;
            }

            if (
                  (*generalIter != nodeID)
               &&
                  !(nodes[*generalIter]->hasNeighbor(nodeID))
               ) {

               // If we hit a good node on the first try, no problem.

               ret_val = *generalIter;

            } else {

               // Otherwise...

               done = false;
               fell_off_end = false;

               count = 0;

               // ...cycle around the nodes which match the desired
               // value.  If we hit a good one, stop.  If we cycle all the way
               // around, set the 'done' flag and stop.

               while ( !(done)
                     &&
                        (
                           (*generalIter == nodeID)
                        ||
                           (nodes[*generalIter]->hasNeighbor(nodeID))
                        )
                     ) {

                  if ( fell_off_end && count < randomPosition ) {

                     // If we've cycled around the end of the node list,
                     // but we haven't yet advanced to our original position,
                     // keep going, and keep track of how far we've advanced.

                     count++;

                  } else if ( fell_off_end ) {

                     // If we've cycled around the end of the node list,
                     // and we DID come back to our original position, set
                     // the 'done' flag and give up - there are no valid nodes.

                     done = true;
                  }

                  if ( !done ) {

                     generalIter++;

                     // If we've just fallen off the end of the list,

                     if ( generalIter == weightHash[value].end() ) {

                        // Start over at the beginning.

                        fell_off_end = true;
                        generalIter = weightHash[value].begin();
                     }
                  }

               } // end while ( we're still looking for One Good Node )

               if ( done ) {

                  // We wrapped all the way around and didn't find anything.
                  // Give up.

                  ret_val = -1;

               } else {

                  // We didn't go all the way around, but the while-loop
                  // terminated - meaning we found something, and generalIter
                  // is currently sitting on it.  Return its ID.

                  ret_val = *generalIter;
               }

            } // end if ( we hit on the first try )

         } // end if ( we have any nodes )

         break;

      } // end case [weight]

      case 'd': {

         // Degree.

         if (
               (degreeHash.find((int) value) != degreeHash.end())
            &&
               ((int) value != nodeSize() - 1 )
            ) {

            // Get all nodes [if any] matching the desired degree.

            vector<int> idVector;

            int_hash_set::iterator set_iter = degreeHash[(int) value].begin();

            while ( set_iter != degreeHash[(int) value].end() ) {

               // Scan the target degree's int_hash_set for good nodes.

               if (
                     (*set_iter != nodeID)
                  &&
                     !(nodes[*set_iter]->hasNeighbor(nodeID))
                  ) {

                  idVector.push_back(*set_iter);
               }

               set_iter++;

            } // end while ( we're scanning the target degree for good nodes )

            // If there are any valid results...

            if ( idVector.size() > 0 ) {

               // ...pick a random index in our results-vector and return its
               // corresponding value.

               randomPosition = randomNumber() % idVector.size();

               ret_val = idVector[randomPosition];

            } // end if ( there were any valid results )

         } // end if ( the requested degree exists and isn't too big )

         break;

      } // end case [degree]

      case 'H': {

         // Node's weight >= (value).

         if ( weightHash.size() != 0 ) {

            // First, locate all valid values, if any.

            set<double> valid_values;
            doubleKey_ID_map_t::iterator iter = weightHash.begin();

            while ( iter != weightHash.end() ) {

               if ( isGreaterEqual( iter->first, value ) ) {
                  valid_values.insert(iter->first);
               }

               iter++;
            }

            // If there _are_ any valid values,

            if ( valid_values.size() > 0 ) {

               // We're looking for open nodes that aren't <nodeID> and aren't
               // neighbors thereof, so create a vector consisting of
               // all node IDs within the valid_values range which don't point
               // to nodes with degree (n-1) and aren't neighbors of nodeID.

               vector<int> idVector;

               set<double>::iterator set_iter = valid_values.begin();

               while ( set_iter != valid_values.end() ) {

                  // Scan each weight's int_hash_set for good nodes.

                  int_hash_set::iterator innerIter
                                                = weightHash[*set_iter].begin();

                  while ( innerIter != weightHash[*set_iter].end() ) {

                     if (
                           (*innerIter != nodeID)
                        &&
                           !(nodes[*innerIter]->hasNeighbor(nodeID))
                        ) {

                        idVector.push_back(*innerIter);
                     }

                     innerIter++;

                  } // end while (parse nodes matching a particular weight)

                  set_iter++;

               } // end while (parse valid weights)

               // If we have any matching nodes,

               if ( idVector.size() > 0 ) {

                  // Next, find an 'absolute' position across all valid nodes.

                  randomPosition = randomNumber() % idVector.size();

                  // Lastly, locate the actual node to which this position
                  // refers, and return its ID.

                  ret_val = idVector[randomPosition];

               } // end if ( we got any matching nodes )

            } // end if ( there are any valid weights )

         } // end if ( there are any weights at all )

         break;

      } // end case [weight >= given value]

      case 'h': {

         // Node's weight <= (value).

         if ( weightHash.size() != 0 ) {

            // First, locate all valid values, if any.

            set<double> valid_values;
            doubleKey_ID_map_t::iterator iter = weightHash.begin();

            while ( iter != weightHash.end() ) {

               if ( isLessEqual( iter->first, value ) ) {
                  valid_values.insert(iter->first);
               }

               iter++;
            }

            // If there _are_ any valid values,

            if ( valid_values.size() > 0 ) {

               // We're looking for open nodes, so create a vector consisting of
               // all node IDs within the valid_values range which don't point
               // to nodes with degree (n-1) and aren't neighbors of nodeID.

               vector<int> idVector;

               set<double>::iterator set_iter = valid_values.begin();

               while ( set_iter != valid_values.end() ) {

                  // Scan each weight's int_hash_set for good nodes.

                  int_hash_set::iterator innerIter
                                                = weightHash[*set_iter].begin();

                  while ( innerIter != weightHash[*set_iter].end() ) {

                     if (
                           (*innerIter != nodeID)
                        &&
                           !(nodes[*innerIter]->hasNeighbor(nodeID))
                        ) {

                        idVector.push_back(*innerIter);
                     }

                     innerIter++;

                  } // end while (parse nodes matching a particular weight)

                  set_iter++;

               } // end while (parse valid weights)

               // If we have any matching nodes,

               if ( idVector.size() > 0 ) {

                  // Next, find an 'absolute' position across all valid nodes.

                  randomPosition = randomNumber() % idVector.size();

                  // Lastly, locate the actual node to which this position
                  // refers, and return its ID.

                  ret_val = idVector[randomPosition];

               } // end if ( we got any matching nodes )

            } // end if ( there are any valid weights )

         } // end if ( there are any weights at all )

         break;

      } // end case [weight <= given value]

      case 'G': {

         // Node's degree >= (value).

         if ( nodes.size() != 0 ) {

            // First, locate all valid values, if any.

            set<int> valid_values;
            intKey_ID_map_t::iterator iter = degreeHash.begin();

            while ( iter != degreeHash.end() ) {

               if (
                     (isGreaterEqual(iter->first, value))
                  ) {
                  valid_values.insert(iter->first);
               }

               iter++;
            }

            // If there _are_ any valid values,

            if ( valid_values.size() > 0 ) {

               // We're looking for open nodes, so create a vector consisting of
               // all node IDs within the valid_values range which aren't
               // neighbors of nodeID.

               vector<int> idVector;

               set<int>::iterator set_iter = valid_values.begin();

               while ( set_iter != valid_values.end() ) {

                  // Scan each degree's int_hash_set for good nodes.

                  int_hash_set::iterator innerIter
                                               = degreeHash[*set_iter].begin();

                  while ( innerIter != degreeHash[*set_iter].end() ) {

                     if (
                           (*innerIter != nodeID)
                        &&
                           !(nodes[*innerIter]->hasNeighbor(nodeID))
                        ) {

                        idVector.push_back(*innerIter);
                     }

                     innerIter++;

                  } // end while (parse nodes matching a particular weight)

                  set_iter++;

               } // end while (parse valid weights)

               // If we have any matching nodes,

               if ( idVector.size() > 0 ) {

                  // Next, find an 'absolute' position across all valid nodes.

                  randomPosition = randomNumber() % idVector.size();

                  // Lastly, locate the actual node to which this position
                  // refers, and return its ID.

                  ret_val = idVector[randomPosition];

               } // end if ( we got any matching nodes )

            } // end if ( there are any valid values )

         } // end if ( there are any nodes at all )

         break;

      } // end case [degree >= given value]

      case 'g': {

         // Node's degree <= (value).

         if ( nodes.size() != 0 ) {

            // First, locate all valid values, if any.

            set<int> valid_values;
            intKey_ID_map_t::iterator iter = degreeHash.begin();

            while ( iter != degreeHash.end() ) {

               if (
                     (isLessEqual(iter->first, value))
                  ) {
                  valid_values.insert(iter->first);
               }

               iter++;
            }

            // If there _are_ any valid values,

            if ( valid_values.size() > 0 ) {

               // We're looking for open nodes, so create a vector consisting of
               // all node IDs within the valid_values range which aren't
               // neighbors of nodeID.

               vector<int> idVector;

               set<int>::iterator set_iter = valid_values.begin();

               while ( set_iter != valid_values.end() ) {

                  // Scan each degree's int_hash_set for good nodes.

                  int_hash_set::iterator innerIter
                                               = degreeHash[*set_iter].begin();

                  while ( innerIter != degreeHash[*set_iter].end() ) {

                     if (
                           (*innerIter != nodeID)
                        &&
                           !(nodes[*innerIter]->hasNeighbor(nodeID))
                        ) {

                        idVector.push_back(*innerIter);
                     }

                     innerIter++;

                  } // end while (parse nodes matching a particular weight)

                  set_iter++;

               } // end while (parse valid weights)

               // If we have any matching nodes,

               if ( idVector.size() > 0 ) {

                  // Next, find an 'absolute' position across all valid nodes.

                  randomPosition = randomNumber() % idVector.size();

                  // Lastly, locate the actual node to which this position
                  // refers, and return its ID.

                  ret_val = idVector[randomPosition];

               } // end if ( we got any matching nodes )

            } // end if ( there are any valid values )

         } // end if ( there are any nodes at all )

         break;

      } // end case [degree <= given value]

   } // end switch (attribute)

   return ret_val;
}

// Grab a random node which isn't of degree zero.

int multiGraph::randomConnectedNode( char attribute, double value ) {

   int ret_val = -1;
   int i, count;
   int randomPosition;
   bool fell_off_end, done;

   switch ( attribute ) {

      case '*': {

         // No attribute in particular; just return any random open node which
         // isn't of degree 0.

         if ( !(nodes.empty()) ) {

            // Pick a random position.
            randomPosition = randomNumber() % nodeSize();

            node_hash_t::iterator generalIter = nodes.begin();

            // Advance to the random position.

            for ( i = 0; i < randomPosition; i++ ) {
               generalIter++;
            }

            if ( generalIter->second->getDegree() > 0 ) {

               // If we hit a good node on the first try, no problem.

               ret_val = generalIter->second->getId();

            } else {

               // Otherwise...

               done = false;
               fell_off_end = false;

               count = 0;

               // ...cycle around the nodes which match the desired
               // value.  If we hit a good one, stop.  If we cycle all the way
               // around, set the 'done' flag and stop.

               while ( !(done)
                      && (generalIter->second->getDegree() == 0) ) {

                  if ( fell_off_end && count < randomPosition ) {

                     // If we've cycled around the end of the node list,
                     // but we haven't yet advanced to our original position,
                     // keep going, and keep track of how far we've advanced.

                     count++;

                  } else if ( fell_off_end ) {

                     // If we've cycled around the end of the node list,
                     // and we DID come back to our original position, set
                     // the 'done' flag and give up - there are no valid nodes.

                     done = true;
                  }

                  if ( !done ) {

                     generalIter++;

                     // If we've just fallen off the end of the list,

                     if ( generalIter == nodes.end() ) {

                        // Start over at the beginning.

                        fell_off_end = true;
                        generalIter = nodes.begin();
                     }
                  }

               } // end while ( we're still looking for One Good Node )

               if ( done ) {

                  // We wrapped all the way around and didn't find anything.
                  // Give up.

                  ret_val = -1;

               } else {

                  // We didn't go all the way around, but the while-loop
                  // terminated - meaning we found something, and generalIter
                  // is currently sitting on it.  Return its ID.

                  ret_val = generalIter->second->getId();
               }

            } // end if ( we hit on the first try )

         } // end if ( we have any nodes )

         break;

      } // end case [general]

      case 'c': {

         // Color.

         if ( colorHash.find((int) value) != colorHash.end() ) {

            // Pick a random position.
            randomPosition = randomNumber() % colorHash[(int) value].size();

            int_hash_set::iterator generalIter = colorHash[(int) value].begin();

            // Advance to the random position.

            for ( i = 0; i < randomPosition; i++ ) {
               generalIter++;
            }

            if ( nodes[*generalIter]->getDegree() > 0 ) {

               // If we hit a good node on the first try, no problem.

               ret_val = *generalIter;

            } else {

               // Otherwise...

               done = false;
               fell_off_end = false;

               count = 0;

               // ...cycle around the nodes which match the desired
               // value.  If we hit a good one, stop.  If we cycle all the way
               // around, set the 'done' flag and stop.

               while ( !(done)
                      && (nodes[*generalIter]->getDegree() == 0) ) {

                  if ( fell_off_end && count < randomPosition ) {

                     // If we've cycled around the end of the node list,
                     // but we haven't yet advanced to our original position,
                     // keep going, and keep track of how far we've advanced.

                     count++;

                  } else if ( fell_off_end ) {

                     // If we've cycled around the end of the node list,
                     // and we DID come back to our original position, set
                     // the 'done' flag and give up - there are no valid nodes.

                     done = true;
                  }

                  if ( !done ) {

                     generalIter++;

                     // If we've just fallen off the end of the list,

                     if ( generalIter == colorHash[(int) value].end() ) {

                        // Start over at the beginning.

                        fell_off_end = true;
                        generalIter = colorHash[(int) value].begin();
                     }
                  }

               } // end while ( we're still looking for One Good Node )

               if ( done ) {

                  // We wrapped all the way around and didn't find anything.
                  // Give up.

                  ret_val = -1;

               } else {

                  // We didn't go all the way around, but the while-loop
                  // terminated - meaning we found something, and generalIter
                  // is currently sitting on it.  Return its ID.

                  ret_val = *generalIter;
               }

            } // end if ( we hit on the first try )

         } // end if ( we have any nodes )

         break;

      } // end case [color]

      case 'w': {

         // Weight.

         if ( weightHash.find(value) != weightHash.end() ) {

            // Pick a random position.
            randomPosition = randomNumber()
                                    % weightHash[value].size();

            int_hash_set::iterator generalIter =
                                       weightHash[value].begin();

            // Advance to the random position.

            for ( i = 0; i < randomPosition; i++ ) {
               generalIter++;
            }

            if ( nodes[*generalIter]->getDegree() > 0 ) {

               // If we hit a good node on the first try, no problem.

               ret_val = *generalIter;

            } else {

               // Otherwise...

               done = false;
               fell_off_end = false;

               count = 0;

               // ...cycle around the nodes which match the desired
               // value.  If we hit a good one, stop.  If we cycle all the way
               // around, set the 'done' flag and stop.

               while ( !(done)
                        && (nodes[*generalIter]->getDegree() == 0) ) {

                  if ( fell_off_end && count < randomPosition ) {

                     // If we've cycled around the end of the node list,
                     // but we haven't yet advanced to our original position,
                     // keep going, and keep track of how far we've advanced.

                     count++;

                  } else if ( fell_off_end ) {

                     // If we've cycled around the end of the node list,
                     // and we DID come back to our original position, set
                     // the 'done' flag and give up - there are no valid nodes.

                     done = true;
                  }

                  if ( !done ) {

                     generalIter++;

                     // If we've just fallen off the end of the list,

                     if ( generalIter == weightHash[value].end() ) {

                        // Start over at the beginning.

                        fell_off_end = true;
                        generalIter = weightHash[value].begin();
                     }
                  }

               } // end while ( we're still looking for One Good Node )

               if ( done ) {

                  // We wrapped all the way around and didn't find anything.
                  // Give up.

                  ret_val = -1;

               } else {

                  // We didn't go all the way around, but the while-loop
                  // terminated - meaning we found something, and generalIter
                  // is currently sitting on it.  Return its ID.

                  ret_val = *generalIter;
               }

            } // end if ( we hit on the first try )

         } // end if ( we have any nodes )

         break;

      } // end case [weight]

      case 'd': {

         // Degree.

         if (
               (degreeHash.find((int) value) != degreeHash.end())
            &&
               ((int) value != 0 )
            ) {

            randomPosition = randomNumber() % degreeHash[(int) value].size();

            int_hash_set::iterator degreeIter = degreeHash[(int) value].begin();

            for ( i = 0; i < randomPosition; i++ ) {
               degreeIter++;
            }

            ret_val = *degreeIter;
         }

         break;

      } // end case [degree]

      case 'H': {

         // Node's weight >= (value).

         if ( weightHash.size() != 0 ) {

            // First, locate all valid values, if any.

            set<double> valid_values;
            doubleKey_ID_map_t::iterator iter = weightHash.begin();

            while ( iter != weightHash.end() ) {

               if ( isGreaterEqual( iter->first, value ) ) {
                  valid_values.insert(iter->first);
               }

               iter++;
            }

            // If there _are_ any valid values,

            if ( valid_values.size() > 0 ) {

               // We're looking for open nodes, so create a vector consisting of
               // all node IDs within the valid_values range which don't point
               // to nodes with degree > 0.

               vector<int> idVector;

               set<double>::iterator set_iter = valid_values.begin();

               while ( set_iter != valid_values.end() ) {

                  // Scan each weight's int_hash_set for good nodes.

                  int_hash_set::iterator innerIter
                                                = weightHash[*set_iter].begin();

                  while ( innerIter != weightHash[*set_iter].end() ) {

                     if ( nodes[*innerIter]->getDegree() > 0 ) {

                        idVector.push_back(*innerIter);
                     }

                     innerIter++;

                  } // end while (parse nodes matching a particular weight)

                  set_iter++;

               } // end while (parse valid weights)

               // If we have any matching nodes,

               if ( idVector.size() > 0 ) {

                  // Next, find an 'absolute' position across all valid nodes.

                  randomPosition = randomNumber() % idVector.size();

                  // Lastly, locate the actual node to which this position
                  // refers, and return its ID.

                  ret_val = idVector[randomPosition];

               } // end if ( we got any matching nodes )

            } // end if ( there are any valid weights )

         } // end if ( there are any weights at all )

         break;

      } // end case [weight >= given value]

      case 'h': {

         // Node's weight <= (value).

         if ( weightHash.size() != 0 ) {

            // First, locate all valid values, if any.

            set<double> valid_values;
            doubleKey_ID_map_t::iterator iter = weightHash.begin();

            while ( iter != weightHash.end() ) {

               if ( isLessEqual( iter->first, value ) ) {
                  valid_values.insert(iter->first);
               }

               iter++;
            }

            // If there _are_ any valid values,

            if ( valid_values.size() > 0 ) {

               // We're looking for open nodes, so create a vector consisting of
               // all node IDs within the valid_values range which don't point
               // to nodes with degree > 0.

               vector<int> idVector;

               set<double>::iterator set_iter = valid_values.begin();

               while ( set_iter != valid_values.end() ) {

                  // Scan each weight's int_hash_set for good nodes.

                  int_hash_set::iterator innerIter
                                                = weightHash[*set_iter].begin();

                  while ( innerIter != weightHash[*set_iter].end() ) {

                     if ( nodes[*innerIter]->getDegree() > 0 ) {

                        idVector.push_back(*innerIter);
                     }

                     innerIter++;

                  } // end while (parse nodes matching a particular weight)

                  set_iter++;

               } // end while (parse valid weights)

               // If we have any matching nodes,

               if ( idVector.size() > 0 ) {

                  // Next, find an 'absolute' position across all valid nodes.

                  randomPosition = randomNumber() % idVector.size();

                  // Lastly, locate the actual node to which this position
                  // refers, and return its ID.

                  ret_val = idVector[randomPosition];

               } // end if ( we got any matching nodes )

            } // end if ( there are any valid weights )

         } // end if ( there are any weights at all )

         break;

      } // end case [weight <= given value]

      case 'G': {

         // Node's degree >= (value).

         if ( nodes.size() != 0 ) {

            // First, locate all valid values, if any.

            set<int> valid_values;
            intKey_ID_map_t::iterator iter = degreeHash.begin();

            while ( iter != degreeHash.end() ) {

               if (
                     (isGreaterEqual(iter->first, value))
                  &&
                     (iter->first != 0)
                  ) {
                  valid_values.insert(iter->first);
               }

               iter++;
            }

            // If there _are_ any valid values,

            if ( valid_values.size() > 0 ) {

               // First, count how many nodes exist in our selected subset,
               // which matches the given criterion.

               int valid_count = 0;

               set<int>::iterator set_iter = valid_values.begin();

               while ( set_iter != valid_values.end() ) {

                  valid_count += degreeHash[*set_iter].size();
                  set_iter++;
               }

               // Next, find an 'absolute' position across all valid nodes.

               randomPosition = randomNumber() % valid_count;

               // Lastly, locate the actual node to which this position
               // actually refers, and return its ID.

               set_iter = valid_values.begin();

               while ( randomPosition >= degreeHash[*set_iter].size() ) {
                  randomPosition -= degreeHash[*set_iter].size();
                  set_iter++;
               }

               int_hash_set::iterator target_iter
                                                = degreeHash[*set_iter].begin();

               while ( randomPosition > 0 ) {
                  target_iter++;
                  randomPosition--;
               }

               ret_val = *target_iter;

            } // end if ( there are any valid values )

         } // end if ( there are any nodes at all )

         break;

      } // end case [degree >= given value]

      case 'g': {

         // Node's degree <= (value).

         if ( nodes.size() != 0 ) {

            // First, locate all valid values, if any.

            set<int> valid_values;
            intKey_ID_map_t::iterator iter = degreeHash.begin();

            while ( iter != degreeHash.end() ) {

               if (
                     (isLessEqual(iter->first, value))
                  &&
                     (iter->first != 0)
                  ) {
                  valid_values.insert(iter->first);
               }

               iter++;
            }

            // If there _are_ any valid values,

            if ( valid_values.size() > 0 ) {

               // First, count how many nodes exist in our selected subset,
               // which matches the given criterion.

               int valid_count = 0;

               set<int>::iterator set_iter = valid_values.begin();

               while ( set_iter != valid_values.end() ) {

                  valid_count += degreeHash[*set_iter].size();
                  set_iter++;
               }

               // Next, find an 'absolute' position across all valid nodes.

               randomPosition = randomNumber() % valid_count;

               // Lastly, locate the actual node to which this position
               // actually refers, and return its ID.

               set_iter = valid_values.begin();

               while ( randomPosition >= degreeHash[*set_iter].size() ) {
                  randomPosition -= degreeHash[*set_iter].size();
                  set_iter++;
               }

               int_hash_set::iterator target_iter
                                                = degreeHash[*set_iter].begin();

               while ( randomPosition > 0 ) {
                  target_iter++;
                  randomPosition--;
               }

               ret_val = *target_iter;

            } // end if ( there are any valid values )

         } // end if ( there are any nodes at all )

         break;

      } // end case [degree <= given value]

   } // end switch (attribute)

   return ret_val;
}
