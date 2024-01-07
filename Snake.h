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
  }

  // Move the snake
  readAccelerometer();
  for (int i = 0; i < snakeLength; i++) {
    if (i == 0) {
      // Move the head
      snakeHead->x += -ay;
      snakeHead->y += -ax;
      snakeHead->x = round(snakeHead->x);
      snakeHead->y = round(snakeHead->y);
    } else {
      // Move the body
      SnakeNode* current = snakeHead;
      for (int j = 0; j < i; j++) {
        current = current->next;
      }
      current->x = current->next->x;
      current->y = current->next->y;
    }
  }

  // Check if the snake has eaten food
  if (snakeHead->x == snakeFood->x && snakeHead->y == snakeFood->y) {
    // Snake has eaten food
    addSnakeNode(snakeTail->x, snakeTail->y);
    generateFood(maxX, maxY);
    frameRate += 5;
  }

  // Check if the snake is out of bounds
  if (snakeHead->x < 0) {
    snakeHead->x = maxX - 1;
  } else if (snakeHead->x > maxX) {
    snakeHead->x = 0;
  }

  if (snakeHead->y < 0) {
    snakeHead->y = maxY - 1;
  } else if (snakeHead->y > maxY) {
    snakeHead->y = 0;
  }

  // Check if the snake has collided with itself
  SnakeNode* current = snakeHead->next;
  while (current != nullptr) {
    if (current->x == snakeHead->x && current->y == snakeHead->y) {
      // Snake has collided with itself
      // Reset the snake
      while (snakeLength > 1) {
        removeSnakeNode();
        frameRate--;
      }
      break;
    }
    current = current->next;
  }
};

#endif  // !SNAKE_H