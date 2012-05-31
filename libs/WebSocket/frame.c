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
#include "frame.h"

void construct(const char *data) {
  struct frame* _frame = malloc(sizeof(struct frame*));
  _frame->header = malloc(sizeof(struct header*));

  printf("Inizio split di %s\n", data);
  _frame->_chunks = getChunkedData(data);

  // Expand first two bytes
  setFirstByte(_frame->header, _frame->_chunks[0]);
  setSecondByte(_frame->header, _frame->_chunks[1]);

  printf("--- FRAME\n");
  printf("frameFin: %d\n", _frame->header->frameFin);
  printf("frameRsv1: %d\n", _frame->header->frameRsv1);
  printf("frameRsv2: %d\n", _frame->header->frameRsv2);
  printf("frameRsv3: %d\n", _frame->header->frameRsv3);
  printf("Op Code: %d\n", _frame->header->opCode);
  printf("Masked: %d\n", _frame->header->frameMasked);
  printf("Payload Length: %d\n", _frame->header->framePayloadLength);

  // check Extended
  if (_frame->header->extended >= LENGTH_16) {
    // TODO: handle extended frames
  }

  // check Masked
  if (_frame->header->frameMasked) {
    // TODO: handle masked frames
  }

  _frame->_framePayloadData = slice(_frame->_chunks, _frame->_dataOffset, _frame->header->framePayloadLength);
  printf("%s\n", _frame->_framePayloadData);
  /* TODO: implement decoding
  int *dataPtr = _frame->_framePayloadData;

  int _dataByte;
  int c = 0;
  while(dataPtr != NULL) {
    printf("%c", encodeDecodeMask(*dataPtr, _frame->header->frameMasked, c));
    c++;
    dataPtr++;
  }*/
}

char encodeDecodeMask(int _char, int masked, int offset) {
  // TODO: implement encode/decode when masked
  return _char;
}

int* slice(int *chunks, int offset, int payloadLength) {
  printf("Allocating %d\n", payloadLength);
  int *data = malloc(payloadLength * sizeof(int));

  int c = 0;
  for (c = 0; c < payloadLength; c++) {
    data[c] = chunks[c + offset];
  }
  return data;
}

void setFirstByte(struct header *head, int value) {
  printf("1. VALUE %d\n", value);
  head->frameFin = (value >> 8) & 1;
  head->frameRsv1 = (value >> 7) & 1;
  head->frameRsv2 = (value >> 6) & 1;
  head->frameRsv3 = (value >> 5) & 1;
  printf("1. %d\n", value >> 4);
  head->opCode = value >> 4;
}

void setSecondByte(struct header *head, int value) {
  printf("2. VALUE %d\n", value);
  head->frameMasked = (value >> 8) & 1;
  printf("2. VALUE %d\n", value >> 7);
  head->framePayloadLength = value >> 7;
  printf("2. Len: %d\n", head->framePayloadLength);

  if (head->framePayloadLength == LENGTH_16) {
    printf("Extended 126\n");
    head->extended = LENGTH_16;
  } else if (head->framePayloadLength == LENGTH_64) {
    printf("Extended 127\n");
    head->extended = LENGTH_64;
  }
}

int* getChunkedData(const char *data) {
  int c = 0;
  int *chunks = (int *) malloc(strlen(data) * sizeof(int));

  for (c = 0; c < strlen(data); c++) {
    chunks[c] = (int) data[c];
    printf("Chunk %d: %x, %x\n", c, chunks[c], data[c]);
  }
  return chunks;
}

void main(int argc, char **argv) {
  construct(argv[1]);
}
