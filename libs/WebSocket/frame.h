/**
 * WsHub
 * Copyright 2012 Stefano Tortarolo <stefano.tortarolo at gmail dot com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Library General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/** See RFC 6455 for details

8185d47a594f9c1f3523bb
Hello frames of this message follow

x81 x85
1000 0001 1000 0101


Mask
xd4 x7a x59 x4f


x9c x1f x35 x23 xbb

Example code:
  const int KEYS_SIZE = 4;
  int keys[4] = { 0xd4, 0x7a, 0x59, 0x4f};

  const int DATA_SIZE = 5;
  int data[5] = { 0x9c, 0x1f, 0x35, 0x23, 0xbb};

  int c = 0;
  int res = 0;
  for (c = 0; c < DATA_SIZE; c++) {
    res = data[c] ^ keys[c % 4];
    printf("%x - %c => %c\n", keys[c], keys[c], res);
  }

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct header {
  // First byte
  unsigned int frameFin : 1;
  unsigned int frameRsv1 : 1;
  unsigned int frameRsv2 : 1;
  unsigned int frameRsv3 : 1;
  unsigned int opCode : 4;

  // Second byte
  unsigned int frameMasked : 1;
  unsigned int framePayloadLength : 7;

  unsigned int extended;
};

struct frame {
  struct header* header;
  int *_chunks;
  int *_framePayloadData;
  int _dataOffset;
};

#define LENGTH_16 126
#define LENGTH_64 127

void setFirstByte(struct header*, int);
void setSecondByte(struct header *, int);
int* getChunkedData(const char*);
void construct(const char*);
int* slice(int *, int, int);
char encodeDecodeMask(int, int, int);
