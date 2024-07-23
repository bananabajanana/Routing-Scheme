// util.h:  Specification for miscellaneous utility functions.

#ifndef _UTIL_H
#define _UTIL_H

#include <string>
#include <list>
//#include <hash_map>
#include <backward/hash_map>

#include <limits.h>
#include <float.h>
#include <math.h>

#include <iostream>

using namespace std;
using namespace __gnu_cxx; // This might be necessary for older GCC versions

// ------------------
// Utility functions.
// ------------------

// Return a random integer between 0 and INT_MAX.

int randomNumber();

// Test two floating-point numbers for [in]equality within a specified
// [presumably small] range, and sidestep potential floating-point errors.

bool areEqual( double a, double b );

bool isLess( double a, double b );

bool isGreater( double a, double b );

bool isLessEqual( double a, double b );

bool isGreaterEqual( double a, double b );


// Return an iterator to the given position of a list.

list<int>::iterator listPosition( int position, list<int>* const targetList );


/*
Sorts the first array - within a specified range - in increasing order, then
re-aligns the second array to the new ordering on the first, e.g.:

      ARRAY1: { 3, 2, 8, 6 )
      ARRAY2: { 0, 1, 2, 3 )
   quickSort( ARRAY1, ARRAY2 );
      ARRAY1: { 2, 3, 6, 8 }
      ARRAY2: { 1, 0, 3, 8 }

   Note: no error-checking [e.g. valid ranges, sizeof(a) == sizeof(b), etc.]
   is happening here.  A key assumption is that all values in each array are
   distinct.
*/

void correspondenceSort( int a[], int b[], int size);


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
*/

void binarySearch(double sortedArray[], int result[],
                     int first, int last, double key);

#endif
