/*
  utilitites.h - misc utility functions
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

#ifndef UTILITIES_H_
#define UTILITIES_H_

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

uint32_t mapBits(uint32_t value, uint32_t from, uint32_t to);
long parseInt(const char *buffer, char skipChar);
float parseFloat(const char *buffer, char skipChar);

#endif /* UTILITIES_H_ */
