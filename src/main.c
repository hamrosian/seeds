#include <math.h>
#include <raylib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int trun(int x, int n) { return (((x % n) + n) % n); }

uint8_t update_neighbors(uint8_t *grid, int cols, int rows, int x, int y) {
  uint8_t current = grid[x + y * cols] & 0b10000000;
  int neighbors = 0;
  for (int i = -1; i < 2; i++) {
    for (int j = -1; j < 2; j++) {
      if (i == 0 && j == 0) {
        continue;
      } else {
        int col = trun(x + i, cols);
        int row = trun(y + j, rows);
        if (grid[col + row * cols] & 0b10000000) {
          neighbors++;
        }
      }
    }
  }
  current = current | neighbors;
  return current;
}

void update_neighbors_all(uint8_t *grid, int cols, int rows) {
  // #pragma omp parallel for
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      grid[j + i * cols] = update_neighbors(grid, cols, rows, j, i);
    }
  }
}

void run_seeds(uint8_t *data, int rows, int cols) {
  uint8_t *grid = malloc(sizeof(uint8_t) * rows * cols);
  memcpy(grid, data, sizeof(uint8_t) * rows * cols);
  update_neighbors_all(grid, cols, rows);
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      int alive = grid[(i * rows) + j] & 0b10000000;
      int n = grid[(i * rows) + j] & 0b00001111;
      if (n == 2 && alive == 0) {
        grid[(i * rows) + j] = grid[(i * rows) + j] | 0b10000000;
      } else {
        grid[(i * rows) + j] = grid[(i * rows) + j] & 0b00001111;
      }
    }
  }
  memcpy(data, grid, sizeof(uint8_t) * rows * cols);
}

int main(int argc, char *argv[]) {

  srand(time(NULL));
  uint16_t rows = 100;
  uint16_t cols = 100;

  // 0000 0000
  // ^--- ^^^^
  // |--- ||||
  // [ON] \--------[NUM NEIGHBHORS]
  uint8_t *grid = malloc(sizeof(uint8_t) * rows * cols);
  for (int i = 0; i < rows * cols; i++) {
    double n = (double)rand() / RAND_MAX;
    if (n > 0.995) {
      grid[i] = 0b10000000;
    } else {
      grid[i] = 0b00000000;
    }
  }

  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(1200, 1200, "seeds");
  SetTargetFPS(30);

  int scale =
      fmin((float)GetScreenWidth() / rows, (float)GetScreenHeight() / cols);
  while (!WindowShouldClose()) {
    scale =
        fmin((float)GetScreenWidth() / rows, (float)GetScreenHeight() / cols);
    BeginDrawing();
    DrawFPS(0, 0);
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        if (grid[(i * rows) + j] / 0b10000000) {
          ClearBackground(BLACK);
          DrawRectangle(i * scale, j * scale, scale, scale, WHITE);
        }
      }
    }
    EndDrawing();
    run_seeds(grid, rows, cols);
  }
  CloseWindow();
  return 0;
}
