#include "LuTopologyGenerator.h"
#include <queue>
#include <algorithm>
#include <string>
#include <cstdlib>
#include <sstream>
#include <iostream>

#define WHITE 0
#define GRAY 1
#define BLACK 2

using namespace std;

typedef hash_map<int, multiNode*> node_hash_t;
typedef hash_map<int, multiEdge*> edge_hash_t;


void indexComponents(multiGraphManager* mgm,
      hash_map<int, hash_set<int> >& componentHash);

bool indexComponentsAndCheckCycles(multiGraphManager* mgm,
      hash_map<int, hash_set<int> >& componentHash, hash_set<int>& cyclicComponents);
   
void countHighestDegreeNodes(multiGraphManager* mgm, int N, queue<int>& highestDegreeNodes);

void extractComponent(multiGraphManager* mgm, hash_set<int> targetComponent);

void identifyNeighborhood(multiGraphManager* mgm, int centerID, int size, vector<int>& storage);

bool isForest(multiGraphManager* mgm);

bool isInTreeComponent(int node, multiGraphManager* mgm);

int countComponentsWithCycles(multiGraphManager* mgm, hash_map<int, hash_set<int> > components);
int countExtraEdges(          multiGraphManager* mgm, hash_map<int, hash_set<int> > components,
      hash_set<int> cyclicComponents);

template <class T>
string to_string(T t) {
   
      ostringstream oss;
      oss << t;
      return oss.str();
}




int main(int argc, char* argv[]) {

   char* outputfilename = "main.xml";
   char* betaArg = argv[1];
   int i = atoi(argv[2]);
   
   multiGraphManager* mgm = new multiGraphManager(outputfilename);
   
   // Node size for graph.
   
   int n = 20000;
   
//   // Growth parameter for power-law generator.
//   
//   double beta = 1.2;
   
   // Number of random instances for each set of parameters.
   
   int filecount = 30;
   
   // Number of highest-degree nodes to be removed from giant component in
   // connectivity analysis.
   
   int topCount = 1;

   // Size of neighborhood to be deleted from around each high-degree node
   // in connectivity analysis.
   
   int neighborhoodSize = atoi(argv[3]);
   
   // Initialize reusable data structures.
   
   bool atEnd;
   bool forest;
   
   string filename = "";
   char* final_filename;

   hash_map<int, hash_set<int> > components;
   hash_map<int, hash_set<int> >::iterator compIter;
   
   hash_set<int> cyclicComponents;
   
   vector< pair<int, int> > sortedComponentIndices;
   vector< pair<int, int> >::iterator sortedComponentIndicesIter;
   
   vector<int> unionOfNeighborhoods;
   vector<int>::iterator unionIter;
   
   // Run through all the files conforming to current settings & send
   // all analysis to cout.
   
//   while ( beta < 2.00 ) {

//    for ( int i = 0; i < filecount; i++ ) {
	 
	      int dummyInt_1;
         int dummyInt_2;
         char* dummyString;

         // Make sure to take care of C++'s idiotically poorly-supported
         // num-to-string conversion.  Nothing this elementary should be
         // this hard.

         filename = "N_";
         filename += to_string<int>(n);
         filename += '_';
         filename += "beta_";

//         dummyString = fcvt(beta, 2, &dummyInt_1, &dummyInt_2);
         dummyString = betaArg;

         filename += dummyString[0];
         filename += '.';
         filename += dummyString[2];

         filename += '_';
         
         if ( i + 1 < 10 ) {
         
            filename += to_string<int>(0);
         }
         
         filename += to_string<int>(i+1);
         filename += ".xml";

         // Remove const-ness from filename.

         final_filename = (char *) filename.c_str();

	      cerr << "Processing file " << final_filename << endl;
         
	      mgm->processFile(final_filename);
	 
         cout << n << " " << dummyString[0] << "." << dummyString[2] << " ";
         cout << (i+1) << " ";
         
         // Index components and check for cycles.

         forest = indexComponentsAndCheckCycles(mgm, components, cyclicComponents);
         
         // Reverse-sort component indices by size.

         compIter = components.begin();
         
         while ( compIter != components.end() ) {

            pair<int, int> element;
            
            element.first = compIter->second.size();
            element.second = compIter->first;

            sortedComponentIndices.push_back(element);
            compIter++;
         }
         
         sort(sortedComponentIndices.begin(), sortedComponentIndices.end(),
               greater< pair<int, int> >());
         
         // Print the number of components in G.
         
         cout << components.size();
         
         // Print sizes for the 10 largest components.
         
         sortedComponentIndicesIter = sortedComponentIndices.begin();
         
         atEnd = false;
         
         for ( int j = 0; j < 10; j++ ) {
            
            if ( sortedComponentIndicesIter == sortedComponentIndices.end() ) {
            
               atEnd = true;
            }
            
            if ( !atEnd ) {
            
               cout << " " << components[(*sortedComponentIndicesIter).second].size();
               
            } else {
               
               cout << " NA";
            }
               
            sortedComponentIndicesIter++;
         }
         
         // (Deprecated.  'forest' is now set by indexComponentsAndCheckCycles above.)
         // 
         // forest = isForest(mgm);
         
         if (forest) {
         
            cout << " 1 0";
         
         } else {
            
            cout << " 0";
            cout << " " << cyclicComponents.size();
         }
         
         // ------------------------------------------------------------------
         

         // Delete all vertices not in the giant component from the current graph.
         
         extractComponent(mgm, components[sortedComponentIndices[0].second]);
         
         // Output the giant component's diameter.
         // Currently disabled as it's slow and I need data.
	 
         // cout << " " << mgm->hopDiameter();
            cout << " " << -1;
         
         // Remove the [topCount] highest-degree vertices from the giant component
         // along with all vertices at a distance <= [neighborhoodSize] from it.
         
         cout << " " << neighborhoodSize << " " << topCount;
         
         // First, identify the [topCount] highest-degree nodes.
         
         // [Note: I didn't declare [highestDegreeNodes] at the top because it
         // doesn't appear to have a 'clear()' function and this seemed the
         // easiest equivalent.
         
         queue<int> highestDegreeNodes;
         countHighestDegreeNodes(mgm, topCount, highestDegreeNodes);
         
         // Next, identify a [neighborhoodSize]-sized neighborhood around
         // each.
         
         while ( !(highestDegreeNodes.empty()) ) {

            int currentNode = highestDegreeNodes.front();
	    
            identifyNeighborhood(mgm, currentNode, neighborhoodSize, unionOfNeighborhoods);

            highestDegreeNodes.pop();
         }
         
      	 // Finally, delete all vertices in the union of neighborhoods around
      	 // the selected high-degree vertices.
	 
	      unionIter = unionOfNeighborhoods.begin();
	 
	      while ( unionIter != unionOfNeighborhoods.end() ) {
	    
	         mgm->deleteNode(*unionIter);

	         unionIter++;
	      }
	 
         // Index remaining components and check for cycles.

         components.clear();
         cyclicComponents.clear();
         forest = indexComponentsAndCheckCycles(mgm, components, cyclicComponents);
         
         // Reverse-sort remaining component indices by size.
         
         sortedComponentIndices.clear();
         compIter = components.begin();
         
         while ( compIter != components.end() ) {
            
            pair<int, int> element;
               
            element.first = compIter->second.size();
            element.second = compIter->first;
            
            sortedComponentIndices.push_back(element);
            compIter++;
         }
         
         sort(sortedComponentIndices.begin(), sortedComponentIndices.end(),
               greater< pair<int, int> >());
         
         // Print the number of components in G'.
         
         cout << " " << components.size() << " " << mgm->nodeSize();
         
         // Print the sizes of the 10 largest components in G'.
         
         sortedComponentIndicesIter = sortedComponentIndices.begin();
                  
         atEnd = false;
                  
         for ( int j = 0; j < 10; j++ ) {
            
            if ( sortedComponentIndicesIter == sortedComponentIndices.end() ) {
            
               atEnd = true;
            }
            
            if ( !atEnd ) {
            
               cout << " " << components[(*sortedComponentIndicesIter).second].size();
               
            } else {
               
               cout << " NA";
            }
            
            sortedComponentIndicesIter++;
         }
         
         // Output the largest remaining component's diameter.
         // Currently disabled since it's slow and I need data.
	 
         // cout << " " << mgm->hopDiameter();
            cout << " " << -1;
         
         // (Deprecated.  'forest' is set by indexComponentsAndCheckCycles above.)
         // 
         // forest = isForest(mgm);
         
         if (forest) {
         
            cout << " 1 0 0";
         
         } else {
         
            cout << " 0";
            
            cout << " " << cyclicComponents.size();
            
	         cout << " " << countExtraEdges(mgm, components, cyclicComponents);
         }
         
         // Reset data structures for use with the next graph data-file
         // & start a new output line.
         
         components.clear();
         cyclicComponents.clear();
         sortedComponentIndices.clear();
         unionOfNeighborhoods.clear();

         cout << endl;
         
//      } // end for (each random instance at this setting)
//
//     beta += 0.1;
//   }

   delete mgm;

   return 0;
}


// ---------------------------------SUBROUTINES-------------------------------------
// 
// File all vertex indices into their respective components, and at the same
// time check for any cycles.  Return value is TRUE if the input graph is a forest
// (counting self-loops and parallel edges as cycles, since had the graph been simple,
// each such edge almost certainly would have created a cycle anyway), FALSE otherwise.

bool indexComponentsAndCheckCycles(multiGraphManager* mgm,
         hash_map<int, hash_set<int> >& componentHash, hash_set<int>& cyclicComponents) {
   
   hash_map<int, int> color;
   int componentIndex = 0;
   bool cycle = false;

   // Initialize data structures for BFS.

   hash_map<int, multiNode* >::iterator nodeIter = mgm->G->nodes.begin();

   while ( nodeIter != mgm->G->nodes.end() ) {

      color[nodeIter->first] = WHITE;
      nodeIter++;
   }
   
   queue<int> Q;
   int currentVertex;
   vector<int> neighborList;
   vector<int>::iterator neighborIter;
   
   // Grow a BFS from each unseen vertex, marking vertices as they're
   // observed & indexing component membership.

   nodeIter = mgm->G->nodes.begin();

   while ( nodeIter != mgm->G->nodes.end() ) {
      
      // If we haven't seen this node yet, grow a BFS from it & index its
      // containing component, checking for cycles.
      
      if ( color[nodeIter->first] == WHITE ) {

          color[nodeIter->first] = GRAY;
          Q.push(nodeIter->first);
          componentHash[componentIndex].insert(nodeIter->first);

          while ( !(Q.empty()) ) {

             currentVertex = Q.front();
             mgm->G->nodes[currentVertex]->getNeighbors(neighborList);

             neighborIter = neighborList.begin();

             while ( neighborIter != neighborList.end() ) {

                if ( color[*neighborIter] == WHITE ) {

                  color[*neighborIter] = GRAY;
                  Q.push(*neighborIter);
                  componentHash[componentIndex].insert(*neighborIter);
                  
                } else {
                  
               // Deprecated; we used to ignore loops.    
               // if ( color[*neighborIter] == GRAY && *neighborIter != currentVertex ) {
                  
                  if ( color[*neighborIter] == GRAY ) {
                     
                     // We've seen this guy already, and so there's a cycle here.
                     
                     cycle = true;
                     cyclicComponents.insert(componentIndex);
                  }
                }

                neighborIter++;

             } // end while ( we're scanning the neighbors of the current vertex )

             color[currentVertex] = BLACK;
             Q.pop();

          } // end while( we're still scanning the current component )

          componentIndex++;

      } // end if ( we haven't seen this node yet )

      nodeIter++;

   } // end [outer] while ( we're scanning through all nodes looking for new
     // components )
     
   return !(cycle);
}


// Remove all vertices _not_ in "targetComponent" from mgm.

void extractComponent(multiGraphManager* mgm, hash_set<int> targetComponent) {
   
   hash_map<int, int> color;
   hash_map<int, multiNode*>::iterator iter = mgm->G->nodes.begin();
   vector<int> indices;
   
   // First, grab all IDs of all nodes in mgm.  Do this so as to avoid
   // screwing up the iterator when we delete nodes (in the second pass).
   // Mark each initially to be WHITE.
   
   while ( iter != mgm->G->nodes.end() ) {
      
      indices.push_back(iter->first);
      color[iter->first] = WHITE;
      
      iter++;
   }
   
   // Mark all the nodes in targetComponent to be BLACK.
   
   std::hash_set<int>::iterator targetIter = targetComponent.begin();
   
   while ( targetIter != targetComponent.end() ) {
      
      color[*targetIter] = BLACK;
      
      targetIter++;
   }
   
   // Delete all non-BLACK nodes.
   
   vector<int>::iterator vectorIter = indices.begin();
   
   while ( vectorIter != indices.end() ) {
      
      if ( color[*vectorIter] == WHITE ) {
	 
         mgm->deleteNode(*vectorIter);
      }

      vectorIter++;
   }
}


// Identify the [upper_limit] highest-degree nodes.

void countHighestDegreeNodes(multiGraphManager* mgm, int N, queue<int>& highestDegreeNodes) {
   
   int upper_limit = N;
   vector<int> degreeVector;
   int nodeCount = 0;

   hash_map<int, hash_set<int> >::iterator iter = mgm->G->degreeHash.begin();
   
   while ( iter != mgm->G->degreeHash.end() ) {

      degreeVector.push_back( iter->first );
      iter++;
   }

   sort(degreeVector.begin(), degreeVector.end(), greater<int>());
   
   vector<int>::iterator vectorIter = degreeVector.begin();
   
   int currentDegree = *vectorIter;
   
   while ( vectorIter != degreeVector.end() && nodeCount < upper_limit ) {

      std::hash_set<int>::iterator nodeIter = mgm->G->degreeHash[currentDegree].begin();
      
      if ( mgm->G->degreeHash[currentDegree].size() <= (upper_limit - nodeCount) ) {
      
         while ( nodeIter != mgm->G->degreeHash[currentDegree].end() ) {
            
            highestDegreeNodes.push(*nodeIter);
            nodeIter++;
            nodeCount++;
         }
         
      } else {
         
         while ( nodeCount < upper_limit ) {
            
            highestDegreeNodes.push(*nodeIter);
            nodeIter++;
            nodeCount++;
         }
      }

      currentDegree = *(++vectorIter);

   } // end while ( outer loop iterating across available degrees )
   
}


// Identify a radius-[size] neighborhood from around node [centerID].

void identifyNeighborhood(multiGraphManager* mgm, int centerID, int size, vector<int>& storage) {

   // [WHITE: unprocessed.  GRAY: needs processing.]
   // [BLACK: done, leave it alone.]
   
   hash_map<int, int> color;
   hash_map<int, int> level;
   
   queue<int> neighborhood;
   
   // Initialize maps; color everyone WHITE & set everyone's depth level to
   // fake-infinity.
   
   hash_map<int, multiNode*>::iterator nodeIter = mgm->G->nodes.begin();

   while ( nodeIter != mgm->G->nodes.end() ) {

      color[nodeIter->first] = WHITE;
      level[nodeIter->first] = mgm->nodeSize() + 1;

      nodeIter++;
   }

   color[centerID] = GRAY;
   level[centerID] = 0;
   neighborhood.push(centerID);

   while ( !(neighborhood.empty()) ) {
      
      int currentNode = neighborhood.front();
      color[currentNode] = BLACK;

      vector<int> neighborList;
      vector<int>::iterator neighborIter;
      
      // Grab the neighbors of the current node.
      
      mgm->G->nodes[currentNode]->getNeighbors(neighborList);
      neighborIter = neighborList.begin();
      
      // For each neighbor,
      
      while ( neighborIter != neighborList.end() ) {
	      
         // If we care about this neighbor of the current node,

         if ( color[*neighborIter] == WHITE && level[currentNode] < size ) {

            // Mark it for neighborhood-inclusion and further investigation.

            color[*neighborIter] = GRAY;
            level[*neighborIter] = level[currentNode] + 1;
            neighborhood.push(*neighborIter);
         }

         neighborIter++;
      }
      
      neighborhood.pop();
   }

   // Now that we've colored the neighborhood, just go through and save
   // it node-by-node.
   
   hash_map<int, int>::iterator colorIter = color.begin();

   while( colorIter != color.end() ) {
      
      if ( colorIter->second != WHITE ) {

         storage.push_back(colorIter->first);
      }

      colorIter++;
   }
}


// Count the number of 'extra' edges in the input graph, i.e. for each connected
// component C which isn't a tree,
// 
//   Count the difference |E(C)| - (|V(C)| - 1).
// 
// NOTE: THIS IS DESTRUCTIVE.
// 
// We consider self-loops and parallel edges to be cycles.  (see comment preceding indexComponentsAndCheckCycles.)

int countExtraEdges(multiGraphManager* mgm, hash_map<int, hash_set<int> > components, hash_set<int> cyclicComponents) {
   
   int finalCount = 0;
   int incrementEdges = 0;
   int edgesInComponent = 0;
   std::hash_set<int>::iterator currentComponentNodeIter;

   std::hash_set<int>::iterator cyclicIter = cyclicComponents.begin();
   
   // DEPRECATED: We used to remove loops.  Now we don't.

   /*

   // Iterate through each component which has been identified as having
   // a (non-self-loop) cycle, and actually remove all self-loops.
   
   // For each component containing a (non-self-loop) cycle,
   
   while( cyclicIter != cyclicComponents.end() ) {
      
      currentComponentNodeIter = components[*cyclicIter].begin();
      
      // For each node in this component,
      
      while ( currentComponentNodeIter != components[*cyclicIter].end() ) {
         
         // While this node still has an attached loop,
         
         while ( mgm->G->nodes[*currentComponentNodeIter]->hasNeighbor(*currentComponentNodeIter) ) {
            
            // Remove the loop.
            
            vector<int> neighbors;
            mgm->G->nodes[*currentComponentNodeIter]->getNeighbors(neighbors);
            
            mgm->G->deleteEdge(*currentComponentNodeIter, *currentComponentNodeIter);
         }
         
         currentComponentNodeIter++;
      }

      cyclicIter++;
   }
   
   // Reset the cyclic-components iterator.
   
   cyclicIter = cyclicComponents.begin();
   
   */
   
   // For each component which contains a cycle,
   
   while ( cyclicIter != cyclicComponents.end() ) {
         
      // Count the edges in the current component by summing node degrees &
      // dividing by 2.
      
      currentComponentNodeIter = components[*cyclicIter].begin();
      
      while ( currentComponentNodeIter != components[*cyclicIter].end() ) {
         
         edgesInComponent += mgm->G->nodes[*currentComponentNodeIter]->getDegree();
         
         currentComponentNodeIter++;
      }
      
      edgesInComponent /= 2;
      
      // Calculate, for the current component C, |E(C)| - (|C|-1)
      
      incrementEdges = edgesInComponent - (components[*cyclicIter].size() - 1);
      
      finalCount += incrementEdges;
      
      edgesInComponent = 0;
      
      cyclicIter++;
   }
   
   return finalCount;
}


// ------------------------------DEPRECATED-------------------------------
// 
// File all vertex indices into their respective components.
// (Use indexComponentsAndCheckCycles instead).

void indexComponents(multiGraphManager* mgm, hash_map<int, hash_set<int> >& componentHash) {
   
   hash_map<int, int> color;
   int componentIndex = 0;

   // Initialize data structures for BFS.

   hash_map<int, multiNode* >::iterator nodeIter = mgm->G->nodes.begin();

   while ( nodeIter != mgm->G->nodes.end() ) {

      color[nodeIter->first] = WHITE;
      nodeIter++;
   }
   
   queue<int> Q;
   int currentVertex;
   vector<int> neighborList;
   vector<int>::iterator neighborIter;
   
   // Grow a BFS from each unseen vertex, marking vertices as they're
   // observed & indexing component membership.

   nodeIter = mgm->G->nodes.begin();

   while ( nodeIter != mgm->G->nodes.end() ) {
      
      // If we haven't seen this node yet, grow a BFS from it & index its
      // containing component.
      
      if ( color[nodeIter->first] == WHITE ) {

          color[nodeIter->first] = GRAY;
          Q.push(nodeIter->first);
          componentHash[componentIndex].insert(nodeIter->first);

          while ( !(Q.empty()) ) {

             currentVertex = Q.front();
             mgm->G->nodes[currentVertex]->getNeighbors(neighborList);

             neighborIter = neighborList.begin();

             while ( neighborIter != neighborList.end() ) {

                if ( color[*neighborIter] == WHITE ) {

                  color[*neighborIter] = GRAY;
                  Q.push(*neighborIter);
                  componentHash[componentIndex].insert(*neighborIter);
                }

                neighborIter++;

             } // end while ( we're scanning the neighbors of the current
               // vertex )

             color[currentVertex] = BLACK;
             Q.pop();

          } // end while( we're still scanning the current component )

          componentIndex++;

      } // end if ( we haven't seen this node yet )

      nodeIter++;

   } // end [outer] while ( we're scanning through all nodes looking for new
     // components )
}


// ------------------------------DEPRECATED-------------------------------
// 
// Is the graph argument a forest?
// IGNORE SELF-LOOPS.
// (Combined with indexComponentsAndCheckCycles.)

bool isForest(multiGraphManager* mgm) {

   hash_map<int, int> color;

   // Initialize data structures for BFS.

   hash_map<int, multiNode* >::iterator nodeIter = mgm->G->nodes.begin();

   while ( nodeIter != mgm->G->nodes.end() ) {

      color[nodeIter->first] = WHITE;
      nodeIter++;
   }

   queue<int> Q;
   int currentVertex;
   vector<int> neighborList;
   vector<int>::iterator neighborIter;
   bool cycle = false;

   // Grow a BFS from each unseen vertex, marking vertices as they're
   // observed & checking for cycles.

   nodeIter = mgm->G->nodes.begin();

   while ( nodeIter != mgm->G->nodes.end() && !(cycle) ) {
      
      // If we haven't seen this node yet, grow a BFS from it & check for
      // cycles in its component.
      
      if ( color[nodeIter->first] == WHITE ) {

	       color[nodeIter->first] = GRAY;
      	 Q.push(nodeIter->first);
      
      	 while ( !(Q.empty()) && !(cycle) ) {
      	    
      	    // Identify the current local center.
      	    
      	    currentVertex = Q.front();
      	    
      	    // Check its neighbors.  If we've seen one before that isn't
      	    // black, we're in trouble.
      	    
      	    mgm->G->nodes[currentVertex]->getNeighbors(neighborList);
      
      	    neighborIter = neighborList.begin();
      
      	    while ( neighborIter != neighborList.end() && !(cycle) ) {
      
      	       if ( color[*neighborIter] == WHITE ) {
      		  
            		  // We haven't seen this guy yet.  Tag him & bag him.
            		  
            		  color[*neighborIter] = GRAY;
            		  Q.push(*neighborIter);
            		  
      	       } else {
      
                    if ( color[*neighborIter] == GRAY && *neighborIter != currentVertex ) {
      
                       // Rut roh.
      
                       cycle = true;
                    }
      	       }
      		  
      	       neighborIter++;
      	    }
      	    
      	    color[currentVertex] = BLACK;
      	    Q.pop();
      	    
      	 } // end while ( queue checking the current component isn't empty )
      	 
      } // end if ( we haven't seen this yet )
      
      nodeIter++;
      
   } // end while ( we're iterating through all the nodes looking for new
     // stuff )
   
   return !(cycle);
}


// ------------------------------DEPRECATED-------------------------------
// 
// Count the components of the argument graph which contain cycles.
// (Merged with indexComponentsAndCheckCycles.)

int countComponentsWithCycles(multiGraphManager* mgm, hash_map<int, hash_set<int> > components) {
   
   int finalCount = 0;
   
   hash_map<int, hash_set<int> >::iterator cycleIter = components.begin();
   
   while ( cycleIter != components.end() ) {
      
      // Is the current component a tree?  If not, add it to our tally.
      
      int tempNode = *((cycleIter->second).begin());
      
      if ( !isInTreeComponent(tempNode, mgm) ) {
         finalCount++;
      }
      
      cycleIter++;
   }
   
   return finalCount;
}


// ------------------------------DEPRECATED-------------------------------
// 
// Is the node argument inside a component which is a tree?
// IGNORE SELF-LOOPS.
// (Removed.  countExtraEdges manages this function on its own now, with help
//  from cyclicComponents.)

bool isInTreeComponent(int centerNode, multiGraphManager* mgm) {

   // Grow a BFS from the node & die if you hit a cycle.

   hash_map<int, int> color;

   // Initialize data structures for BFS.

   hash_map<int, multiNode* >::iterator nodeIter = mgm->G->nodes.begin();

   while ( nodeIter != mgm->G->nodes.end() ) {

      color[nodeIter->first] = WHITE;
      nodeIter++;
   }

   queue<int> Q;
   int currentVertex;
   vector<int> neighborList;
   vector<int>::iterator neighborIter;
   bool cycle = false;

   // Grow a BFS from the argument node, marking vertices as they're
   // observed & checking for cycles.

   color[centerNode] = GRAY;
   Q.push(centerNode);

   while ( !(Q.empty()) && !(cycle) ) {

      // Identify the current local center.

      currentVertex = Q.front();

      // Check its neighbors.  If we've seen one before that isn't
      // black, we're in trouble.

      mgm->G->nodes[currentVertex]->getNeighbors(neighborList);
      neighborIter = neighborList.begin();

      while ( neighborIter != neighborList.end() && !(cycle) ) {

         if ( color[*neighborIter] == WHITE ) {

            // We haven't seen this guy yet.  Tag him & bag him.

            color[*neighborIter] = GRAY;
            Q.push(*neighborIter);

         } else {

            if ( color[*neighborIter] == GRAY && *neighborIter != currentVertex ) {

               // Rut roh.

               cycle = true;
            }
         }

         neighborIter++;
      }

      color[currentVertex] = BLACK;
      Q.pop();

   } // end while ( queue checking the current component isn't empty )
   
   return !(cycle);
}
