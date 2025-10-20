#include <stdio.h>
#include <stdlib.h>

#include "chunk.h"

unsigned char first_8bits(unsigned char block, int n) {
  unsigned char uc_n = (unsigned char)n;
  const int nr_1 = 6;
  const int nr_2 = 0b11111;
  return (block << nr_1) | (uc_n & nr_2);
}

void first_16bits(unsigned char block, int n, unsigned char* bit_sequence) {
  const int nr_1 = 6;
  const int nr_2 = 2;
  const int nr_3 = 4;
  const int nr_4 = 8;
  const int nr_5 = 15;
  const int nr_6 = 255;
  bit_sequence[0] = (block << nr_1) | (nr_2 << nr_3) | ((n >> nr_4) & nr_5);
  bit_sequence[1] = n & nr_6;
}

unsigned char* chunk_encode(char*** chunk, int width, int height, int depth,
                            int* length) {
  const int size = width * height * depth;
  char* elements = calloc(size, sizeof(char));
  unsigned char* vector_of_bits =
      (unsigned char*)malloc(4 * size * sizeof(unsigned char));
  int k = 0, index = 0;

  for (int y = 0; y < height; y++) {
    for (int z = 0; z < depth; z++) {
      for (int x = 0; x < width; x++) {
        elements[k++] = chunk[x][y][z];
      }
    }
  }
  char block_current = elements[0];
  int sequence_length = 1;
  const int nr_1 = 4095;
  const int nr_2 = 32;

  for (int i = 1; i < k; i++) {
    if (elements[i] == block_current && sequence_length < nr_1) {
      sequence_length++;
    } else {  // cand se opreste
      int current = sequence_length;
      if (current < nr_2) {
        unsigned char bit = 0;
        bit = first_8bits(block_current, current);
        vector_of_bits[index++] = bit;
      } else {
        unsigned char* bits_sequence =
            (unsigned char*)malloc(2 * sizeof(unsigned char));
        first_16bits(block_current, current, bits_sequence);
        vector_of_bits[index++] = bits_sequence[0];
        vector_of_bits[index++] = bits_sequence[1];
        free(bits_sequence);
      }
      block_current = elements[i];
      sequence_length = 1;
    }
  }
  int current = sequence_length;
  if (current < nr_2) {
    unsigned char bit = 0;
    bit = first_8bits(block_current, current);
    vector_of_bits[index++] = bit;
  } else {
    unsigned char* bits_sequence =
        (unsigned char*)malloc(2 * sizeof(unsigned char));
    first_16bits(block_current, current, bits_sequence);
    vector_of_bits[index++] = bits_sequence[0];
    vector_of_bits[index++] = bits_sequence[1];
    free(bits_sequence);
  }
  free(elements);
  *length = index;
  return vector_of_bits;
}

char*** chunk_decode(unsigned char* code, int width, int height, int depth) {
  int size = width * height * depth;
  char*** chunk = (char***)malloc(width * sizeof(char**));
  for (int i = 0; i < width; i++) {
    chunk[i] = (char**)malloc(height * sizeof(char*));
    for (int j = 0; j < height; j++) {
      chunk[i][j] = (char*)malloc(depth * sizeof(char));
      for (int t = 0; t < depth; t++) {
        chunk[i][j][t] = 0;
      }
    }
  }
  int index = 0;
  int block_current = 0;
  while (block_current < size) {
    const int n_1 = 6;
    const int n_2 = 0b11;
    const int n_3 = 4;
    const int n_4 = 0x03;
    const int n_5 = 0b11111;
    const int n_6 = 0b1111;
    const int n_7 = 8;
    unsigned char bit = code[index];
    int block_type = (bit >> n_1) & n_2;
    int type = (bit >> n_3) & n_4;
    if (type == 0 || type == 1) {
      int lenght = bit & n_5;
      for (int j = 0; j < lenght; j++) {
        int y = block_current / (depth * width);
        int rest = block_current % (depth * width);
        int z = rest / width;
        int x = rest % width;
        chunk[x][y][z] = (char)block_type;
        block_current++;
      }
      index++;
    } else if (type == 2) {
      unsigned char bit2 = code[index + 1];
      int lenght = ((bit & n_6) << n_7) | bit2;

      index += 2;

      for (int j = 0; j < lenght; j++) {
        int y = block_current / (depth * width);
        int rest = block_current % (depth * width);
        int z = rest / width;
        int x = rest % width;
        chunk[x][y][z] = (char)block_type;
        block_current++;
      }
    }
  }
  return chunk;
}
