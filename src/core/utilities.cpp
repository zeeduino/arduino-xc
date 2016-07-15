/*
  utilitites.c - misc utility functions
  Copyright (c) 2016 Ravendyne Inc.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "board/utilities.h"

uint32_t mapBits(uint32_t value, uint32_t from, uint32_t to)
{
    // mask out only bits that are significant according to 'from' parameter
    value = value & ((1<<from) - 1);
    if (from == to)
        return value;
    if (from > to)
        return value >> (from - to);
    else
        return value << (to - from);
}


// Taken from Stream class
long parseInt(const char *buffer, char skipChar)
{
  bool isNegative = false;
  long value = 0;
  int c;

  c = *buffer;
  if(0 == c)
  {
	  return value;
  }

  do
  {
    if(c == skipChar)
      ; // ignore this charactor
    else if(c == '-')
      isNegative = true;
    else if(c >= '0' && c <= '9')        // is c a digit?
      value = value * 10 + c - '0';
    c = *++buffer;
  } while( (c >= '0' && c <= '9') || c == skipChar );

  if(isNegative)
    value = -value;
  return value;
}

// given skipChar is ignored
// this allows format characters (typically commas) in values to be ignored
// Taken from Stream class
float parseFloat(const char *buffer, char skipChar)
{
  bool isFraction = false;
  bool isNegative = false;
  long value = 0;
  int c;
  float fraction = 1.0;

  c = *buffer;
  if(0 == c)
  {
	  return value;
  }

  do
  {
    if(c == skipChar)
      ; // ignore
    else if(c == '-')
      isNegative = true;
    else if (c == '.')
      isFraction = true;
    else if(c >= '0' && c <= '9')  {      // is c a digit?
      value = value * 10 + c - '0';
      if(isFraction)
         fraction *= 0.1;
    }
    c = *++buffer;
  } while( (c >= '0' && c <= '9')  || c == '.' || c == skipChar );

  if(isNegative)
    value = -value;
  if(isFraction)
    return value * fraction;
  else
    return value;
}


