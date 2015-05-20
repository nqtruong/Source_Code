//  polylineFunctions.c
//  googlePolylineTest
//
//  Created by James Snook on 21/04/2014.
//  Copyright (c) 2014 James Snook. All rights reserved.
//
#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "polylineFunctions.h"

void encodedIntValue (INT32 val, char *result, unsigned *length)
{
  bool isNeg = val < 0;
  /* Shift the value right by 1 to make room for the sign bit on the right 
     hand side. */
  val <<= 1;
  
  if (isNeg) {
    /* As the value is stored as a twos compliment value small values have a 
       lot of bits set so not the value. This will also flip the value of the
       sign bit so when we come to decode the value we will know that it is 
       negative. */
    val = ~val;
  }
  
  unsigned count = 0;
  
  do {
    /* get the smallest 5 bits from our value and add them to the charaters. */
    result[count] = val & 0x1f;
    
    /* We've saved the last 5 bits we can remove them from the value. We shift
       the value by 5 meaning that the next 5 bits that we need to save will be
       at the end of the value. */
    val >>= 5;
    
    if (val) {
      /* We have more bits to encode, so we need to set the continuation bit. */
      result[count] |= 0x20;
    }
    
    result[count] += 63;
    
    ++count;
  } while (val);

  if (!length)
    printf ("required value `length` not set in `encodeIntValue`");
  else
    *length = count;
}

char *copyEncodedLocationsString (Coordinate *coords, unsigned coordsCount)
{
  INT32 previousIntLat = 0;
  INT32 previousIntLng = 0;
  unsigned resultCount = 0;
  
  /* To ensure that we can encode one value we need at least 10 characters, 
     which is why we plus 7. */
  unsigned resultLength = 3 * coordsCount + 7;
  /* Hopefully this should be enough that we don't have to allocate more data
     later. */
  char *result = (char*)malloc (resultLength);
  
  
  INT32 intLat;
  INT32 intLng;
  
  unsigned encodedLen;
  
  for (unsigned i = 0; i < coordsCount; ++i) {
    /* Convert the current latitude and longitude to their integer 
       representation. */
	  
    intLat = (INT32)(coords[i].latitude * 1e5);
    intLng = (INT32)(coords[i].longitude * 1e5);
    
    /* Encode the difference between the current integer representation, and
       the previous integer representaion. */
    encodedIntValue (intLat - previousIntLat,
                     result + resultCount,
                     &encodedLen);
    resultCount += encodedLen;
    
    /* Then do the same for the latitudes. */
    encodedIntValue (intLng - previousIntLng,
                     result + resultCount,
                     &encodedLen);
    resultCount += encodedLen;
    
    previousIntLat = intLat;
    previousIntLng = intLng;
    
    if (resultLength - resultCount < 10) {
      /* In this case we may overfun our results buffer, so we need to allocate
       more memory. */
      resultLength += (coordsCount - i) * 3 + 7;
      result = (char*)realloc (result, resultLength);
    }
  }
  
  result[resultCount] = '\0';
  result = (char *)realloc(result, resultCount + 1);
  
  return result;
}

INT32 decodeDifferenceVal (char *chars, unsigned *usedChars)
{
  unsigned i = 0;
  INT32 result = 0;
  char currentByte;
  
  do {
    currentByte = chars[i] - 63;
    result |= (INT32)(currentByte & 0x1f) << (5 * i);
    ++i;
  } while (currentByte & 0x20);
  
  if (result & 1) {
    result = ~result;
  }
  
  result >>= 1;
  
  *usedChars = i;
  return result;
}

Coordinate *decodeLocationsString (char *polylineString, unsigned *locsCount)
{
  int charsCount = strlen (polylineString);
  if (!charsCount)
    return NULL;
  
  unsigned resultLength = charsCount / 3 + 2;
  Coordinate *result = ( Coordinate *)malloc (sizeof(Coordinate) * resultLength);
  
  unsigned totalUsedChars = 0;
  unsigned usedChars = 0;
  unsigned resultCount = 0;
  
  INT32 intLat = 0;
  INT32 intLng = 0;
  
  do {
    intLat += decodeDifferenceVal (polylineString + totalUsedChars, &usedChars);
    totalUsedChars += usedChars;
    
    intLng += decodeDifferenceVal (polylineString + totalUsedChars, &usedChars);
    totalUsedChars += usedChars;
    
    result[resultCount].latitude  = intLat * 1e-5;
    result[resultCount].longitude = intLng * 1e-5;
    
    ++resultCount;
    
    if (resultCount >= resultLength - 1) {
      resultLength += (charsCount - totalUsedChars) / 3 + 2;
      result = ( Coordinate *)realloc (result, resultLength * sizeof(Coordinate));
    }
    
  } while (totalUsedChars != charsCount);
  
  result = ( Coordinate *)realloc (result, resultCount * sizeof(Coordinate));
  *locsCount = resultCount;
  return result;
}
