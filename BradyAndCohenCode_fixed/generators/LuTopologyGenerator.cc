// LuTopologyGenerator.cc: Implementation for the Lu Topology Generator

#include "LuTopologyGenerator.h"
#include <cmath>
#include <algorithm>
#include <time.h>
#include "../rand/randomc.h"

LuTopologyGenerator::LuTopologyGenerator( multiGraphManager* graphArg,
                                          int nArg, double betaArg ) {

   outputGraph = graphArg;
   n = nArg;
   beta = betaArg;

   calculate_alpha();

   calculate_r();

   generateDegreeDistribution();

   generateDegreeSequence();

   generateGraph();
}

void LuTopologyGenerator::calculate_alpha() {

   double currentAlpha = 0;
   double lastAlpha = 0;

   int currentAlphaSum = INT_MAX;
   int lastAlphaSum = INT_MAX;

   int exponent = 0;

   // Find the lower and upper bounds between which the minimum value must lie

   do {

      lastAlpha = currentAlpha;
      currentAlpha = pow( 2, exponent );

      lastAlphaSum = currentAlphaSum;
      currentAlphaSum = static_cast<int>(abs( n - calculate_alphaSum( currentAlpha ) ));

      // To ensure that we ignore the case when the sum equals 0

      if ( lastAlphaSum == 0 ) {

         lastAlphaSum = currentAlphaSum;

      }

      exponent++;

   } while ( lastAlphaSum >= currentAlphaSum );

   minimize( lastAlpha, currentAlpha );

}

void LuTopologyGenerator::calculate_r() {

   r = n - calculate_alphaSum(alpha);

}

void LuTopologyGenerator::generateDegreeDistribution() {

   double numerator = exp(alpha);
   int maxDegree = static_cast<int>( exp(alpha/beta) );
   int numNodes = 0;

   degreeDistribution.clear();

   // Let's avoid indexing errors and stuff in a value for 'degree 0'.

   degreeDistribution.push_back(numNodes);

   // #{i | 1 <= w_i < 2 } = floor(e^alpha) + r

   numNodes = static_cast<int>(numerator) + r;

   degreeDistribution.push_back(numNodes);

   // #{i | k <= w_i < k+1 } = floor(e^alpha / k^beta)

   for ( int i = 2; i <= maxDegree; i++ ) {

      numNodes = static_cast<int>( numerator * pow( (double) i, -beta ) );

      degreeDistribution.push_back(numNodes);
   }
}

void LuTopologyGenerator::generateDegreeSequence() {

   int currentDegree = degreeDistribution.size() - 1;
   int vertexCount = 0;

   // Record the desired degree for each vertex in the target graph.

   while ( currentDegree > 0 ) {

      while ( vertexCount < degreeDistribution[currentDegree] ) {

         degreeSequence.push_back(currentDegree);

         vertexCount++;
      }

      vertexCount = 0;
      currentDegree--;
   }
}

// Return a random integer in [0, N).

int randomNumberProducer( int N ) {

   // Instantiate a "Mersenne twister" generator with a timestamp seed.

   static TRandomMersenne generatorInstance_2(time(0));

   // Get an integer in the specified range & return it.

   int value = generatorInstance_2.IRandom(0, (N - 1));

   return(value);
}

void LuTopologyGenerator::generateGraph() {

   vector<int> v;

   for ( int currentVertex = 0;
         currentVertex < degreeSequence.size();
         currentVertex++ ) {
         
      v.resize(v.size() + degreeSequence[currentVertex], currentVertex);
   }

   random_shuffle(v.begin(), v.end(), randomNumberProducer);
   
   for ( int i = 0; i < v.size() / 2; i++ ) {
   
      outputGraph->addEdge(v[2 * i], v[2 * i + 1]);
   }
}



int LuTopologyGenerator::calculate_alphaSum( double alphaArg ) {

   // The total sum of the summation

   int sum = 0;

   // Where we sum up to

   int sumMax = static_cast<int>( exp(alphaArg/beta) );

   // The numerator of the fraction ( e^alpha )

   double numerator = exp(alphaArg);

   // Calculate the sum

   for (int k = 1; k <= sumMax; k++) {

         sum += static_cast<int>( numerator / pow( (double) k, beta ) );

   }

   return sum;
}

void LuTopologyGenerator::minimize( double lower, double upper ) {

   double midAlpha = (( lower + upper ) / 2);

   // Calculate the direction of the function at the current alpha
   // if we're within the threshhold

   if ( abs( upper - lower ) > 0.01 ) {

      // Take a point to the left

      double lowerAlpha = midAlpha - 0.1;

      // Take a point to the right

      double upperAlpha = midAlpha + 0.1;

      // Determine which way to go

      int lowerSum = calculate_alphaSum( lowerAlpha );
      int upperSum = calculate_alphaSum( upperAlpha );

      // Repeat

      if ( abs( n - lowerSum ) < abs( n - upperSum ) ) {

         minimize( lower, midAlpha );

      } else {

         minimize( midAlpha, upper );

      }

   }  // end if abs( upper - lower) > 0.01

   else {

      alpha = midAlpha;
   }

}
