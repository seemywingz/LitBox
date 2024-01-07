#ifndef SNAKE_H
#define SNAKE_H

#include "Colors.h"
#include "Motion.h"

struct SnakeNode : public Pixel {
  struct SnakeNode* next;
};

SnakeNode* snakeHead = nullptr;
SnakeNode* snakeTail = nullptr;
Pixel* snakeFood = nullptr;
int snakeLength = 0;
int snakeDX = 0;
int snakeDY = 0;

void generateFood(int maxX, int maxY) {
  snakeFood = new Pixel;
  snakeFood->x = random(0, maxX);
  snakeFood->y = random(0, maxY);
  snakeFood->color = colorPallet[random(1, 5)];
};

void initSnake(int maxX, int maxY) {
  // Initialize the snake
  snakeHead = new SnakeNode;
  snakeHead->x = random(0, maxX);
  snakeHead->y = random(0, maxY);
  snakeHead->next = nullptr;
  snakeHead->color = colorPallet[0];
  snakeTail = snakeHead;
  snakeLength = 1;
  generateFood(maxX, maxY);
};

void addSnakeNode(int x, int y) {
  SnakeNode* newTail = new SnakeNode;
  newTail->x = x;
  newTail->y = y;
  newTail->next = nullptr;
  newTail->color = colorPallet[0];
  snakeTail->next = newTail;
  snakeTail = newTail;
  snakeLength++;
};

void removeSnakeNode() {
  SnakeNode* newHead = snakeHead->next;
  delete snakeHead;
  snakeHead = newHead;
  snakeLength--;
};

void updateSnake(int maxX, int maxY, unsigned int& frameRate) {
  if (snakeLength == 0) {
    initSnake(maxX, maxY);
    frameRate = 3;
    snakeDX = 1;
  }

  // Move the snake
  readAccelerometer();
  float movementThreshold = 0.09;
  if (snakeDX != 0) {
    if (-ax > movementThreshold) {
      snakeDX = 0;
      snakeDY = 1;
    } else if (-ax < -movementThreshold) {
      snakeDX = 0;
      snakeDY = -1;
    }
  } else if (snakeDY != 0) {
    if (-ay > movementThreshold) {
      snakeDX = 1;
      snakeDY = 0;
    } else if (-ay < -movementThreshold) {
      snakeDX = -1;
      snakeDY = 0;
    }
  }

  int newX = snakeHead->x + snakeDX;
  int newY = snakeHead->y + snakeDY;

  // Check if the snake has eaten food
  if (newX == snakeFood->x && newY == snakeFood->y) {
    // Add a new node at the current position of the tail
    addSnakeNode(snakeTail->x, snakeTail->y);
    generateFood(maxX, maxY);
    // frameRate++;
  }

  // Move each node to the position of the previous node,
  // starting from the tail
  SnakeNode* current = snakeTail;
  while (current != snakeHead) {
    SnakeNode* previous = snakeHead;
    while (previous->next != current) {
      previous = previous->next;
    }
    current->x = previous->x;
    current->y = previous->y;
    current = previous;
  }

  // Update the head position last
  snakeHead->x = newX;
  snakeHead->y = newY;

  // Check if the snake is out of bounds and wrap around if necessary
  if (snakeHead->x < 0) {
    snakeHead->x = maxX - 1;
  } else if (snakeHead->x >= maxX) {
    snakeHead->x = 0;
  }
  if (snakeHead->y < 0) {
    snakeHead->y = maxY - 1;
  } else if (snakeHead->y >= maxY) {
    snakeHead->y = 0;
  }
}

#endif  // !SNAKE_H