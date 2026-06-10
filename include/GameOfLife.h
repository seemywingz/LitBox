#ifndef GAMEOFLIFE_H
#define GAMEOFLIFE_H


int gol_Generations = 0;
int** gol_Cells = nullptr;

int countNeighbors(int width, int height, int x, int y) {
  int neighbors = 0;
  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      if (x + i < 0 || x + i >= width || y + j < 0 || y + j >= height) {
        continue;
      } else {
        neighbors += gol_Cells[(x + i)][(y + j)];
      }
    }
  }
  return neighbors -= gol_Cells[x][y];
}

void applyRulesToCells(int width, int height) {
  int** nextGenCells = new int* [width] { 0 };
  for (int i = 0; i < width; ++i) nextGenCells[i] = new int[height]{0};

  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      int neighbors = countNeighbors(width, height, x, y);
      if (gol_Cells[x][y] == 1 && (neighbors == 2 || neighbors == 3)) {
        nextGenCells[x][y] = 1;
      } else if (gol_Cells[x][y] == 0 && neighbors == 3) {
        nextGenCells[x][y] = 1;
      } else {
        nextGenCells[x][y] = 0;
      }
    }
  }
  // copy nextGenCells to current cells
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      gol_Cells[x][y] = nextGenCells[x][y];
    }
  }
  // Free each sub-array
  for (int i = 0; i < width; i++) {
    delete[] nextGenCells[i];
  }
  // Free the array of pointers
  delete[] nextGenCells;
}

void randomizeCells(int width, int height) {
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      int rand = random(0, 3000);
      if ((rand % 2) == 0) {
        gol_Cells[x][y] = 1;
      }
    }
  }
}

void startGameOfLife(int width, int height) {
  gol_Generations = 0;
  gol_Cells = new int* [width] { 0 };
  for (int i = 0; i < width; ++i) gol_Cells[i] = new int[height]{0};
  randomSeed(analogRead(A0));
  randomizeCells(width, height);
}

void updateGameOfLife(int width, int height, int maxGenerations) {
  if (gol_Cells == nullptr) {
    startGameOfLife(width, height);
  }
  applyRulesToCells(width, height);
  if (gol_Generations++ >= maxGenerations) {
    startGameOfLife(width, height);
  }
}
#endif