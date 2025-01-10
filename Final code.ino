#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "MAX30105.h"

// OLED definitions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1 // Set to -1 for automatic reset
#define SCREEN_ADDRESS 0x3C

// Create an instance for the OLED and MAX30102 sensor
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
MAX30105 particleSensor;

// Heart rate variables
int32_t heartRate = 0; // Heart rate value
int8_t validHeartRate = 0; // Heart rate validity flag

// Chart data array (to store heart rate values)
#define MAX_POINTS 123
int chartData[MAX_POINTS];
int chartIndex = 0;

void setup() {
  Serial.begin(115200); // Start serial communication for debugging
  Wire.begin();

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (1); // Halt execution if OLED initialization fails
  }

  // Initialize the OLED display
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("Initializing MAX30102..."));
  display.display();

  // Initialize MAX30102 sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_STANDARD)) {
    Serial.println(F("MAX30102 not detected. Check wiring."));
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println(F("MAX30102 error!"));
    display.display();
    while (1); // Halt execution if the sensor initialization fails
  }

  // Configure the MAX30102 sensor
  configureSensor();

  // Display ready message on OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(F("MAX30102 Ready!"));
  display.display();
  Serial.println(F("Setup complete!"));

  // Initialize chart data
  for (int i = 0; i < MAX_POINTS; i++) {
    chartData[i] = 0;
  }
}

void loop() {
  // Measure heart rate
  heartRate = getHeartRate();

  // Display the heart rate on the OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print(F("Heart Rate: "));
  display.print(heartRate);
  display.println(F(" bpm"));

  // Update chart with heart rate value
  updateChart(heartRate);

  // Display the chart
  drawChart();
  display.display();

  // Send the heart rate to the Serial Plotter
  Serial.print("HeartRate: ");
  Serial.println(heartRate);

  // Shorten the delay to update more frequently
  delay(200); // Adjust delay to 200 ms for more frequent updates
}

// Function to configure MAX30102 sensor
void configureSensor() {
  byte ledBrightness = 60; // Adjust LED brightness (0-255)
  byte sampleAverage = 4; // Average samples (1, 2, 4, 8, 16, 32)
  byte ledMode = 2; // LED mode: 2 = Red + IR
  byte sampleRate = 100; // Sampling rate in Hz (50, 100, 200, etc.)
  int pulseWidth = 411; // Pulse width (69, 118, 215, 411)
  int adcRange = 4096; // ADC range (2048, 4096, 8192, 16384)

  // Apply the sensor configuration settings
  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange);
  Serial.println(F("Sensor configured successfully"));
}

// Function to get heart rate from MAX30102
int32_t getHeartRate() {
  uint16_t irBuffer[100]; // Infrared data buffer
  uint16_t redBuffer[100]; // Red data buffer
  int bufferLength = 100; // Data length

  // Read the first 100 samples
  for (byte i = 0; i < bufferLength; i++) {
    while (particleSensor.available() == false) { // Check for new data
      particleSensor.check();
    }
    redBuffer[i] = particleSensor.getRed();
    irBuffer[i] = particleSensor.getIR();
    particleSensor.nextSample(); // Move to the next sample
  }

  // DEBUG: Print the raw sensor readings to the serial monitor
  Serial.print("Red: ");
  Serial.print(redBuffer[0]);
  Serial.print(" IR: ");
  Serial.println(irBuffer[0]);

  // Calculate the heart rate from the raw values
  int32_t calculatedHeartRate = calculateHeartRate(redBuffer, irBuffer, bufferLength);

  // Return the heart rate or 0 if invalid
  return calculatedHeartRate;
}

// Simple calculation method for heart rate (based on signal fluctuations)
int32_t calculateHeartRate(uint16_t* redBuffer, uint16_t* irBuffer, int length) {
  int32_t heartRate = 0;

  // Basic peak detection algorithm 
  for (int i = 1; i < length - 1; i++) {
    if (irBuffer[i] > irBuffer[i - 1] && irBuffer[i] > irBuffer[i + 1]) {
      heartRate++; // Count peaks
    }
  }

  // Estimate heart rate based on peak count
  heartRate = heartRate * 60 / (length / 2); // Simple estimate of BPM
  return heartRate;
}

// Function to update chart with heart rate value
void updateChart(int value) {
  chartData[chartIndex] = value; // Store new heart rate value
  chartIndex++;
  if (chartIndex >= MAX_POINTS) {
    chartIndex = 0; // Reset chart index when full
  }
}

// Function to draw the chart
void drawChart() {
  // Draw the chart area (clear the chart area)
  display.drawRect(0, 20, 128, 44, SSD1306_WHITE); // Chart border

  // Draw the graph by connecting the points
  for (int i = 1; i < MAX_POINTS; i++) {
    int x1 = i - 1;
    int y1 = map(chartData[x1], 0, 100, 0, 44); // Map value to chart height
    int x2 = i;
    int y2 = map(chartData[x2], 0, 100, 0, 44); // Map value to chart height
    display.drawLine(x1, 64 - y1 - 20, x2, 64 - y2 - 20, SSD1306_WHITE); // Draw line
  }
}
