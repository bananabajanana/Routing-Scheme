// LuTopologyGenerator.h: Specification for the Lu Topology Generator

#ifndef _LU_TOPOLOGY_GENERATOR_H
#define _LU_TOPOLOGY_GENERATOR_H

#include <iostream>
#include <vector>
#include <string>

#include "../multi/multiGraphManager.h"

using namespace std;

class LuTopologyGenerator {

   public:

      // ------------
      // Constructor.
      // ------------

      LuTopologyGenerator( multiGraphManager* graphArg, int n, double beta );

   private:

      // -------------------------
      // Private Member Variables.
      // -------------------------

      multiGraphManager* outputGraph;
      int n;
      int r;
      double alpha;
      double beta;

      // degreeDistribution[i] is the number of nodes with degree i.
      // degreeSequence[i] is the degree of node i in the output graph.

      vector<int> degreeDistribution;
      vector<int> degreeSequence;

      // -------------------------
      // Private Member Functions.
      // -------------------------

      // Calculate alpha minimizing:
      //
      //    min( abs( n - sum( floor( e^alpha / k^beta ) ) )

      void calculate_alpha();

      // Calculate the remainder value.

      void calculate_r();

      // Generate a valid degree distribution.
      //
      //    #{i | k <= wi < k+1 } = floor((e^alpha / pow((double)i, beta)))

      void generateDegreeDistribution();

      // Generate a degree sequence from the distribution.

      void generateDegreeSequence();

      // Generate the graph using the constructed degree sequence.

      void generateGraph();

      // Calculate a specific sum for a given value of alpha.

      int calculate_alphaSum( double alphaArg );

      // Minimize alpha within a given range.

      void minimize( double alphaLower, double alphaUpper);

};

#endif
