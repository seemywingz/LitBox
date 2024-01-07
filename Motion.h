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
const int motion_i2c_addr = 0x69;

struct MotionObject {
  Body* body;
  uint32_t color;
};

// Motion Animation
int motionNumObjects = 9;
MotionObject* motionObjects = nullptr;

// BMI160 sensor
float gx = 0.0, gy = 0.0, gz = 0.0;
float ax = 0.0, ay = 0.0, az = 0.0;
bool BMI160Initialized = false;
const float rawDataConversion = 32768.0;

// Physics
bool gravityEnabled = true;
World world(Vec2{0.0, 0.0}, 6);

void generateMotionObjects(int maxX, int maxY) {
  world.Clear();

  float wallFriction = 0.0f;
  Body* ground = new Body();
  ground->Set(Vec2{maxX + 3, 1.0f}, FLT_MAX);
  ground->position.Set(maxX / 2.0f, maxY);
  ground->friction = wallFriction;
  world.Add(ground);

  Body* ceiling = new Body();
  ceiling->Set(Vec2{maxX + 3, 1.0f}, FLT_MAX);
  ceiling->position.Set(maxX / 2.0f, -1);
  ceiling->friction = wallFriction;
  world.Add(ceiling);

  Body* leftWall = new Body();
  leftWall->Set(Vec2{1.0f, maxY + 3}, FLT_MAX);
  leftWall->position.Set(-1, maxY / 2.0f);
  leftWall->friction = wallFriction;
  world.Add(leftWall);

  Body* rightWall = new Body();
  rightWall->Set(Vec2{1.0f, maxY + 3}, FLT_MAX);
  rightWall->position.Set(maxX, maxY / 2.0f);
  rightWall->friction = wallFriction;
  world.Add(rightWall);

  motionObjects = new MotionObject[motionNumObjects];
  for (int i = 0; i < motionNumObjects; i++) {
    motionObjects[i].color = colorPallet[random(0, palletSize)];
    Body* b = new Body();
    int w = 1;
    b->Set(Vec2{w, w}, 10);
    b->position.Set(random(0, maxX), random(0, maxY));
    b->friction = 0.3f;
    world.Add(motionObjects[i].body = b);
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

void readGyro() {
  if (!BMI160Initialized) return;
  int gxRaw, gyRaw, gzRaw;
  BMI160.readGyro(gxRaw, gyRaw, gzRaw);
  gx = convertRawGyro(gxRaw);
  gy = convertRawGyro(gyRaw);
  gz = convertRawGyro(gzRaw);
  // Serial.println("g: " + String(gx) + ", " + String(gy) + ", " + String(gz));
}

void readAccelerometer() {
  if (!BMI160Initialized) return;
  int axRaw, ayRaw, azRaw;
  BMI160.readAccelerometer(axRaw, ayRaw, azRaw);
  ax = convertRawAccel(axRaw, BMI160.getXAccelOffset());
  ay = convertRawAccel(ayRaw, BMI160.getYAccelOffset());
  az = convertRawAccel(azRaw, BMI160.getZAccelOffset());
  // Serial.println("a: " + String(ax) + ", " + String(ay) + ", " + String(az));
}

void motionAnimation(int maxX, int maxY, float frameRate) {
  if (motionObjects == nullptr) {
    generateMotionObjects(maxX, maxY);
  }

  readAccelerometer();

  Serial.println("g: " + String(-ay) + ", " + String(-ax));
  world.gravity.Set(-ay, -ax);
  world.Step(0.8f);

  for (int i = 0; i < motionNumObjects; i++) {
    Body* b = motionObjects[i].body;
    b->position.x = constrain(b->position.x, 0, maxX - 1);
    b->position.y = constrain(b->position.y, 0, maxY - 1);
    b->rotation = 0.0f;
  }
}
#endif  // MOTION_H