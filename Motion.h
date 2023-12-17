#ifndef MOTION_H
#define MOTION_H

#define PHYSAC_IMPLEMENTATION
#include <BMI160Gen.h>
#include <Body.h>
#include <World.h>

#include "Colors.h"
#include "Utils.h"

// SDA/D2
// SCL/D1

struct MotionObject : public Pixel {
  Body* body;
};

int motionNumObjects = 3;
bool gravityEnabled = true;
float energyLossFactor = 0.99;
float gx = 0, gy = 0, gz = 0;
float ax = 0, ay = 0, az = 0;
bool BMI160Initialized = false;
MotionObject* motionObjects = nullptr;
const int motion_i2c_addr = 0x69;
const float rawDataConversion = 32768.0;

// Physics
float timeStep = 1.0f / 60.0f;
Vec2 gravity(0.0f, -10.0f);
int iterations = 10;
World world(gravity, iterations);

void generateMotionObjects(int maxX, int maxY) {
  motionObjects = new MotionObject[motionNumObjects];
  for (int i = 0; i < motionNumObjects; i++) {
    motionObjects[i].color = colorPallet[random(0, palletSize)];
    motionObjects[i].x = random(1, maxX - 1);
    motionObjects[i].y = random(1, maxY - 1);
    motionObjects[i].vx = 0;
    motionObjects[i].vy = 0;
    world.Add(motionObjects[i].body = new Body());
    motionObjects[i].body->position.Set(motionObjects[i].x, motionObjects[i].y);
  }
}

void initializeMotion(int maxX, int maxY) {
  if (checkI2CDevice(motion_i2c_addr)) {
    if (BMI160.begin(BMI160GenClass::I2C_MODE, motion_i2c_addr)) {
      generateMotionObjects(maxX, maxY);
      Serial.print("BMI160 Auto Calibration Starting");
      BMI160.autoCalibrateXAccelOffset(1);
      BMI160.autoCalibrateYAccelOffset(1);
      BMI160.autoCalibrateZAccelOffset(1);
      BMI160.autoCalibrateGyroOffset();
      delay(250);
      Serial.print("BMI160 Auto Calibration Complete");
      BMI160Initialized = true;
    } else {
      BMI160Initialized = false;
      Serial.print("BMI160 initialization failed!");
    }
  } else {
    BMI160Initialized = false;
    Serial.print("BMI160 sensor not detected at the specified I2C address:" +
                 String(motion_i2c_addr));
  }
}

float getTemperature(String unit) {
  if (!BMI160Initialized) return 0;
  int16_t rawTemp = BMI160.getTemperature();  // returns a 16-bit integer
  // The temperature data is a signed 16-bit value where 0x0000 corresponds to
  // 23°C, and each least significant bit (LSB) represents approximately
  // 0.00195°C.
  float tempC = 23.0 + ((float)rawTemp) * 0.00195;
  if (unit == "F") {
    float tempF = tempC * 9.0 / 5.0 + 32.0;  // Convert to Fahrenheit
    return tempF;
  }
  return tempC;
}

float convertRawGyro(int gRaw) {
  // convert the raw gyro data to degrees/second
  // we are using 250 degrees/seconds range
  // -250 maps to a raw value of -32768
  // +250 maps to a raw value of 32767
  return (gRaw * 250.0) / rawDataConversion;
}

float convertRawAccel(int raw, int offset) {
  // Assuming the full scale range is ±2g
  // Scale Factor for ±2g = 32768/2 = 16384
  return (raw - offset) / (rawDataConversion / BMI160.getAccelerometerRange());
}

void readSensors() {
  if (!BMI160Initialized) return;
  int gxRaw, gyRaw, gzRaw;
  int axRaw, ayRaw, azRaw;
  BMI160.readGyro(gxRaw, gyRaw, gzRaw);
  BMI160.readAccelerometer(axRaw, ayRaw, azRaw);

  gx = convertRawGyro(gxRaw);
  gy = convertRawGyro(gyRaw);
  gz = convertRawGyro(gzRaw);

  ax = convertRawAccel(axRaw, BMI160.getXAccelOffset());
  ay = convertRawAccel(ayRaw, BMI160.getYAccelOffset());
  az = convertRawAccel(azRaw, BMI160.getZAccelOffset());

  // Serial.println("g: " + String(gx) + ", " + String(gy) + ", " + String(gz));
  // Serial.println("a: " + String(ax) + ", " + String(ay) + ", " + String(az));
}

void motionAnimation(int maxX, int maxY) {
  if (motionObjects == nullptr) {
    generateMotionObjects(maxX, maxY);
  }
  world.Step(timeStep);
  readSensors();
  float scale = 0.001;
  float gravityMagnitude = 1.1;
  float gravityX = -ay * gravityMagnitude;
  float gravityY = -ax * gravityMagnitude;
  for (int i = 0; i < motionNumObjects; i++) {
    if (gravityEnabled) {
      motionObjects[i].vx = gravityX;
      motionObjects[i].vy = gravityY;
    } else {
      motionObjects[i].vx += -gx * scale;
      motionObjects[i].vy += gy * scale;
      motionObjects[i].vx *= energyLossFactor;
      motionObjects[i].vy *= energyLossFactor;
    }

    Serial.println("x: " + String(motionObjects[i].body->position.x) +
                   ", y: " + String(motionObjects[i].body->position.y));

    motionObjects[i].x =
        constrain(motionObjects[i].x += motionObjects[i].vx, 0, maxX - 1);
    motionObjects[i].y =
        constrain(motionObjects[i].y += motionObjects[i].vy, 0, maxY - 1);
  }
}
#endif  // MOTION_H