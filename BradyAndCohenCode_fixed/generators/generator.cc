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


typedef hash_map<int, multiNode*> node_hash_t;
typedef hash_map<int, multiEdge*> edge_hash_t;


void indexComponents(multiGraphManager* mgm,
      hash_map<int, hash_set<int> >& componentHash);

void countHighestDegreeNodes(multiGraphManager* mgm, int N, 
      queue<int>& highestDegreeNodes);

void extractComponent(multiGraphManager* mgm, int index, 
      hash_map<int, hash_set<int> > components);

void extractNeighborhood(multiGraphManager* mgm, int centerID, int size);

bool isForest(multiGraphManager* mgm);

void countComponentsWithCycles(multiGraphManager* mgm, 
      hash_map<int, hash_set<int> > components,
      hash_map<int, int>& cycleComponents);

template <class T>
string to_string(T t) {
   
      ostringstream oss;
      oss << t;
      return oss.str();
}

int main() {
   cout<< "start"<<endl;
   multiGraphManager* mgm;
   LuTopologyGenerator* ltg;
   
   int i;
   int dummyInt_1;
   int dummyInt_2;
   char* dummyString;
   
   char* final_filename;
   
   const char* outputfilename = "main.xml";
   
   string filename = "";
   
   
   // Node size for graph.
    int n = 90000;

   // Growth parameter for power-law generator.
   
   double beta = 2.9;
   
   // Number of random instances for each set of parameters.
   
   int filecount = 30;

   
   while ( beta < 3.0 ) {

      for ( i = 0; i < filecount; i++ ) {
	 
         // Make sure to take care of C++'s idiotically poorly-supported
         // num-to-string conversion.  Nothing this elementary should be
         // this hard.

         filename = "N_";
         filename += to_string<int>(n);
         filename += '_';
         filename += "beta_";

         dummyString = fcvt(beta, 2, &dummyInt_1, &dummyInt_2);

         filename += dummyString[0];
         filename += '.';
         filename += dummyString[1];

         filename += '_';
         
         if ( i + 1 < 10 ) {
            filename += to_string<int>(0);
         }
         
         filename += to_string<int>(i+1);
         filename += ".xml";

         // remove const-ness from filename.

         final_filename = (char *) filename.c_str();

         mgm = new multiGraphManager(n, final_filename);

         ltg = new LuTopologyGenerator(mgm, n, beta);

         mgm->snapshot();

         cout << "Finished file " << final_filename << ".\n";
        delete mgm;
      } // end for (each random instance at this setting)
      
      beta += 0.1000;
   }
      
//   delete mgm;

   return 0;
}
