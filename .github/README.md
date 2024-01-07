```

                                                                 
                                                                 
`7MMF'      `7MMF'MMP""MM""YMM     `7MM"""Yp,                    
  MM          MM  P'   MM   `7       MM    Yb                    
  MM          MM       MM            MM    dP  ,pW"Wq.`7M'   `MF'
  MM          MM       MM            MM"""bg. 6W'   `Wb `VA ,V'  
  MM      ,   MM       MM            MM    `Y 8M     M8   XMX    
  MM     ,M   MM       MM            MM    ,9 YA.   ,A9 ,V' VA.  
.JMMmmmmMMM .JMML.   .JMML.        .JMMmmmd9   `Ybmd9'.AM.   .MA.
                                                                 
                                                                 

```
------

## Overview


LitBox is an Arduino-based project for driving an LED matrix display. It features various visualizations including waveform, circles, motion detection, text scrolling, birds simulation, the Game of Life, and star pulses. The project is configurable through a web server interface, allowing users to adjust settings like brightness, color, and display mode.

## Hardware Requirements

1. **WeMos Board** with ESP8266 WiFi module.
2. **Microphone** connected to `MIC Pin A0`.
3. **LED Matrix Display**: 32x8 Pixels, connected via `LED Pin D6`.
4. **I2C Devices**: SCL connected to `Pin D1`, SDA connected to `Pin D2`.

## Software Requirements

- [**Arduino IDE**](https://www.arduino.cc/en/software): For compiling and uploading the firmware.
- [**Adafruit NeoPixel Library**](https://github.com/adafruit/Adafruit_NeoPixel): For controlling the LED matrix.
- [**Adafruit GFX Library**](https://github.com/adafruit/Adafruit-GFX-Library): For graphics functions.
- [**ESPWiFi Library**](https://github.com/seemywingz/ESPWiFi): For handling WiFi and web server functionalities.
- [**ArduinoFFT Library**](https://github.com/kosme/arduinoFFT): For Fourier Transform operations in spectral analysis.

## Features

- **Multiple Visualizations**: Waveform, circles, motion, text, birds, Game of Life, star pulse, and bars.
- **Web Server Control**: Configure settings like brightness, color, sensitivity, and display modes.
- **Motion Detection**: Using a BMI160 sensor, it can detect and visualize motion.
- **Spectral Analyzer**: Visualizes audio input in different formats.
- **Game of Life**: Simulates Conway's Game of Life on the LED matrix.
- **Temperature Sensing**: Displays temperature readings (Celsius or Fahrenheit).

## Setup and Installation

1. **Hardware Assembly**: Connect all hardware components as per the requirements.
2. **Software Setup**: Install the required libraries in the Arduino IDE.
3. **Firmware Upload**: Compile and upload the provided code to the WeMos board.
4. **Configuration**: Connect to the web server hosted by the WeMos board to configure the settings.

## Usage

Once powered on, the LED matrix will start displaying the default visualization. You can change the visualization and settings through the web server interface. The web server allows you to control the brightness, sensitivity, frame rate, and specific settings for each visualization mode.

## Contributing

Contributions to the project are welcome. You can contribute by improving the code, adding new features, fixing bugs, or improving this documentation.

## License

This project is open-source and is licensed under the [MIT License](https://opensource.org/licenses/MIT).

---

**Note**: This README is a brief overview of the project. For detailed implementation and customization, refer to the inline comments in the source code.
