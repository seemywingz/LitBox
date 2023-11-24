#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <ESPWiFi.h>
#include <IOPin.h>
#include <arduinoFFT.h>

#include "Birds.h"
#include "Colors.h"
#include "GameofLife.h"
#include "SpectralAnalyzer.h"

// IOPins
IOPin ledData(12);  // Board Pin D6

// LED Matrix Config
int LEDWidth = 32;
int LEDHeight = 8;
uint8_t matrixType =
    NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG;
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(
    LEDWidth, LEDHeight, ledData.pin(), matrixType, NEO_GRB + NEO_KHZ800);

// Brightness and Color Config
const int minBrightness = 1;
const int maxBrightness = 255;
int brightness = 6;

// Visualization Config
const int maxFrameRate = 120;
unsigned int frameRate = 60;
int visualization = 0;
int maxVisualization = 3;

// text color and speed
int textSpeed = 60;                // Default speed
uint32_t textColor = WHITE;        // Default color
String text = "*.*. LuciFi .*.*";  // Default text

// Web Server Config
ESPWiFi wifi = ESPWiFi("LuciFi", "abcd1234");

void setup() {
  matrix.begin();
  Serial.begin(115200);
  matrix.setTextWrap(false);
  ledData.setPinMode(OUTPUT);
  matrix.setBrightness(brightness);
  testMatrix(&matrix, LEDWidth, LEDHeight);
  initializeWebServer();
}

void loop() {
  wifi.handleClient();
  switch (visualization) {
    case 1:
      drawCircles();
      break;
    case 2:
      runAtFrameRate(drawBirds, frameRate);
      break;
    case 3:
      runAtFrameRate(drawGameOfLife, frameRate);
      break;
    default:
      drawBars();
      break;
  }
}

void drawGameOfLife() {
  if (gol_Cells == nullptr) {
    startGameOfLife(LEDWidth, LEDHeight);
  }
  int cellColor = colorPallets[currentPalette][0];
  updateGameOfLife(LEDWidth, LEDHeight, 231);
  matrix.fillScreen(0);
  for (int x = 0; x < LEDWidth; x++) {
    for (int y = 0; y < LEDHeight; y++) {
      if (gol_Cells[x][y] == 1) {
        matrix.drawPixel(x, y, cellColor);
      }
    }
  }
  matrix.show();
}

void drawCircles() {
  spectralAnalyzer(LEDWidth, LEDHeight);
  matrix.fillScreen(0);
  for (int x = 0; x < LEDWidth; x++) {
    int circleRadius = spectralData[x];
    int circleColor = colorPallets[currentPalette][0];
    circleColor =
        (circleRadius > 2) ? colorPallets[currentPalette][1] : circleColor;
    circleColor =
        (circleRadius > 3) ? colorPallets[currentPalette][2] : circleColor;
    circleColor =
        (circleRadius > 5) ? colorPallets[currentPalette][3] : circleColor;
    if (circleRadius > 0) {
      matrix.drawCircle(x, 4, circleRadius, circleColor);
    }
  }
  matrix.show();
}

void drawBars() {
  spectralAnalyzer(LEDWidth, LEDHeight);
  matrix.fillScreen(0);
  for (int x = 0; x < LEDWidth; x++) {
    for (int y = 0; y < spectralData[x]; y++) {
      uint32_t pixelColor = colorPallets[currentPalette][0];
      pixelColor = (y > 1) ? colorPallets[currentPalette][1] : pixelColor;
      pixelColor = (y > 3) ? colorPallets[currentPalette][2] : pixelColor;
      pixelColor = (y > 6) ? colorPallets[currentPalette][3] : pixelColor;
      matrix.drawPixel(x, LEDHeight - 1 - y, pixelColor);
    }
  }
  matrix.show();
}

void drawBirds() {
  if (birds == nullptr) {
    generateBirds(LEDWidth, LEDHeight);
  }
  updateFlock(LEDWidth, LEDHeight);
  matrix.fillScreen(0);
  for (int i = 0; i < birdCount; i++) {
    matrix.drawPixel(birds[i].pixel.x, birds[i].pixel.y, birds[i].color);
  }
  matrix.show();
}

void setBrightness(int newBrightness) {
  brightness = constrain(newBrightness, minBrightness, maxBrightness);
  matrix.setBrightness(brightness);
  matrix.show();
}

void setSensitivity(int newSensitivity) {
  constrain(newSensitivity, minSensitivity, maxSensitivity);
  sensitivity = newSensitivity;
}

void setVisualization(int newMode) {
  visualization = constrain(newMode, 0, maxVisualization);
}

void setFPS(int newFPS) { frameRate = constrain(newFPS, 0, maxFrameRate); }

void scrollText(String text) {
  matrix.setTextColor(textColor);  // Set the text color
  matrix.fillScreen(0);
  int startX = matrix.width();
  int len = text.length() * 6;  // Approx width of a character
  for (int x = startX; x > -len; x--) {
    matrix.fillScreen(0);
    matrix.setCursor(x, 0);
    matrix.print(text);
    matrix.show();
    delay(100 - textSpeed);  // Adjust speed based on textSpeed
  }
}

void initializeWebServer() {
  wifi.webServer.on("/sensitivity", HTTP_GET, []() {
    wifi.webServer.send(200, "text/plain", String(sensitivity));
  });
  wifi.webServer.on("/sensitivity", HTTP_POST, []() {
    if (wifi.webServer.hasArg("value")) {
      int newSensitivity = wifi.webServer.arg("value").toInt();
      setSensitivity(newSensitivity);
      wifi.webServer.send(200, "text/plain", "Sensitivity updated");
    } else {
      wifi.webServer.send(400, "text/plain", "Missing sensitivity value");
    }
  });

  wifi.webServer.on("/brightness", HTTP_GET, []() {
    wifi.webServer.send(200, "text/plain", String(brightness));
  });
  wifi.webServer.on("/brightness", HTTP_POST, []() {
    if (wifi.webServer.hasArg("value")) {
      int newBrightness = wifi.webServer.arg("value").toInt();
      setBrightness(newBrightness);
      wifi.webServer.send(200, "text/plain", "Brightness updated");
    } else {
      wifi.webServer.send(400, "text/plain", "Missing brightness value");
    }
  });

  wifi.webServer.on("/visualization", HTTP_GET, []() {
    if (wifi.webServer.hasArg("mode")) {
      int newMode = wifi.webServer.arg("mode").toInt();
      setVisualization(newMode);
      wifi.webServer.send(200, "text/plain",
                          "Visualization set to: " + String(visualization));
    } else {
      wifi.webServer.send(400, "text/plain", "Missing visualization mode");
    }
  });

  wifi.webServer.on("/text", HTTP_POST, []() {
    if (wifi.webServer.hasArg("textColor")) {
      String color = wifi.webServer.arg("textColor");
      textColor = hexToColor(color);
    }
    if (wifi.webServer.hasArg("textSpeed")) {
      textSpeed = wifi.webServer.arg("textSpeed").toInt();
      wifi.webServer.send(200, "text/plain", "Speed updated");
    }
    if (wifi.webServer.hasArg("text")) {
      text = wifi.webServer.arg("text");
    }
    scrollText(text);
    wifi.webServer.send(200, "text/plain", "Text updated");
  });

  wifi.webServer.on("/birds", HTTP_GET, []() {
    String response = "";
    response += "birdCount=" + String(birdCount) + "\n";
    response += "birdAlignment=" + String(birdAlignment) + "\n";
    response += "birdCohesion=" + String(birdCohesion) + "\n";
    response += "birdSeparation=" + String(birdSeparation) + "\n";
    response += "birdVerticalBounds=" + String(birdVerticalBounds) + "\n";
    response += "birdHorizontalBounds=" + String(birdHorizontalBounds) + "\n";
    wifi.webServer.send(200, "text/plain", response);
  });

  wifi.webServer.on("/birds", HTTP_POST, []() {
    if (wifi.webServer.hasArg("max_velocity")) {
      birdMaxVelocity = wifi.webServer.arg("max_velocity").toFloat();
    }
    if (wifi.webServer.hasArg("min_velocity")) {
      birdMinVelocity = wifi.webServer.arg("min_velocity").toFloat();
    }
    if (wifi.webServer.hasArg("birdCount")) {
      birdCount = wifi.webServer.arg("birdCount").toInt();
    }
    if (wifi.webServer.hasArg("birdAlignment")) {
      birdAlignment = wifi.webServer.arg("birdAlignment").toFloat();
    }
    if (wifi.webServer.hasArg("birdCohesion")) {
      birdCohesion = wifi.webServer.arg("birdCohesion").toFloat();
    }
    if (wifi.webServer.hasArg("birdSeparation")) {
      birdSeparation = wifi.webServer.arg("birdSeparation").toFloat();
    }
    if (wifi.webServer.hasArg("birdVerticalBounds")) {
      birdVerticalBounds =
          wifi.webServer.arg("birdVerticalBounds").compareTo("true") == 0;
    }
    if (wifi.webServer.hasArg("birdHorizontalBounds")) {
      birdHorizontalBounds =
          wifi.webServer.arg("birdHorizontalBounds").compareTo("true") == 0;
    }
    generateBirds(LEDWidth, LEDHeight);
    wifi.webServer.send(200, "text/plain", "Bird settings updated");
  });

  wifi.setConnectSubroutine([]() { testMatrix(&matrix, LEDWidth, LEDHeight); });
  wifi.enableMDNS("lucifi");
  wifi.Start();
}