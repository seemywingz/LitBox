#ifndef SPECTRAL_ANALYZER_H
#define SPECTRAL_ANALYZER_H

#include <IOPin.h>
#include <arduinoFFT.h>

#define SAMPLE_RATE 40000  // sampling rate

// Audio Config
IOPin audio(2, INPUT);  // MIC Pin A0
const int maxInput = 1023;
const int minSensitivity = 1;
const int maxSensitivity = 100;
const uint16_t audioSamples = 128;     // This value MUST ALWAYS be a power of 2
const double samplingFrequency = 100;  // Hz, must be less than 10000 due to ADC
const int usableSamples = (audioSamples / 2);
int* spectralData = nullptr;
bool scaling = true;
double vReal[audioSamples];
double vImage[audioSamples];
ArduinoFFT<double> FFT =
    ArduinoFFT<double>(vReal, vImage, audioSamples, samplingFrequency);
int sensitivity = 6;

void logarithmicScaling(int* spectralData, int maxWidth, int maxHeight) {
  for (int i = 0; i < maxWidth; i++) {
    if (spectralData[i] > 0) {
      // Adjust the scaling factor and add an offset
      float scaledValue =
          log10(spectralData[i] + 1) * (maxHeight / log10(maxInput + 1));
      spectralData[i] = static_cast<int>(scaledValue);
    }
  }
}

void peakDetection(int* peakData, int maxWidth, int maxHeight) {
  int avgRange = (usableSamples - 1) /
                 maxWidth;  // Adjust the range, skipping the first bin

  // Start the loop from 1 to skip the first bin
  for (int i = 1; i <= maxWidth; i++) {
    double peak = 0;
    int startFreqBin = (i)*avgRange + 1;  // Adjust start to skip first bin
    int endFreqBin = startFreqBin + avgRange;

    for (int j = startFreqBin; j < endFreqBin && j < usableSamples; j++) {
      if (vReal[j] > peak) {
        peak = vReal[j];
      }
    }
    // Map the peak value to a row on the LED matrix
    peakData[i - 1] = map(peak, 0, maxInput, 0, maxHeight);
  }
}

void spectralAnalyzer(int maxWidth, int maxHeight) {
  for (int i = 0; i < audioSamples; i++) {
    // Scale the audio input according to sensitivity
    vReal[i] = audio.readA() * (sensitivity / 10.0);
    vImage[i] = 0;
  }

  FFT.windowing(FFTWindow::Hamming, FFTDirection::Forward); /* Weigh data */
  FFT.compute(FFTDirection::Forward);                       /* Compute FFT */
  FFT.complexToMagnitude(); /* Compute magnitudes */

  if (spectralData == nullptr) {
    spectralData = new int[maxWidth];
  }
  peakDetection(spectralData, maxWidth, maxHeight);
}
#endif  // SPECTRAL_ANALYZER_H