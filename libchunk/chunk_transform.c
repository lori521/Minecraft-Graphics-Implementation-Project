#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "chunk.h"

char*** chunk_rotate_y(char*** chunk, int width, int height, int depth) {
  int new_width = depth;
  int new_height = height;
  int new_depth = width;
  char*** copy_of_chunk = (char***)malloc(new_width * sizeof(char**));
  for (int i = 0; i < new_width; i++) {
    copy_of_chunk[i] = (char**)malloc(new_height * sizeof(char*));
    for (int j = 0; j < new_height; j++) {
      copy_of_chunk[i][j] = (char*)malloc(new_depth * sizeof(char));
      for (int t = 0; t < new_depth; t++) {
        copy_of_chunk[i][j][t] = 0;
      }
    }
  }
  for (int i = 0; i < width; i++) {
    for (int t = 0; t < height; t++) {
      for (int j = 0; j < depth; j++) {
        copy_of_chunk[depth - j - 1][t][i] = chunk[i][t][j];
      }
    }
  }
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      free(chunk[x][y]);
    }
    free(chunk[x]);
  }
  free(chunk);

  return copy_of_chunk;
}

struct block_position {
  int x, y, z;
};
typedef struct block_position block_position;

struct corp {
  int number_of_blocks, capacitate;
  block_position* blocks;
  char tip;
  bool stop;
};
typedef struct corp corp;

void add_block(corp* corp_current, block_position block) {
  if (corp_current->number_of_blocks == corp_current->capacitate) {
    corp_current->capacitate *= 2;
    corp_current->blocks = (block_position*)realloc(
        corp_current->blocks,
        corp_current->capacitate * sizeof(block_position));
  }
  corp_current->blocks[corp_current->number_of_blocks++] = block;
}

void bfs(char*** chunk, int width, int height, int depth, int*** matrix_freq,
         corp* corp_current, block_position start) {
  char corp_tip = chunk[start.x][start.y][start.z];
  corp_current->tip = corp_tip;
  int size = width * height * depth;
  int queue_capatitate = size;
  int front = 0, rear = 0;
  block_position* queue =
      (block_position*)malloc(queue_capatitate * sizeof(block_position));
  queue[rear++] = start;
  matrix_freq[start.x][start.y][start.z] = 1;

  const int nmax = 6;
  int x_p[] = {-1, 0, 1, 0, 0, 0};
  int y_p[] = {0, 0, 0, 0, -1, 1};
  int z_p[] = {0, 1, 0, -1, 0, 0};

  while (front < rear) {
    block_position current = queue[front++];
    add_block(corp_current, current);
    for (int t = 0; t < nmax; t++) {
      int new_x = current.x + x_p[t];
      int new_y = current.y + y_p[t];
      int new_z = current.z + z_p[t];
      if (new_x >= 0 && new_x < width && new_y >= 0 && new_y < height &&
          new_z >= 0 && new_z < depth) {
        if (chunk[new_x][new_y][new_z] == corp_tip &&
            matrix_freq[new_x][new_y][new_z] == 0) {
          if (rear == queue_capatitate) {
            queue_capatitate *= 2;
            queue = (block_position*)realloc(
                queue, queue_capatitate * sizeof(block_position));
          }
          queue[rear++] = (block_position){new_x, new_y, new_z};
          matrix_freq[new_x][new_y][new_z] = 1;
        }
      }
    }
  }
  free(queue);
}

char*** chunk_apply_gravity(char*** chunk, int width, int height, int depth,
                            int* new_height) {
  char*** new_chunk = (char***)malloc(width * sizeof(char**));
  for (int x = 0; x < width; x++) {
    new_chunk[x] = (char**)malloc(height * sizeof(char*));
    for (int y = 0; y < height; y++) {
      new_chunk[x][y] = (char*)malloc(depth * sizeof(char));
      for (int z = 0; z < depth; z++) {
        new_chunk[x][y][z] = chunk[x][y][z];
      }
    }
  }
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      free(chunk[x][y]);
    }
    free(chunk[x]);
  }
  free(chunk);
  int*** matrix_freq = (int***)malloc(width * sizeof(int**));
  for (int i = 0; i < width; i++) {
    matrix_freq[i] = (int**)malloc(height * sizeof(int*));
    for (int j = 0; j < height; j++) {
      matrix_freq[i][j] = (int*)calloc(depth, sizeof(int));
    }
  }

  int corpuri_size = width * height * depth;
  corp* corpuri = (corp*)malloc(corpuri_size * sizeof(corp));
  int corpuri_index = 0;

  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      for (int z = 0; z < depth; z++) {
        if (new_chunk[x][y][z] != 0 && matrix_freq[x][y][z] == 0) {
          if (corpuri_index == corpuri_size) {
            corpuri_size *= 2;
            corpuri = (corp*)realloc(corpuri, corpuri_size * sizeof(corp));
          }
          corpuri[corpuri_index].number_of_blocks = 0;
          corpuri[corpuri_index].capacitate = width * height * depth;
          corpuri[corpuri_index].blocks = (block_position*)malloc(
              width * height * depth * sizeof(block_position));
          corpuri[corpuri_index].tip = new_chunk[x][y][z];
          corpuri[corpuri_index].stop = false;
          block_position start = {x, y, z};
          bfs(new_chunk, width, height, depth, matrix_freq,
              &corpuri[corpuri_index], start);
          corpuri_index++;
        }
      }
    }
  }

  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      free(matrix_freq[i][j]);
    }
    free(matrix_freq[i]);
  }
  free(matrix_freq);

  if (corpuri_index == 0) {
    *new_height = height;
    return new_chunk;
  }
  bool moved = false;
  do {
    moved = false;

    bool* can_move = (bool*)malloc(corpuri_index * sizeof(bool));
    for (int c = 0; c < corpuri_index; c++) {
      can_move[c] = !corpuri[c].stop;
    }

    bool changed = false;
    do {
      changed = false;
      for (int c = 0; c < corpuri_index; c++) {
        if (!corpuri[c].stop && can_move[c]) {
          corp* corp_current = &corpuri[c];
          bool can_fall = true;
          for (int k = 0; k < corp_current->number_of_blocks; k++) {
            block_position block = corp_current->blocks[k];
            if (block.y == 0) {
              // suntem la sol
              can_fall = false;
              break;
            }
            char sub = new_chunk[block.x][block.y - 1][block.z];
            if (sub != 0) {
              // avem corp dedesubt
              // vedem daca blocul de dedesubt apartine unuia dintre corpurile
              // noastre
              bool found = false;
              for (int t = 0; t < corpuri_index; t++) {
                for (int kk = 0; kk < corpuri[t].number_of_blocks; kk++) {
                  block_position block2 = corpuri[t].blocks[kk];
                  if (block2.x == block.x && block2.y == block.y - 1 &&
                      block2.z == block.z) {
                    found = true;
                    // daca corpul de dedesubt nu se poate misca nici corpul
                    // nostru nu se poate misca
                    if (corpuri[t].stop || !can_move[t]) {
                      can_fall = false;
                    }
                    break;
                  }
                }
                if (found) break;
              }
              if (!found && sub != 0) {
                can_fall = false;
              }
            }
            if (!can_fall) break;
          }
          if (!can_fall) {
            can_move[c] = false;
            corpuri[c].stop = true;
            changed = true;
          }
        }
      }
    } while (changed);
    bool any_can_move = false;
    for (int c = 0; c < corpuri_index; c++) {
      if (!corpuri[c].stop && can_move[c]) {
        any_can_move = true;
        break;
      }
    }
    if (any_can_move) {
      // coboram ce se poate misca
      moved = true;
      for (int c = 0; c < corpuri_index; c++) {
        if (!corpuri[c].stop && can_move[c]) {
          for (int k = 0; k < corpuri[c].number_of_blocks; k++) {
            block_position block = corpuri[c].blocks[k];
            new_chunk[block.x][block.y][block.z] = 0;
          }
        }
      }
      for (int c = 0; c < corpuri_index; c++) {
        if (!corpuri[c].stop && can_move[c]) {
          for (int k = 0; k < corpuri[c].number_of_blocks; k++) {
            corpuri[c].blocks[k].y -= 1;
            block_position block = corpuri[c].blocks[k];
            new_chunk[block.x][block.y][block.z] = corpuri[c].tip;
          }
        }
      }
    }
    free(can_move);
  } while (moved);

  // eliminam planurile goale de sus

  int final_height = height;
  while (final_height > 0) {
    int empty = 1;
    for (int x = 0; x < width && empty; x++) {
      for (int z = 0; z < depth && empty; z++) {
        if (new_chunk[x][final_height - 1][z] != 0) {
          empty = 0;
        }
      }
    }
    if (empty)
      final_height--;
    else
      break;
  }

  *new_height = final_height;
  if (final_height == height) {
    // ramane la fel
    for (int c = 0; c < corpuri_index; c++) {
      free(corpuri[c].blocks);
    }
    free(corpuri);
    return new_chunk;
  }

  // altfel redimensionam chunkul
  char*** chunk_final = (char***)malloc(width * sizeof(char**));
  for (int x = 0; x < width; x++) {
    chunk_final[x] = (char**)malloc(final_height * sizeof(char*));
    for (int y = 0; y < final_height; y++) {
      chunk_final[x][y] = (char*)malloc(depth * sizeof(char));
      for (int z = 0; z < depth; z++) {
        chunk_final[x][y][z] = new_chunk[x][y][z];
      }
    }
  }

  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      free(new_chunk[x][y]);
    }
    free(new_chunk[x]);
  }
  free(new_chunk);

  for (int c = 0; c < corpuri_index; c++) {
    free(corpuri[c].blocks);
  }
  free(corpuri);

  return chunk_final;
}
