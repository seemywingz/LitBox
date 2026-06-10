#include <ESPWiFi.h>
#include <LED/Colors.h>
#include <LED/Matrix.h>
// #include <LED/Text.h>
// #include <SpectralAnalyzer.h>
#include <math.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

// #include "Birds.h"
// #include "GameOfLife.h"
// #include "MatrixAnimation.h"
// #include "Motion.h"
// #include "Stars.h"
#include "Text.h"

// Web Server
ESPWiFi device;

bool needsCleanUp = false;

// Visualization Config
unsigned int frameRate = 18;
const int maxFrameRate = 120;
std::string visualization = "bars";
std::string lastVisualization = visualization;


// void setBrightness(int newBrightness) {
//   device.config["brightness"] =
//       // constrain(newBrightness, minBrightness, maxBrightness);
//   FastLED.setBrightness(device.config["brightness"]);
// }

// void setSensitivity(int newSensitivity) {
//   device.config["sensitivity"] = constrain(newSensitivity, 1, 100);
//   sensitivity = device.config["sensitivity"];
// }

// void setFramerate(unsigned int fps) {
//   device.config["frameRate"] = constrain(fps, 1, maxFrameRate);
//   frameRate = device.config["frameRate"];
// }

// void drawBars() {
//   // Analyze audio and populate spectralData
//   spectralAnalyzer(LED_WIDTH, LED_HEIGHT);
//   FastLED.clear();

//   for (int x = 0; x < LED_WIDTH; x++) {
//     int spectralValue = spectralData[x];
//     // Apply minimum threshold to reduce noise
//     if (spectralValue > 1) {
//       for (int y = 0; y < spectralValue && y < LED_HEIGHT; y++) {
//         // Use colorPallet directly based on y value
//         CRGB color = colorPallet[min(y / 2, 3)];
//         drawPixel(x, y, color);
//       }
//     }
//   }

//   FastLED.show();
// }

// void drawCircles() {
//   spectralAnalyzer(LED_WIDTH, LED_HEIGHT);
//   FastLED.clear();
//   int maxRadius = LED_HEIGHT / 2;
//   for (int x = 0; x < LED_WIDTH; x++) {
//     int circleRadius = spectralData[x] / 2;
//     // Apply minimum threshold to reduce noise
//     if (circleRadius > 1) {
//       // Map radius proportionally to color index (0-3)
//       int colorIdx = map(circleRadius, 1, maxRadius, 0, 3);
//       colorIdx = constrain(colorIdx, 0, 3);
//       CRGB circleColor = colorPallet[colorIdx];
//       drawCircle(x, 3, circleRadius, circleColor);
//     }
//   }
//   FastLED.show();
// }

// void drawWaveform() {
//   spectralAnalyzer(LED_WIDTH, LED_HEIGHT);
//   FastLED.clear();

//   int centerY = LED_HEIGHT / 2 - 1;
//   int paletteSize = palletSize;

//   for (int x = 0; x < LED_WIDTH; x++) {
//     int amplitude = spectralData[x];
//     int maxAmplitudeInput = 32; // adjust as needed for your signal
//     int maxOffset = map(amplitude, 1, maxAmplitudeInput, 0, centerY + 1);
//     maxOffset = constrain(maxOffset, 0, centerY + 1);
//     if (amplitude > 0) {
//       if (maxOffset == 0) {
//         if (centerY >= 0 && centerY < LED_HEIGHT)
//           drawPixel(x, centerY, colorPallet[0]);
//       } else {
//         if (centerY >= 0 && centerY < LED_HEIGHT)
//           drawPixel(x, centerY, colorPallet[0]);
//         // Expand outwards from the center
//         for (int y = 1; y <= maxOffset; y++) {
//           int colorIdx = map(y, 0, centerY + 1, 0, paletteSize - 1);
//           CRGB color = colorPallet[colorIdx];
//           int upY = centerY - y;
//           int downY = centerY + y;
//           if (upY >= 0)
//             drawPixel(x, upY, color);
//           if (downY < LED_HEIGHT)
//             drawPixel(x, downY, color);
//         }
//       }
//     }
//   }
//   FastLED.show();
// }

// void drawBirds() {
//   updateFlock(LED_WIDTH, LED_HEIGHT);
//   FastLED.clear();
//   for (int i = 0; i < birdCount; i++) {
//     // Invert Y coordinate so birds appear right-side up
//     int displayY = LED_HEIGHT - 1 - birds[i].y;
//     drawPixel(birds[i].x, displayY, colorPallet[birds[i].colorPaletteIndex]);
//   }
//   FastLED.show();
// }

// void drawGameOfLife() {
//   updateGameOfLife(LED_WIDTH, LED_HEIGHT, 231);
//   fillMatrix(pixelBgColor);
//   for (int x = 0; x < LED_WIDTH; x++) {
//     for (int y = 0; y < LED_HEIGHT; y++) {
//       if (gol_Cells[x][y] == 1) {
//         drawPixel(x, y, pixelColor);
//       }
//     }
//   }
//   FastLED.show();
// }

// void drawStarPulse() {
//   spectralAnalyzer(LED_WIDTH, LED_HEIGHT);
//   updateStartPulse(LED_WIDTH, LED_HEIGHT);
//   FastLED.clear();
//   for (int i = 0; i < starCount; i++) {
//     drawPixel(stars[i].x, stars[i].y, colorPallet[stars[i].colorPaletteIndex]);
//   }
//   FastLED.show();
// }

// void drawMatrix() {
//   matrixAnimation(LED_WIDTH, LED_HEIGHT);
//   FastLED.show();
// }

// void drawMotion() {
//   motionStep(LED_WIDTH, LED_HEIGHT, device);
//   FastLED.clear();
//   for (int i = 0; i < motionNumObjects; i++) {
//     drawPixel(round(motionObjects[i].body->position.x),
//               round(motionObjects[i].body->position.y),
//               colorPallet[motionObjects[i].colorPaletteIndex]);
//   }
//   FastLED.show();
// }

// void drawTemperature() {
//   float temperature = device.getTemperature(device.config["temperatureUnit"]);
//   staticText(String(temperature).c_str());
// }

// void applyConfig() {
//   setBrightness(device.config["brightness"].as<int>());
//   setSensitivity(device.config["sensitivity"].as<int>());
//   setFramerate(device.config["frameRate"].as<unsigned int>());
//   visualization = device.config["visualization"].as<std::string>();

//   if (visualization != lastVisualization) {
//     device.log("📱 Visualization Changed: " + lastVisualization + " -> " +
//                visualization);
//     lastVisualization = visualization;
//     needsCleanUp = true;
//   }

//   if (!device.config["pixelColor"].isNull()) {
//     CRGB newColor = hexToCRGB(device.config["pixelColor"]);
//     if (newColor != pixelColor) {
//       pixelColor = newColor;
//       device.log("👾 Pixel Color Changed:");
//       device.log("\tR=" + String(pixelColor.r) + ", G=" + String(pixelColor.g) +
//                  ", B=" + String(pixelColor.b));
//     }
//   }
//   if (!device.config["pixelBgColor"].isNull()) {
//     CRGB newColor = hexToCRGB(device.config["pixelBgColor"]);
//     if (newColor != pixelBgColor) {
//       pixelBgColor = newColor;
//       device.log("👾 Pixel Background Color Changed:");
//       device.log("\tR=" + String(pixelBgColor.r) + ", G=" +
//                  String(pixelBgColor.g) + ", B=" + String(pixelBgColor.b));
//     }
//   }
//   if (!device.config["colorPallet"].isNull()) {
//     JsonArray arr = device.config["colorPallet"].as<JsonArray>();
//     for (int i = 0; i < palletSize && i < arr.size(); i++) {
//       CRGB newColor = hexToCRGB(arr[i]);
//       if (colorPallet[i] != newColor) {
//         colorPallet[i] = newColor;
//         device.log("🎨 Color Pallet Updated:");
//         device.log("\tColor " + String(i) + ": R=" + String(colorPallet[i].r) +
//                    ", G=" + String(colorPallet[i].g) +
//                    ", B=" + String(colorPallet[i].b));
//       }
//     }
//   }
// }

void setup() {
  device.start();

  // randomSeed(analogRead(A0));
  // device.connectSubroutine = []() {
  //   CRGB color = CRGB(random(255), random(255), random(255));
  //   testMatrix(color);
  // };

  // device.configUpdateCallback = applyConfig;
  // device.readConfig();
  // applyConfig();

  device.startLEDMatrix();
  testMatrix();

  // device.srvAll();
  device.startWiFi();
  // startText(&device);
  device.startMDNS();
  device.startWebServer();
  // device.startSpectralAnalyzer();
  // device.startBMI160();

  device.log("🔥 Lit Box Initialized");
}

void loop() {
 device.feedWatchDog();

  // handleText();

  // if (visualization == "circles") {
  //   drawCircles();
  // } else if (visualization == "birds") {
  //   runAtFrameRate(drawBirds, frameRate);
  // } else if (visualization == "gameOfLife") {
  //   runAtFrameRate(drawGameOfLife, frameRate);
  // } else if (visualization == "matrix") {
  //   runAtFrameRate(drawMatrix, frameRate);
  // } else if (visualization == "temperature") {
  //   runAtFrameRate(drawTemperature, frameRate);
  // } else if (visualization == "starPulse") {
  //   drawStarPulse();
  // } else if (visualization == "waveform") {
  //   drawWaveform();
  // } else if (visualization == "motion") {
  //   drawMotion();
  // } else {
  //   drawBars();
  // }

  // // Clean up memory when visualization changes
  // if (needsCleanUp) {
  //   device.log("🧹 Cleaning up memory...");
  //   motionObjects = nullptr;
  //   gol_Cells = nullptr;
  //   birds = nullptr;
  //   stars = nullptr;
  //   needsCleanUp = false;
  // }
}

extern "C" void app_main(void) {
  setup();
  while (true) {
    loop();
    vTaskDelay(pdMS_TO_TICKS(1));
  }
}
