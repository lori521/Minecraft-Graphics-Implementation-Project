#include <stdlib.h>

#include "chunk.h"

char*** chunk_shell(char*** chunk, int width, int height, int depth,
                    char target_block, char shell_block) {
  char*** copy_of_chunk = (char***)malloc(width * sizeof(char**));
  for (int i = 0; i < width; i++) {
    copy_of_chunk[i] = (char**)malloc(height * sizeof(char*));
    for (int j = 0; j < height; j++) {
      copy_of_chunk[i][j] = (char*)malloc(depth * sizeof(char));
      for (int t = 0; t < depth; t++) {
        copy_of_chunk[i][j][t] = chunk[i][j][t];
      }
    }
  }
  const int nmax = 18;
  int x[] = {0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, 1, 1, 1, 1, 1};
  int y[] = {-1, -1, -1, 0, 0, 1, 1, 1, 0, -1, 1, 0, 0, 0, -1, 1, 0, 0};
  int z[] = {-1, 0, 1, -1, 1, -1, 0, 1, 0, 0, 0, -1, 1, 0, 0, 0, -1, 1};
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      for (int t = 0; t < depth; t++) {
        if (chunk[i][j][t] == target_block) {
          for (int k = 0; k < nmax; k++) {
            int i1 = i + x[k], j1 = j + y[k], t1 = t + z[k];
            if (i1 >= 0 && i1 < width && j1 >= 0 && j1 < height && t1 >= 0 &&
                t1 < depth) {
              if (chunk[i1][j1][t1] != target_block) {
                chunk_place_block(copy_of_chunk, width, height, depth, i1, j1,
                                  t1, shell_block);
              }
            }
          }
        }
      }
    }
  }
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      for (int t = 0; t < depth; t++) {
        chunk[i][j][t] = copy_of_chunk[i][j][t];
      }
    }
  }
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      free(copy_of_chunk[x][y]);
    }
    free(copy_of_chunk[x]);
  }
  free(copy_of_chunk);
  return chunk;
}

char*** chunk_fill_xz(char*** chunk, int width, int height, int depth, int x,
                      int y, int z, char block) {
  char block_copy = chunk[x][y][z];
  const int nmax = 4;
  int x_p[] = {-1, 0, 1, 0};
  int z_p[] = {0, 1, 0, -1};

  typedef struct {
    int x, y, z;
  } block_position;
  const int size = width * depth;
  block_position* queue = malloc(size * sizeof(block_position));
  int front = 0, rear = 0;
  queue[rear++] = (block_position){x, y, z};
  chunk_place_block(chunk, width, height, depth, x, y, z, block);
  while (front < rear) {
    block_position current_block = queue[front++];
    for (int k = 0; k < nmax; k++) {
      block_position neighbour_block = (block_position){
          current_block.x + x_p[k], current_block.y, current_block.z + z_p[k]};
      if (neighbour_block.x >= 0 && neighbour_block.x < width &&
          neighbour_block.y >= 0 && neighbour_block.y < height &&
          neighbour_block.z >= 0 && neighbour_block.z < depth) {
        if (chunk[neighbour_block.x][neighbour_block.y][neighbour_block.z] ==
            block_copy) {
          chunk_place_block(chunk, width, height, depth, neighbour_block.x,
                            neighbour_block.y, neighbour_block.z, block);
          queue[rear++] = neighbour_block;
        }
      }
    }
  }
  return chunk;
}

char*** chunk_fill(char*** chunk, int width, int height, int depth, int x,
                   int y, int z, char block) {
  char block_copy = chunk[x][y][z];
  const int nmax = 6;
  int x_p[] = {-1, 0, 1, 0, 0, 0};
  int y_p[] = {0, 0, 0, 0, -1, 1};
  int z_p[] = {0, 1, 0, -1, 0, 0};
  typedef struct {
    int x, y, z;
  } block_position;
  const int size = width * depth * height;
  block_position* queue = malloc(size * sizeof(block_position));
  int front = 0, rear = 0;
  queue[rear++] = (block_position){x, y, z};
  chunk_place_block(chunk, width, height, depth, x, y, z, block);
  while (front < rear) {
    block_position current_block = queue[front++];
    for (int k = 0; k < nmax; k++) {
      block_position neighbour_block =(block_position){current_block.x + x_p[k], current_block.y + y_p[k],
                           current_block.z + z_p[k]};
      if (neighbour_block.x >= 0 && neighbour_block.x < width &&
          neighbour_block.y >= 0 && neighbour_block.y < height &&
          neighbour_block.z >= 0 && neighbour_block.z < depth) {
        if (chunk[neighbour_block.x][neighbour_block.y][neighbour_block.z] ==
            block_copy) {
          chunk_place_block(chunk, width, height, depth, neighbour_block.x,
                            neighbour_block.y, neighbour_block.z, block);
          queue[rear++] = neighbour_block;
        }
      }
    }
  }
  return chunk;
}
