#ifndef TEXT_H
#define TEXT_H

#include <Adafruit_NeoMatrix.h>

#include "Colors.h"

int textSize = 1;                   // Default text size
int textSpeed = 75;                 // Default speed
int textPixelSize = 6;              // Number of pixels per character
String text = "*.*. Lit Box .*.*";  // Default text

void scrollText(Adafruit_NeoMatrix* matrix, String text, ESPWiFi* wifi) {
  matrix->setTextColor(pixelColor);
  int startX = matrix->width();
  int len = text.length() * textPixelSize;
  for (int x = startX; x > -len; x--) {
    matrix->fillScreen(pixelBgColor);
    matrix->setCursor(x, 0);
    matrix->print(text);
    matrix->show();
    delay(100 - textSpeed);
    if (wifi != nullptr) {
      wifi->handleClient();
    }
  }
}

void staticText(Adafruit_NeoMatrix* matrix, String text) {
  int textLength = text.length() * 6;
  int xStart = (matrix->width() - textLength) / 2;
  int yStart = (matrix->height() - 8) / 2;

  matrix->fillScreen(pixelBgColor);   // Clear the matrix
  matrix->setTextColor(pixelColor);   // Set the text color
  matrix->setCursor(xStart, yStart);  // Set the starting position
  matrix->print(text);                // Print the text
  matrix->show();                     // Show the text on the matrix
}

bool textFits(Adafruit_NeoMatrix* matrix, String text) {
  int textLength = text.length() * textPixelSize;
  if (textLength > matrix->width()) {
    return false;
  }
  return true;
}

void displayOrScrollText(Adafruit_NeoMatrix* matrix, String text,
                         ESPWiFi* wifi) {
  int textLength = text.length() * textPixelSize;
  if (!textFits(matrix, text)) {
    scrollText(matrix, text, wifi);  // Scroll text if it's too long
  } else {
    staticText(matrix, text);  // Display static text if it fits
  }
}

void waveText(Adafruit_NeoMatrix* matrix, String text) {
  matrix->setTextColor(pixelColor);  // Set the text color
  int textLength = text.length() * textPixelSize;
  for (int x = 0; x < matrix->width() + textLength; x++) {
    int y = sin(x / 2.0) * 4;          // Sine wave for vertical position
    matrix->fillScreen(pixelBgColor);  // set bg color
    matrix->setCursor(matrix->width() - x, y);  // Set start position
    matrix->print(text);                        // Print the text
    matrix->show();                             // Show on the matrix
    delay(100 - textSpeed);                     // Delay for scrolling speed
  }
}

#endif
