// util.cc:  Definitions for miscellaneous utility functions.

#include "util.h"

#include <time.h>
#include "rand/randomc.h"


// Return a random integer between 0 and INT_MAX.

int randomNumber() {

   // Instantiate a "Mersenne twister" generator with a timestamp seed.

   static TRandomMersenne generatorInstance(time(0));

   // Get an integer in the specified range & return it.

   int value = generatorInstance.IRandom(0, (INT_MAX - 1));

   return(value);
}

// Test two floating-point numbers for [in]equality within a system-dependent
// [small] range, and sidestep potential floating-point errors.

bool areEqual( double a, double b ) {

   if ( ((-10 * FLT_EPSILON) < (a - b)) && ((a - b) < (FLT_EPSILON * 10)) ) {
      return true;
   } else {
      return false;
   }
}

bool isLess( double a, double b ) {

   if ( !areEqual(a, b) && (a < b) ) {
      return true;
   } else {
      return false;
   }
}

bool isGreater( double a, double b ) {

   if ( !areEqual(a, b) && (a > b) ) {
      return true;
   } else {
      return false;
   }
}

bool isLessEqual( double a, double b ) {

   if ( areEqual(a, b) ) {
      return true;
   }

   if ( isLess(a, b) ) {
      return true;
   } else {
      return false;
   }
}

bool isGreaterEqual( double a, double b ) {

   if ( areEqual(a, b) ) {
      return true;
   }

   if ( isGreater(a, b) ) {
      return true;
   } else {
      return false;
   }
}


// Return an iterator to the given position of a list.  [Positions start
// at 1, not zero.]

list<int>::iterator listPosition( int position, list<int>* const targetList ) {

   list<int>::iterator iter;
   int count;

   if ( position > targetList->size() ) {
      return targetList->end();
   }

   iter = targetList->begin();

   for (count = 0; count < position; count++) {
      iter++;
   }

   return iter;
}


/*
Sorts the first array argument in increasing order, then reorders the
second array based on its original correspondence to the first, e.g.:

      ARRAY1: { 3, 2, 8, 6 )
      ARRAY2: { 0, 1, 2, 3 )

      <call correspondenceSort( ARRAY1, ARRAY2 )>

      ARRAY1: { 2, 3, 6, 8 }
      ARRAY2: { 1, 0, 3, 8 }

   Notes:

      A _critical_ assumption is that all values in each array are distinct.

      No error-checking [e.g. valid ranges, sizeof(a) == size == sizeof(b),
      etc.] is happening here.
*/

void correspondenceSort( int a[], int b[], int size) {

//   hash_map<int, int> correspondenceMap;
    __gnu_cxx::hash_map<int, int> correspondenceMap;


    int count;

   // First, save b's values - keyed by their corresponding values in
   // a - in our temporary hash.

   for ( count = 0; count < size; count++ ) {
      correspondenceMap[a[count]] = b[count];
   }

   // Next, sort a.  Efficiently.  [Native STL algorithm.]

   sort(a, a + size);

   // Lastly, rearrange b by dumping the values from our temporary hash back
   // in, using a's new order as a guide.

   for ( count = 0; count < size; count++ ) {
      b[count] = correspondenceMap[a[count]];
   }
}


/*
   Searches sortedArray[first]..sortedArray[last] for <key>.
   Returns the index in sortedArray of the matched key,
   or, should the match fail, the indices between which the key
   would appear, e.g.:

      sortedArray: { 1, 2, 3, 4 }
      indices:       0, 1, 2, 3

      key: 3 [exact match possible]
      result: { 2, -1, -1 }

      sortedArray: { 1, 2, 4, 5 }
      indices:       0, 1, 2, 3

      key: 3 [exact match not possible]
      result: { -1, 1, 2 }

   Note:

      A critical assumption is that the key being looked up is
      between the values sortedArray[first] and sortedArray[last].
*/

void binarySearch(double sortedArray[], int result[],
                     int first, int last, double key) {

   bool found = false;
   result[0] = -1;
   result[1] = 0;
   result[2] = 0;

   int mid;

   while (first <= last) {

       // Calculate [the floor of] the midpoint of our current search range.

       mid = (first + last) / 2;

       // If we're done, we're done.
       // Otherwise, check to see which half of the remaining range
       // needs to be searched.

       if ( isGreater(key, sortedArray[mid]) ) {
           first = mid + 1;
       } else if ( isLess(key, sortedArray[mid]) ) {
           last = mid - 1;
       } else {
           result[0] = mid;
           found = true;
           break;
       }
   }

   if (!found) {

      result[0] = -1;

      while ( isGreater(sortedArray[mid], key) ) {
         mid--;
      }

      result[1] = mid;
      result[2] = mid+1;
   }
}
