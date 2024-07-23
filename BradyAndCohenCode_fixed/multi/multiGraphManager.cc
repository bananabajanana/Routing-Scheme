// multiGraphManager.cc: Definitions for file & object controller wrapping
// [undirected] multigraph object.
#include <cstring> // Add this line
#include "multiGraphManager.h"
#include <cstdlib>
#include <string>

#define WEIGHT_INIT 42.87597

void initTag( xmlTag& tag );
bool moreAttributeData( string& line );
void getNextPair( string& line, xmlTag& tag );

// -------------
// Constructors.
// -------------

// Simplest constructor: Create an empty graph, init all data members
// to reasonable defaults, open a stream for an XML output file.
// Overwrite any preexisting file with this name.

multiGraphManager::multiGraphManager( char* outputfilename ) {

   G = new multiGraph();

   nodes = &(G->nodes);
   edges = &(G->edges);

   output_file_name = outputfilename;
   recording = 0;
   structureInfo = 0;

   output_file_stream.open(output_file_name);

   assert(output_file_stream);

   output_file_stream << "<graph nodeSize=\"0\" "
      << "directed=\"0\" multi=\"1\">\n";

}

// Copy constructor.

multiGraphManager::multiGraphManager( multiGraphManager& source ) {

   G = new multiGraph( *(source.G) );
   
   nodes = &(G->nodes);
   edges = &(G->edges);
   
   char tempString[200];
   strcpy(tempString, source.output_file_name);
   
   output_file_name = strcat(tempString, ".copy");
   
   recording = source.recording;
   structureInfo = source.structureInfo;
   
   output_file_stream.open(output_file_name);

   assert(output_file_stream);

   output_file_stream << "<graph nodeSize=\"" << G->nodeSize() << "\" "
      << "directed=\"0\" multi=\"1\">\n";
}

// Create a graph with n nodes and no edges, & open a stream for
// an XML output file.  Overwrite any preexisting file with this name.

multiGraphManager::multiGraphManager( int numNodes, char* outputfilename ) {

   G = new multiGraph(numNodes);

   nodes = &(G->nodes);
   edges = &(G->edges);

   output_file_name = outputfilename;
   recording = 0;
   structureInfo = 0;
   output_file_stream.open(output_file_name);

   assert(output_file_stream);

   output_file_stream << "<graph nodeSize=\"" << numNodes
	   << "\" directed=\"0\" multi=\"1\">\n";
}

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

multiGraphManager::multiGraphManager( char graphType, int numNodes, char* outputfilename ) {

   G = new multiGraph(graphType, numNodes);

   nodes = &(G->nodes);
   edges = &(G->edges);

   output_file_name = outputfilename;
   recording = 0;
   structureInfo = 0;
   output_file_stream.open(output_file_name);

   assert(output_file_stream);

   output_file_stream << "<graph nodeSize=\"" << numNodes
      << "\" directed=\"0\" multi=\"1\" graphType=\"" << graphType << "\">\n";
}

// -------------------------------------
// Wrapped graph manipulation functions.
// -------------------------------------

// Add a node to the contained graph & return its ID.

int multiGraphManager::addNode( void ) {

   if ( recording ) {
      output_file_stream << "   <addNode/>\n";
   }

   return G->addNode();
}

// Delete the node with the given index.

void multiGraphManager::deleteNode( int targetNode ) {

   if ( recording ) {
      output_file_stream << "   <deleteNode ID=\"" << targetNode
       << "\"/>\n";
   }

   G->deleteNode(targetNode);
}

// Delete a random node.

void multiGraphManager::deleteRandomNode( void ) {

   int deletedID = G->deleteRandomNode();

   if ( structureInfo && recording ) {
      output_file_stream << "   <deleteRandomNode _ID=\""
	    << deletedID << "\"/>\n";
   } else if ( recording ) {
      output_file_stream << "   <deleteRandomNode/>\n";
   }
}

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

int multiGraphManager::deleteRandomNode( char attribute, double value ) {

   int deletedID = G->deleteRandomNode(attribute, value);

   if ( structureInfo && recording ) {
      output_file_stream << "   <deleteRandomNode attribute=\""
	    << attribute << "\" value=\"" << value << "\" _ID=\""
	    << deletedID << "\"/>\n";
   } else if ( recording ) {
      output_file_stream << "   <deleteRandomNode attribute=\""
	    << attribute << "\" value=\"" << value << "\"/>\n";
   }

   return deletedID;
}

// Add an edge between two named vertices & return its ID.
// If one of the nodes does not exist, return -1.

int multiGraphManager::addEdge( int node_1, int node_2 ) {

   if ( recording ) {
      output_file_stream << "   <addEdge node_1=\"" << node_1 << "\" "
	    << "node_2=\"" << node_2 << "\"/>\n";
   }

   return G->addEdge(node_1, node_2);
}

// Add a random edge, return its ID.

int multiGraphManager::addRandomEdge( void ) {

   int edgeID = G->addRandomEdge();
   int node_1 = -1;
   int node_2 = -1;

   if ( edgeID != -1 ) {
      node_1 = G->edges[edgeID]->getNode1();
      node_2 = G->edges[edgeID]->getNode2();
   }

   if ( recording && structureInfo ) {
      output_file_stream << "   <addRandomEdge _node_1=\""
	    << node_1 << "\" "
       << "_node_2=\"" << node_2
	    << "\"/>\n";
   } else if ( recording ) {
      output_file_stream << "   <addRandomEdge/>\n";
   }

   return edgeID;
}

// Add a random edge to a given node.

int multiGraphManager::addRandomEdge( int node_1 ) {

   int edgeID = G->addRandomEdge(node_1);

   int node_2 = -1;

   if ( edgeID != -1 ) {
      node_2 = G->edges[edgeID]->getNode2();
   }

   if ( recording && structureInfo ) {
      output_file_stream << "   <addRandomEdge node_1=\""
	    << node_1 << "\" "
       << "_node_2=\"" << node_2
	    << "\"/>\n";
   } else if ( recording ) {
      output_file_stream << "   <addRandomEdge node_1=\"" << node_1 << "\"/>\n";
   }

   return edgeID;
}

// Add a random edge incident to a node where some attribute [first
// argument; see table above for deleteRandomNode( char, double )]
// matches some value [second argument].
//
// Returns the edge's ID if successful, -1 if it proved impossible.

int multiGraphManager::addRandomEdge( char attribute, double value ) {

   int edgeID = G->addRandomEdge(attribute, value);
   int node_1 = -1;
   int node_2 = -1;

   if ( edgeID != -1 ) {
      node_1 = G->edges[edgeID]->getNode1();
      node_2 = G->edges[edgeID]->getNode2();
   }

   if ( recording && structureInfo ) {
      output_file_stream << "   <addRandomEdge attribute=\""
       << attribute << "\" value=\"" << value << "\" _node_1=\""
       << node_1 << "\" _node_2=\""
       << node_2 << "\"/>\n";
   } else if ( recording ) {
      output_file_stream << "   <addRandomEdge attribute=\"" << attribute
          << "\" value=\"" << value << "\"/>\n";
   }

   return edgeID;
}

// Add a random edge incident to a given node where some attribute [first
// argument; see table above for deleteRandomNode( char, double )]
// matches some value [second argument] with respect to the node's other
// endpoint.
//
// Returns the edge's ID if successful, -1 if it proved impossible.

int multiGraphManager::addRandomEdge( int node_1, char attribute, double value ) {

   int edgeID = G->addRandomEdge( node_1, attribute, value );
   int node_2 = -1;

   if ( edgeID != -1 ) {
      node_2 = G->edges[edgeID]->getNode2();
   }

   if ( recording && structureInfo ) {

      output_file_stream << "   <addRandomEdge "
	 << "node_1=\"" << node_1
	 << "\" attribute=\"" << attribute
	 << "\" value=\"" << value
	 << "\" _node_2=\"" << node_2
	 << "\"/>\n";

   } else if ( recording ) {

      output_file_stream << "   <addRandomEdge "
	 << "node_1=\"" << node_1
	 << "\" attribute=\"" << attribute
	 << "\" value=\"" << value
	 << "\"/>\n";
   }

   return edgeID;
}

// Add a random edge _between_ two vertices with specified attribute
// constraints, as above.
//
// Returns the edge's ID if successful, -1 if it proved impossible.

int multiGraphManager::addRandomEdge( char attribute_1, double value_1,
                                 char attribute_2, double value_2 ) {

   int edgeID = -1;

   edgeID = G->addRandomEdge(attribute_1,value_1, attribute_2,value_2);
   int node_1 = -1;
   int node_2 = -1;

   if ( edgeID != -1 ) {
      node_1 = G->edges[edgeID]->getNode1();
      node_2 = G->edges[edgeID]->getNode2();
   }

   if ( recording && structureInfo ) {
      output_file_stream << "   <addRandomEdge attribute_1=\""
       << attribute_1 << "\" value_1=\""
       << value_1 << "\" attribute_2=\""
       << attribute_2 << "\" value_2=\""
       << value_2 << "\" _node_1=\""
       << node_1 << "\" _node_2=\""
       << node_2 << "\"/>\n";
   } else if ( recording ) {
      output_file_stream << "   <addRandomEdge attribute_1=\""
       << attribute_1 << "\" value_1=\""
       << value_1 << "\" attribute_2=\""
       << attribute_2 << "\" value_2=\""
       << value_2 << "\"/>\n";
   }

   return edgeID;
}

// Add a random edge _between_ two vertices with specified attribute
// constraints, as above.  Disallow loops & multiedges.
//
// Returns the edge's ID if successful, -1 if it proved impossible.

int multiGraphManager::addRandomNewEdge( char attribute_1, double value_1,
                                 char attribute_2, double value_2 ) {

   int edgeID = -1;

   edgeID = G->addRandomNewEdge(attribute_1, value_1, attribute_2, value_2);
   int node_1 = -1;
   int node_2 = -1;

   if ( edgeID != -1 ) {
      node_1 = G->edges[edgeID]->getNode1();
      node_2 = G->edges[edgeID]->getNode2();
   }

   if ( recording && structureInfo ) {
      output_file_stream << "   <addRandomNewEdge attribute_1=\""
       << attribute_1 << "\" value_1=\""
       << value_1 << "\" attribute_2=\""
       << attribute_2 << "\" value_2=\""
       << value_2 << "\" _node_1=\""
       << node_1 << "\" _node_2=\""
       << node_2 << "\"/>\n";
   } else if ( recording ) {
      output_file_stream << "   <addRandomNewEdge attribute_1=\""
       << attribute_1 << "\" value_1=\""
       << value_1 << "\" attribute_2=\""
       << attribute_2 << "\" value_2=\""
       << value_2 << "\"/>\n";
   }

   return edgeID;
}

// Delete _some_ edge between two specified nodes.

void multiGraphManager::deleteEdge( int node_1, int node_2 ) {

   if ( recording ) {
      output_file_stream << "   <deleteEdge node_1=\"" << node_1
          << "\" node_2=\"" << node_2 << "\"/>\n";
   }

   G->deleteEdge(node_1, node_2);
}

// Delete a specified edge.

void multiGraphManager::deleteEdge( int edgeID ) {

   if ( recording ) {
      output_file_stream << "   <deleteEdge ID=\"" << edgeID << "\"/>\n";
   }

   G->deleteEdge(edgeID);
}

// Delete a random edge.

void multiGraphManager::deleteRandomEdge( void ) {

   vector<int> endpoints = G->deleteRandomEdge();
   int node_1_ID;
   int node_2_ID;

   if ( endpoints.size() != 2 ) {
      node_1_ID = -1;
      node_2_ID = -1;
   } else {
      node_1_ID = endpoints[0];
      node_2_ID = endpoints[1];
   }

   if ( recording && structureInfo ) {
      output_file_stream << "   <deleteRandomEdge _node_1=\"" <<
	 node_1_ID << "\" _node_2=\"" << node_2_ID << "\"/>\n";
   } else if ( recording ) {
      output_file_stream << "   <deleteRandomEdge/>\n";
   }
}

// Delete a random edge incident to a specified node.

void multiGraphManager::deleteRandomEdge( int targetNode ) {

   vector<int> endpoints = G->deleteRandomEdge(targetNode);
   int node_1_ID;
   int node_2_ID;

   if ( endpoints.size() != 2 ) {
      node_1_ID = -1;
      node_2_ID = -1;
   } else {
      node_1_ID = endpoints[0];
      node_2_ID = endpoints[1];
   }

   if ( recording && structureInfo ) {
      output_file_stream << "   <deleteRandomEdge node=\""
	 << targetNode << "\" _node_1=\"" <<
	 node_1_ID << "\" _node_2=\"" << node_2_ID << "\"/>\n";
   } else if ( recording ) {
      output_file_stream << "   <deleteRandomEdge node=\""
	 << targetNode << "\"/>\n";
   }
}

// Delete a random edge indicent to a node matching specified attributes,
// as in addRandomEdge( char, double ) above.

void multiGraphManager::deleteRandomEdge( char attribute, double value ) {

   vector<int> endpoints = G->deleteRandomEdge(attribute, value);
   int node_1_ID, node_2_ID;

   if ( endpoints.size() == 2 ) {
      node_1_ID = endpoints[0];
      node_2_ID = endpoints[1];
   } else {
      node_1_ID = -1;
      node_2_ID = -1;
   }

   if ( recording && structureInfo ) {
      output_file_stream << "   <deleteRandomEdge attribute=\""
	 << attribute << "\" value=\"" << value
	 << "\" _node_1=\"" << node_1_ID
	 << "\" _node_2=\"" << node_2_ID
	 << "\"/>\n";
   } else if ( recording ) {
      output_file_stream << "   <deleteRandomEdge attribute=\""
	 << attribute << "\" value=\""
	 << value << "\"/>\n";
   }
}

// Delete a random edge between two nodes matching specified attributes,
// as in addRandomEdge( char, double, char, double ) above.
//
// *****************************
// DEPRECATED FOR THE TIME BEING
// *****************************

void multiGraphManager::deleteRandomEdge( char attribute_1, double value_1,
			                            char attribute_2, double value_2 ) {

   /*

   vector<int> endpoints = G->deleteRandomEdge(attribute_1, value_1,
						   attribute_2, value_2);

   int node_1_ID, node_2_ID;

   node_1_ID = endpoints[0];
   node_2_ID = endpoints[1];

   if ( recording && structureInfo ) {
      output_file_stream << "   <deleteRandomEdge attribute_1=\""
	 << attribute_1 << "\" value_1=\"" << value_1 << "\" attribute_2=\""
	 << attribute_2 << "\" value_2=\"" << value_2
	 << "\" _node_1=\"" << node_1_ID
	 << "\" _node_2=\"" << node_2_ID
	 << "\"/>\n";
   } else if ( recording ) {
      output_file_stream << "   <deleteRandomEdge attribute_1=\""
	 << attribute_1 << "\" value_1=\""
	 << value_1 << "\" attribute_2=\""
	 << attribute_2 << "\" value_2=\""
	 << value_2 << "\"/>\n";
   }

   */
}

// ------------------------------
// GraphManager-specific methods.
// ------------------------------

// Pause output-recording.

void multiGraphManager::pause( void ) {

   if ( recording ) {
      output_file_stream << "</updateSequence>\n";
   }

   recording = 0;
}

// Switch on output-recording.

void multiGraphManager::record( void ) {

   if ( !(recording) ) {
      output_file_stream << "<updateSequence>\n";
   }

   recording = 1;
}

// Process an XML input file, breaking it first into update-sequences
// and snapshots.  Scan to last snapshot, create a graph to match, then
// process all subsequent updates.
//
// Possibly to be made more flexible in future, e.g. reporting some
// graph statistic at each snapshot, etc.
// 
// IMPORTANT: This is NOT a robust XML parser.  It'll read what DIGG produces,
// but it isn't guaranteed to understand all legal permutations of XML
// information.

void multiGraphManager::processFile( char* filename ) {
   
   ifstream input_file_stream;
   
   input_file_stream.open(filename);
   assert(input_file_stream);
   
   string line;
   vector<string> tagStrings;
   tagStrings.clear();
   
   // Preprocess lines into strings and save in the 'tagStrings' vector.
   
   while ( getline(input_file_stream, line) ) {
      
      // Strip leading whitespace.
      
      while ( line[0] == ' ' || line[0] == '\t' ) {
         line.erase(0,1);
      }
      
      // Strip trailing non-tag characters, backing up to the end of a tag.
      
      while (line[line.length() - 1] != '>' && line.length() > 0) {
         line.erase( (line.length() - 1), 1 );
      }
      
      // If we're at the beginning of a tag,
      
      if ( line[0] == '<' ) {
         
         // Save the tag.
         
         tagStrings.push_back(line);
      }
   }
   
   string tempString;
   
   vector<xmlTag> xmlTags;
   xmlTags.clear();
   
   vector<string>::iterator stringIter = tagStrings.begin();
   
   // Save tags in their final form, as hashes of string/string pairs.
   
   while ( stringIter != tagStrings.end() ) {
      
      line = *stringIter;
      xmlTag currentTag;
      initTag(currentTag);
      
      // Remove initial '<'.  Save potential initial / as part of tag name,
      // i.e., the tag name of "</updateSequence>" will be "/updateSequence".
      
      line = line.substr(1, line.length()-1);
      
      // Grab tag name.  Assume no whitespace between initial '<' and
      // tag name.
      
      tempString = line[0];
      line.erase(0,1);
      
      while ( line[0] != ' ' && line[0] != '\t' && line[0] != '>'
            && line.substr(0, 2) != "/>" ) {
         
         tempString += line[0];
         line.erase(0,1);
      }
      
      currentTag.tagName = tempString;
      
      // While there are more attribute/value pairs in the input line,
      // read them into our tag structure.
      
      while ( moreAttributeData(line) ) {
         
         getNextPair(line, currentTag);
      }
      
      xmlTags.push_back(currentTag);
      
      stringIter++;
   }
   
   // Now that we've got a vector of xmlTags, squidge it into chunks:
   //    
   //    graph declarations
   //    update sequences
   //    graph snapshots
   // 
   // and pass each chunk to the appropriate subprocessor.
   
   xmlTag tempTag;
   vector<xmlTag> chunkVector;
   
   while ( xmlTags.size() > 0 ) {
      
      chunkVector.clear();
      
      // Scan the first tag and delete it from the main vector.
      
      tempTag = xmlTags[0];
      xmlTags.erase(xmlTags.begin());
      
      if ( tempTag.tagName == "graph" ) {
         
         // Scrap everything and create a new graph with the desired
         // parameters.  It'd better be an undirected multiGraph, or we'll
         // barf all over the user.
         
         if ( tempTag.directed != 0 || tempTag.multi != 1 ) {
            
            cout << "Wrong type of graph in input file.  Exiting.\n";
            exit(1);
            
         } else if ( tempTag.graphType == '\0' ) {
            
            // Create a graph with nodeSize nodes.  Emulate the appropriate
            // constructor.
            
            G = new multiGraph(tempTag.nodeSize);
         
            nodes = &(G->nodes);
            edges = &(G->edges);
            
            int was_recording = recording;
            
            if ( was_recording ) {
               pause();
            }
            
            output_file_stream << "</graph>\n"
                  << "<graph nodeSize=\"" << tempTag.nodeSize
            	   << "\" directed=\"0\" multi=\"1\">\n";
               
            if ( was_recording ) {
               record();
            }
            
         } else {
            
            // Create a graph with nodeSize nodes, of type graphType.
            // Emulate the appropriate constructor.
            
            G = new multiGraph(tempTag.graphType, tempTag.nodeSize);
         
            nodes = &(G->nodes);
            edges = &(G->edges);
            
            int was_recording = recording;
            
            if ( was_recording ) {
               pause();
            }
            
            output_file_stream << "</graph>\n"
               << "<graph nodeSize=\"" << tempTag.nodeSize
               << "\" directed=\"0\" multi=\"1\" graphType=\""
               << tempTag.graphType << "\">\n";
               
            if ( was_recording ) {
               record();
            }
         }
         
      } else if ( tempTag.tagName == "updateSequence" ) {
         
         // Use chunkVector to create an updateSequence, then pass
         // it to processUpdates().
         
         // Until we reach the end, grab all update commands.
         
         while ( xmlTags[0].tagName != "/updateSequence" ) {
            
            chunkVector.push_back(xmlTags[0]);
            xmlTags.erase(xmlTags.begin());
         }
         
         // Delete the closing </updateSequence> tag.
         
         xmlTags.erase(xmlTags.begin());
         
         // Execute the updates in sequence.
         
         processUpdates(chunkVector);
         
      } else if ( tempTag.tagName == "snapshot" ) {
         
         // Use chunkVector to create a graph-snapshot sequence, then pass
         // it to processSnapshot().
      
         // Until we reach the end, grab all snapshot elements.
         
	 vector<xmlTag>::iterator xmlIter = xmlTags.begin();
	 
	 while ( (*xmlIter).tagName != "/snapshot" ) {
	    
	    xmlTag tempTag = *xmlIter;

            chunkVector.push_back(tempTag);

	    xmlIter++;
         }
         
	 // Delete the tags we just copied into chunkVector.
	 
	 xmlTags.erase(xmlTags.begin(), xmlIter);
	 
         // Delete the closing </snapshot> tag.
         
         xmlTags.erase(xmlTags.begin());
         
         // Reconstruct the snapshot.
         
         processSnapshot(chunkVector);
         
      } else if ( tempTag.tagName == "/graph" ) {
         
         // Skip it, it's a non-action tag.
      }
   
   } // end while (we're scanning all tags in the input file)
   
   input_file_stream.close();
}

// Write a snapshot of the current graph to the output file.

void multiGraphManager::snapshot( void ) {

   int was_recording = recording;

   if ( was_recording ) {
      pause();
   }

   // Write the structural snapshot.

   output_file_stream << "<snapshot>\n";

   node_hash_t::iterator node_iterator;
   edge_hash_t::iterator edge_iterator;

   multiNode* node_ptr;
   multiEdge* edge_ptr;

   for ( node_iterator = G->nodes.begin(); node_iterator != G->nodes.end();
	      node_iterator++ ) {

	 node_ptr = node_iterator->second;
	 output_file_stream << "   <node ID=\"" << node_ptr->getId()
	    << "\" color=\"" << node_ptr->getColor()
	    << "\" weight=\"" << node_ptr->getWeight()
	    << "\"/>\n";
   }

   for ( edge_iterator = G->edges.begin(); edge_iterator != G->edges.end();
	      edge_iterator++ ) {

	 edge_ptr = edge_iterator->second;
	 output_file_stream << "   <edge ID=\"" << edge_ptr->getId()
	    << "\" color=\"" << edge_ptr->getColor()
	    << "\" weight=\"" << edge_ptr->getWeight()
	    << "\" nodeID_1=\"" << edge_ptr->getNode1()
	    << "\" nodeID_2=\"" << edge_ptr->getNode2()
	    << "\"/>\n";
   }

   output_file_stream << "</snapshot>\n";

   // Resume recording, if we were originally.

   if ( was_recording ) {
      record();
   }
}

// [Private member functions]

// Input parsing functions called by processFile().  Change argument
// type to reflect XML parse tree, or whatever internal data structure
// ends up representing the appropriate chunk of input data.
//
// *****************************
// DEPRECATED FOR THE TIME BEING
// *****************************

void multiGraphManager::processUpdates( vector<xmlTag>& updates ) {
   
   xmlTag tempTag;
   
   // Execute each update in the sequence provided.
   
   while ( updates.size() > 0 ) {
      
      tempTag = updates[0];
      updates.erase(updates.begin());
      
      if ( tempTag.tagName == "addNode" ) {
         
         this->addNode();
      
      } else if ( tempTag.tagName == "deleteNode" ) {
         
         this->deleteNode(tempTag.ID);
         
      } else if ( tempTag.tagName == "deleteRandomNode" ) {
         
         // If extraStructureInfo was set to 'on,' delete the exact node
         // indicated, otherwise replicate the appropriate function.
         
         if ( tempTag._ID != -1 ) {
            
            this->deleteNode(tempTag._ID);
         
         } else if ( tempTag.attribute != '\0' ) {
            
            this->deleteRandomNode(tempTag.attribute, tempTag.value);
            
         } else {
            
            this->deleteRandomNode();
         }
         
      } else if ( tempTag.tagName == "addEdge" ) {
         
         this->addEdge(tempTag.node_1, tempTag.node_2);
         
      } else if ( tempTag.tagName == "addRandomEdge" ) {
         
         // If extraStructureInfo was set to 'on,' add the exact edge
         // indicated, otherwise replicate the appropriate function.
         
         if ( tempTag._node_2 != -1 ) {
            
            if ( tempTag._node_1 == -1 ) {
               this->addEdge(tempTag.node_1, tempTag._node_2);
            } else {
               this->addEdge(tempTag._node_1, tempTag._node_2);
            }
            
         } else if ( tempTag.attribute_1 != -1 ) {
            
            // (attribute_1, value_1, attribute_2, value_2 version.)
            
            this->addRandomEdge(tempTag.attribute_1, tempTag.value_1,
                     tempTag.attribute_2, tempTag.value_2);
            
         } else if ( tempTag.attribute == '\0' && tempTag.node_1 == -1 ) {
            
            // void version.
            
            this->addRandomEdge();
            
         } else if ( tempTag.attribute == '\0' ) {
            
            // connect-to-specified-node version.
            
            this->addRandomEdge(tempTag.node_1);
         
         } else if ( tempTag.node_1 == -1 ) {
            
            // (attribute, value) version.
            
            this->addRandomEdge(tempTag.attribute, tempTag.value);
         
         } else {
            
            // (node_1, attribute, value) version.
            
            this->addRandomEdge(tempTag.node_1,
                     tempTag.attribute, tempTag.value);
         
         } // end switch on version of addRandomEdge
         
      } else if ( tempTag.tagName == "addRandomNewEdge" ) {
         
         // If extraStructureInfo was set to 'on,' add the exact edge
         // indicated, otherwise replicate the appropriate function.
         
         if ( tempTag._node_2 != -1 ) {
            
            this->addEdge(tempTag._node_1, tempTag._node_2);
            
         } else {
            
            this->addRandomNewEdge(tempTag.attribute_1, tempTag.value_1,
                     tempTag.attribute_2, tempTag.value_2);
         }
         
      } else if ( tempTag.tagName == "deleteEdge" ) {
         
         if ( tempTag.ID != -1 ) {
            
            this->deleteEdge(tempTag.ID);
            
         } else {
            
            this->deleteEdge(tempTag.node_1, tempTag.node_2);
         }
         
      } else if ( tempTag.tagName == "deleteRandomEdge" ) {
         
         // If we've got specific endpoints, Just Do It.
         
         if ( tempTag._node_2 != -1 ) {
            
            this->deleteEdge(tempTag._node_1, tempTag._node_2);
         
         } else if ( tempTag.node == -1 && tempTag.attribute == '\0' ) {
            
            // void version.
            
            this->deleteRandomEdge();
            
         } else if ( tempTag.node == -1 ) {
            
            // (attribute, value) version.
            
            this->deleteRandomEdge(tempTag.attribute, tempTag.value);
            
         } else if ( tempTag.node != -1 ) {
            
            // (delete-from-targetNode) version.
            
            this->deleteRandomEdge(tempTag.node);
            
         } // end switch on version of deleteRandomEdge
         
      } // end switch on tag name
   
   } // end iterator over all tags in update sequence
}

void multiGraphManager::processSnapshot( vector<xmlTag>& snapshot ) {
   
   // Reset our internal graph.
   
   G = new multiGraph();
   
   nodes = &(G->nodes);
   edges = &(G->edges);
   
   xmlTag tempTag;
   
   // Create each graph object in the sequence provided.
   
   vector<xmlTag>::iterator xmlIter = snapshot.begin();
   
   while ( xmlIter != snapshot.end() ) {
      
      tempTag = *xmlIter;
      
      if ( tempTag.tagName == "node" ) {
         
         G->setNodeIndex(tempTag.ID);
         G->addNode();
         G->setNodeColor(tempTag.ID, tempTag.color);
         G->setNodeWeight(tempTag.ID, tempTag.weight);
         
      } else if ( tempTag.tagName == "edge" ) {
         
         G->setEdgeIndex(tempTag.ID);
         G->addEdge(tempTag.nodeID_1, tempTag.nodeID_2);
         G->edges[tempTag.ID]->setColor(tempTag.color);
         G->edges[tempTag.ID]->setWeight(tempTag.weight);
         
      } // end switch on tag name
      
      xmlIter++;

   } // end iterator over all tags in snapshot
   
   if ( recording ) {
      this->snapshot();
   }
}

// Is there more attribute data in a given tag?  Assume the tag name
// has already been removed, so everything left should be properly paired.

bool moreAttributeData( string& line ) {
   
   bool moreData = false;
   
   // Kill initial whitespace.
   
   while ( line[0] == ' ' || line[0] == '\t' ) {
      line.erase(0,1);
   }
   
   if (  line[0] != '>' && line.substr(0, 2) != "/>" ) {
      
      moreData = true;
   }
   
   return moreData;
}

// Get next attribute/value pair from a given tag into an 'xmlTag' struct.
// Assume such a pair exists and is at the immediate beginning of the string.

void getNextPair( string& line, xmlTag& tag ) {
   
   string attributeName = "";
   string value = "";
   
   // Grab the attribute name.
   
   while ( line[0] != '=' ) {
      
      attributeName += line[0];
      line.erase(0, 1);
   }
   
   // Erase the equals-sign & quote mark before the value.
   
   line.erase(0, 1);
   line.erase(0, 1);
   
   // Get the value.
   
   while ( line[0] != '\"' ) {
      
      value += line[0];
      line.erase(0, 1);
   }
   
   // Erase the quote mark after the value.
   
   line.erase(0, 1);
   
   // Insert the value, typed appropriately, into the proper field
   // in the xmlTag structure.
   
   if ( attributeName == "attribute" ) {
      
      tag.attribute = value[0];
      
   } else if ( attributeName == "attribute_1" ) {
      
      tag.attribute_1 = value[0];
      
   } else if ( attributeName == "attribute_2" ) {
      
      tag.attribute_2 = value[0];
      
   } else if ( attributeName == "graphType" ) {
      
      tag.graphType = value[0];
      
   } else if ( attributeName == "color" ) {
      
      tag.color = atoi(value.c_str());
      
   } else if ( attributeName == "ID" ) {
      
      tag.ID = atoi(value.c_str());
      
   } else if ( attributeName == "_ID" ) {
      
      tag._ID = atoi(value.c_str());
      
   } else if ( attributeName == "node_1" ) {
      
      tag.node_1 = atoi(value.c_str());
      
   } else if ( attributeName == "_node_1" ) {
      
      tag._node_1 = atoi(value.c_str());
      
   } else if ( attributeName == "node_2" ) {
      
      tag.node_2 = atoi(value.c_str());
      
   } else if ( attributeName == "_node_2" ) {
      
      tag._node_2 = atoi(value.c_str());
      
   } else if ( attributeName == "nodeID_1" ) {
      
      tag.nodeID_1 = atoi(value.c_str());
      
   } else if ( attributeName == "nodeID_2" ) {
      
      tag.nodeID_2 = atoi(value.c_str());
      
   } else if ( attributeName == "nodeSize" ) {
      
      tag.nodeSize = atoi(value.c_str());
      
   } else if ( attributeName == "directed" ) {
      
      tag.directed = atoi(value.c_str());
      
   } else if ( attributeName == "multi" ) {
      
      tag.multi = atoi(value.c_str());
      
   } else if ( attributeName == "value" ) {
      
      tag.value = atof(value.c_str());
      
   } else if ( attributeName == "value_1" ) {
      
      tag.value_1 = atof(value.c_str());
      
   } else if ( attributeName == "value_2" ) {
      
      tag.value_2 = atof(value.c_str());
      
   } else if ( attributeName == "weight" ) {
      
      tag.weight = atof(value.c_str());
   }
}

// Seed some default values to facilitate structure discovery later on.

void initTag( xmlTag& tag ) {
   
   tag.attribute = '\0';
   tag.attribute_1 = '\0';
   tag.attribute_2 = '\0';
   tag.graphType = '\0';
   
   tag.color = -1;
   tag.ID = -1;
   tag.node = -1;
   tag._ID = -1;
   tag.node_1 = -1;
   tag._node_1 = -1;
   tag.node_2 = -1;
   tag._node_2 = -1;
   tag.nodeID_1 = -1;
   tag.nodeID_2 = -1;
   tag.nodeSize = -1;

   tag.directed = -1;
   tag.multi = -1;
   
   // These'll be picked up only if the 'attribute*' fields contain
   // relevant data, so their seed values don't matter.
   
   tag.value = 0;
   tag.value_1 = 0;
   tag.value_2 = 0;
   
   // Set to a hopefully unlikely value to serve later as a marker for
   // whether or not this has been set.
   
   tag.weight = WEIGHT_INIT;
}
