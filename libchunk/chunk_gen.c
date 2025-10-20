#include "chunk.h"
#include "math.h"

char*** chunk_place_block(
    char*** chunk, int width, int height, int depth,
    int x, int y, int z, char block) {
     if ( (x < width && x >= 0) &&
     (y < height && y >=0) &&
     (z < depth && z >=0) )
     chunk[x][y][z] = block;
     return chunk;
}

char*** chunk_fill_cuboid(
    char*** chunk, int width, int height, int depth,
    int x0, int y0, int z0, int x1, int y1, int z1, char block) {
        if ( x1 >= width ) {
          x1 = width - 1;
        }
        if ( y0 >= height ) {
            y0 = height - 1;
        }
        if ( z1 >= depth ) {
            z1 = depth - 1;
        }
        if ( x0 > x1 ) {
           int aux = x0;
           x0 = x1;
           x1 = aux;
        }
        if ( y0 > y1 ) {
           int aux = y0;
           y0 = y1;
           y1 = aux;
        }
        if ( z0 > z1 ) {
           int aux = z0;
           z0 = z1;
           z1 = aux;
        }
        for ( int i = x0; i <= x1; i++ ) {
            for ( int j = y0; j <= y1; j++ ) {
                for ( int t = z0; t <= z1; t++ ) {
                    chunk_place_block(chunk, width, height, depth,
                    i, j, t, block);
                }
            }
        }
        return chunk;
}

char*** chunk_fill_sphere(
    char*** chunk, int width, int height, int depth,
    int x, int y, int z, double radius, char block) {
        for (int i = (x - (int)radius) - 1; i <= x + (int)radius; i++) {
            for (int j = (y - (int)radius) - 1; j <= y + (int)radius; j++) {
                for (int t = (z - (int)radius) - 1; t <= z + (int)radius; t++) {
                    if (sqrt(pow(i - x, 2) + pow(j - y, 2) + pow(t - z, 2)) <= radius) {
                         chunk_place_block(chunk, width, height, depth, i, j, t, block);
                    }
                }
            }
        }
        return chunk;
}

