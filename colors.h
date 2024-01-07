#ifndef COLORS_H
#define COLORS_H

#include <ArduinoJson.h>

// Color definitions
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF
#define ORANGE 0xFD20
#define TEAL 0x0410
#define VIOLET 0xEC1D
#define OLIVE 0x5345
#define GOLD 0xFEA0
#define SILVER 0xC618
#define DARK_GREEN 0x0320
#define FOREST_GREEN 0x2444
#define CORAL 0xFBEA
#define SALMON 0xFC0E
#define ROSE 0xF8A6
#define PEACH 0xFEA6

struct Pixel {
  float x = 0, y = 0;
  float vx = 0, vy = 0;
  byte intensity = 0;
  uint32_t color = 0;
};

const int palletSize = 4;
uint32_t pixelColor = WHITE;    // Default color
uint32_t pixelBgColor = BLACK;  // Default background color
uint32_t colorPallet[palletSize] = {BLUE, CYAN, VIOLET, WHITE};

void saveColors() {
  // Create a JSON document
  StaticJsonDocument<256> doc;

  // Add data to the document
  JsonArray colorArray = doc.createNestedArray("colorPallet");
  for (int i = 0; i < palletSize; i++) {
    colorArray.add(colorPallet[i]);
  }
  doc["pixelColor"] = pixelColor;
  doc["pixelBgColor"] = pixelBgColor;

  // Open file for writing
  File file = LittleFS.open("/config.json", "w");
  if (!file) {
    Serial.println("Failed to open config file for writing");
    return;
  }

  // Serialize JSON to file
  if (serializeJson(doc, file) == 0) {
    Serial.println("Failed to write to file");
  }

  file.close();
}

void loadColors() {
  // Open file for reading
  File file = LittleFS.open("/config.json", "r");
  if (!file) {
    Serial.println("Failed to open config file for reading");
    return;
  }

  // Deserialize JSON document
  StaticJsonDocument<256> doc;
  DeserializationError error = deserializeJson(doc, file);
  if (error) {
    Serial.print("Failed to read file, using default configuration");
  }

  // Read data from document
  JsonArray colorArray = doc["colorPallet"];
  for (int i = 0; i < palletSize; i++) {
    colorPallet[i] = colorArray[i];
  }
  pixelColor = doc["pixelColor"];
  pixelBgColor = doc["pixelBgColor"];

  file.close();
}

String colorToHex(uint16_t rgb565) {
  // Extract RGB components
  uint8_t r = (rgb565 >> 11) & 0x1F;  // Extract the 5 bits of the red component
  uint8_t g =
      (rgb565 >> 5) & 0x3F;   // Extract the 6 bits of the green component
  uint8_t b = rgb565 & 0x1F;  // Extract the 5 bits of the blue component

  // Scale the components to 8-bit values
  r = (r * 255) / 31;
  g = (g * 255) / 63;
  b = (b * 255) / 31;

  // Construct hex string
  char hexColor[7];
  sprintf(hexColor, "#%02X%02X%02X", r, g, b);

  return String(hexColor);
}

uint32_t hexToColor(String hexColor) {
  // Remove the '#' character if present
  if (hexColor.startsWith("#")) {
    hexColor = hexColor.substring(1);
  }

  // Convert the hex string to an integer
  unsigned long number = strtoul(hexColor.c_str(), NULL, 16);

  // Extract RGB components
  uint8_t r = (number >> 16) & 0xFF;  // Extract the RR byte
  uint8_t g = (number >> 8) & 0xFF;   // Extract the GG byte
  uint8_t b = number & 0xFF;          // Extract the BB byte

  // Convert to 5-6-5 format
  uint16_t rgb565 = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);

  return rgb565;
}
#endif  // COLORS_H