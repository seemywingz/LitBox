#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <ESPWiFi.h>
#include <arduinoFFT.h>

#include "Birds.h"
#include "Colors.h"
#include "GameofLife.h"
#include "MatrixAnimation.h"
#include "Motion.h"
#include "SpectralAnalyzer.h"
#include "Stars.h"
#include "Text.h"
#include "Utils.h"
#define LED_BUILTIN 8

// WeMos Pin Config:
// MIC Pin A0
// SCL Pin D1
// SDA Pin D2
// LED Pin D6

// LED Matrix Config
int LEDWidth = 32;
int LEDHeight = 8;
int ledDataPin = 12;
uint8_t matrixType =
    NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG;
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(
    LEDWidth, LEDHeight, ledDataPin, matrixType, NEO_GRB + NEO_KHZ800);

// Brightness and Color Config
const int maxBrightness = 255;
const int minBrightness = 1;
int brightness = 18;

// Visualization Config
const int maxFrameRate = 120;
unsigned int frameRate = 60;
String visualization = "bars";

// temperature Config
String temperatureUnit = "C";

// Web Server ConfigN
ESPWiFi wifi;

void setup() {
  // TODO: Use loaded config on startup
  initializeMatrix();
  initializeWebServer();
  initializeFromConfig();
  initializeMotion(LEDWidth, LEDHeight);
  randomSeed(analogRead(A0));
  Serial.println("Lit Box Initialized");
}

void initializeMatrix() {
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(brightness);
  testMatrix(&matrix, LEDWidth, LEDHeight);
}

void loop() {
  // TODO: Use loaded config
  wifi.handleClient();
  if (visualization == "waveform") {
    drawWaveform();
  } else if (visualization == "circles") {
    drawCircles();
  } else if (visualization == "motion") {
    drawMotion();
  } else if (visualization == "starField") {
    // the star field will be a 3D visualization
    // the stars are position locked and the movement of the board will
    // change the perspective of the stars, farther stars brightness will
    // be dim
    // and closer stars will be brighter
  } else if (visualization == "text") {
    displayOrScrollText(&matrix, text, &wifi);
  } else if (visualization == "birds") {
    runAtFrameRate(drawBirds, frameRate);
  } else if (visualization == "gameOfLife") {
    runAtFrameRate(drawGameOfLife, frameRate);
  } else if (visualization == "temperature") {
    runAtFrameRate(drawTemperature, frameRate);
  } else if (visualization == "matrix") {
    runAtFrameRate(drawMatrixAnimation, frameRate);
  } else if (visualization == "starPulse") {
    drawStarPulse();
  } else {
    drawBars();
  }
}

void initializeFromConfig() {
  setBrightness(wifi.config["brightness"].as<int>());
  setSensitivity(wifi.config["sensitivity"].as<int>());
  setFramerate(wifi.config["frameRate"].as<unsigned int>());

  visualization = wifi.config["visualization"].as<String>();
  temperatureUnit = wifi.config["temperatureUnit"].as<String>();

  JsonArray colorArray = wifi.config["colorPallet"];
  for (int i = 0; i < palletSize; i++) {
    colorPallet[i] = colorArray[i];
  }
  pixelColor = wifi.config["pixelColor"];
  pixelBgColor = wifi.config["pixelBgColor"];

  text = wifi.config["text"]["content"].as<String>();
  textSpeed = wifi.config["text"]["speed"].as<int>();
}

void drawTemperature() {
  staticText(&matrix, String(getTemperature(temperatureUnit)));
}

void drawMatrixAnimation() { matrixAnimation(&matrix, LEDWidth, LEDHeight); }

void drawMotion() {
  motionAnimation(LEDWidth, LEDHeight, frameRate);
  matrix.fillScreen(0);
  for (int i = 0; i < motionNumObjects; i++) {
    Body* b = motionObjects[i].body;
    matrix.drawRect(round(b->position.x), round(b->position.y), b->width.x,
                    b->width.y, motionObjects[i].color);
  }
  matrix.show();
}

void drawBars() {
  spectralAnalyzer(LEDWidth, LEDHeight);
  matrix.fillScreen(0);
  for (int x = 0; x < LEDWidth; x++) {
    for (int y = 0; y < spectralData[x]; y++) {
      uint32_t pixelColor = colorPallet[0];
      pixelColor = (y > 1) ? colorPallet[1] : pixelColor;
      pixelColor = (y > 3) ? colorPallet[2] : pixelColor;
      pixelColor = (y > 6) ? colorPallet[3] : pixelColor;
      matrix.drawPixel(x, LEDHeight - 1 - y, pixelColor);
    }
  }
  matrix.show();
}

void drawBirds() {
  updateFlock(LEDWidth, LEDHeight);
  matrix.fillScreen(0);
  for (int i = 0; i < birdCount; i++) {
    matrix.drawPixel(birds[i].x, birds[i].y, birds[i].color);
  }
  matrix.show();
}

void drawCircles() {
  spectralAnalyzer(LEDWidth, LEDHeight);
  matrix.fillScreen(0);
  for (int x = 0; x < LEDWidth; x++) {
    int circleRadius = spectralData[x] / 2;
    int circleColor = colorPallet[0];
    circleColor = (circleRadius > 1) ? colorPallet[1] : circleColor;
    circleColor = (circleRadius > 2) ? colorPallet[2] : circleColor;
    circleColor = (circleRadius >= 3) ? colorPallet[3] : circleColor;
    if (circleRadius > 0) {
      matrix.drawCircle(x, 4, circleRadius, circleColor);
    }
  }
  matrix.show();
}

void drawGameOfLife() {
  updateGameOfLife(LEDWidth, LEDHeight, 231);
  matrix.fillScreen(pixelBgColor);
  for (int x = 0; x < LEDWidth; x++) {
    for (int y = 0; y < LEDHeight; y++) {
      if (gol_Cells[x][y] == 1) {
        matrix.drawPixel(x, y, pixelColor);
      }
    }
  }
  matrix.show();
}

void drawStarPulse() {
  spectralAnalyzer(LEDWidth, LEDHeight);
  updateStartPulse(LEDWidth, LEDHeight);
  matrix.fillScreen(0);
  for (int i = 0; i < starCount; i++) {
    matrix.drawPixel(stars[i].x, stars[i].y, stars[i].color);
  }
  matrix.show();
}

void drawWaveform() {
  spectralAnalyzer(LEDWidth, LEDHeight);
  matrix.fillScreen(0);

  int middleY = LEDHeight / 2;  // Calculate the middle row of the matrix

  for (int x = 0; x < LEDWidth; x++) {
    int value = spectralData[x] / 2;
    for (int y = 0; y < value; y++) {
      // Draw upwards from the middle
      uint32_t pixelColor = colorPallet[0];
      if (y > 1) {
        pixelColor = colorPallet[1];
      }
      if (y > 2) {
        pixelColor = colorPallet[2];
      }
      if (y > 3) {
        pixelColor = colorPallet[3];
      }
      matrix.drawPixel(x, middleY - y, pixelColor);
      // Draw downwards from the middle
      if (y > 0 && middleY + y < LEDHeight) {
        matrix.drawPixel(x, middleY + y, pixelColor);
      }
    }
  }

  matrix.show();
}

void setBrightness(int newBrightness) {
  wifi.config["brightness"] =
      constrain(newBrightness, minBrightness, maxBrightness);
  matrix.setBrightness(wifi.config["brightness"]);
  matrix.show();
}

void setSensitivity(int newSensitivity) {
  constrain(newSensitivity, minSensitivity, maxSensitivity);
  sensitivity = newSensitivity;
}

void setFramerate(unsigned int fps) {
  frameRate = constrain(fps, 1, maxFrameRate);
}

void initializeWebServer() {
  wifi.webServer.on("/config", HTTP_GET, []() {
    JsonDocument config;
    config.set(wifi.config);
    for (int i = 0; i < palletSize; i++) {
      config["colorPallet"][i] = colorToHex(colorPallet[i]);
    }
    config["pixelColor"] = colorToHex(pixelColor);
    config["pixelBgColor"] = colorToHex(pixelBgColor);
    wifi.webServer.send(200, "application/json", config.as<String>());
  });

  wifi.webServer.on("/config", HTTP_POST, []() {
    String body = wifi.webServer.arg("plain");
    JsonDocument config;
    DeserializationError error = deserializeJson(config, body);
    if (error) {
      wifi.webServer.send(400, "text/plain", "Invalid JSON");
      return;
    } else {
      if (config.containsKey("brightness")) {
        setBrightness(config["brightness"]);
      }
      if (config.containsKey("sensitivity")) {
        setSensitivity(config["sensitivity"]);
      }
      if (config.containsKey("frameRate")) {
        setFramerate(config["frameRate"]);
      }
      if (config.containsKey("visualization")) {
        visualization = config["visualization"].as<String>();
      }
      if (config.containsKey("temperatureUnit")) {
        temperatureUnit = config["temperatureUnit"].as<String>();
      }
      if (config.containsKey("colorPallet")) {
        for (int i = 0; i < palletSize; i++) {
          colorPallet[i] = hexToColor(config["colorPallet"][i].as<String>());
        }
      }
      if (config.containsKey("pixelColor")) {
        pixelColor = hexToColor(config["pixelColor"]);
      }
      if (config.containsKey("pixelBgColor")) {
        pixelBgColor = hexToColor(config["pixelBgColor"]);
      }
      stars = nullptr;
      birds = nullptr;
      wifi.webServer.send(200, "application/json", config.as<String>());
      for (int i = 0; i < palletSize; i++) {
        config["colorPallet"][i] = hexToColor(config["colorPallet"][i]);
      }
      config["pixelColor"] = hexToColor(config["pixelColor"]);
      config["pixelBgColor"] = hexToColor(config["pixelBgColor"]);
      wifi.config.set(config);
    }
  });

  wifi.webServer.on("/saveConfig", HTTP_GET, []() {
    wifi.saveConfig();
    wifi.webServer.send(200, "application/json", wifi.config.as<String>());
  });

  wifi.webServer.on("/text", HTTP_POST, []() {
    String body = wifi.webServer.arg("plain");
    JsonDocument config;
    DeserializationError error = deserializeJson(config, body);
    if (error) {
      wifi.webServer.send(400, "text/plain", "Invalid JSON");
      return;
    } else {
      if (config.containsKey("text")) {
        text = config["text"]["content"].as<String>();
        if (config["text"].containsKey("speed")) {
          textSpeed = config["text"]["speed"].as<int>();
        }
        if (config["text"].containsKey("animation")) {
          String textAnimation = config["text"]["animation"].as<String>();
          if (textAnimation == "scroll") {
            scrollText(&matrix, text, &wifi);
          } else if (textAnimation == "wave") {
            waveText(&matrix, text);
          } else if (textAnimation == "display") {
            visualization = "text";
          }
        }
      }
      wifi.config.set(config);
      wifi.webServer.send(200, "application/json", config.as<String>());
    }
  });

  wifi.connectSubroutine = []() { testMatrix(&matrix, LEDWidth, LEDHeight); };
  wifi.start();
}
