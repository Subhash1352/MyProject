/*
  Heart Rate Monitor using MAX30102 and Arduino
  ---------------------------------------------
  Connections (MAX30102 to Arduino):
    - VIN  -> 3.3V or 5V (based on sensor version)
    - GND  -> GND
    - SCL  -> A5 (on Uno), or dedicated SCL pin
    - SDA  -> A4 (on Uno), or dedicated SDA pin
    - INT  -> Not used (optional, can be left unconnected)

  Library Dependencies:
    - MAX30105 by SparkFun
    - heartRate.h (included in MAX30105 library examples)

  This code initializes the MAX30102 sensor and reads the heart rate
  in beats per minute (BPM), displaying it over Serial Monitor.
*/

#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"

// Global variables for heart rate monitoring
float currentBPM;
int averageBPM;
float currentSPO2;

MAX30105 heartRateSensor;

const byte SAMPLE_SIZE = 4;              // Number of samples for averaging BPM
byte bpmReadings[SAMPLE_SIZE];           // Array to hold recent BPM values
byte currentSampleIndex = 0;             // Index to keep track of sample insertion
long lastBeatTime = 0;                   // Stores the time of the last detected heartbeat

// Function to initialize the MAX30102 sensor
void myMAX30102_initialise() {
  // Attempt to initialize the sensor
  if (!heartRateSensor.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println("MAX30102 not detected. Please check wiring or power.");
    while (1); // Stop execution if sensor not found
  }

  Serial.println("Place your index finger on the sensor with steady pressure.");

  heartRateSensor.setup();                        // Apply default sensor configuration
  heartRateSensor.setPulseAmplitudeRed(0x0A);     // Dim red LED to indicate sensor is active
  heartRateSensor.setPulseAmplitudeGreen(0x00);   // Turn off green LED (not needed for HR monitoring)
}

// Function to read heart rate data from the sensor
void myMAX30102_read() 
{
  long irValue ;
  for(int i=0;i<300;i++)
  {
  irValue = heartRateSensor.getIR();  // Get IR sensor value

  // Check if a heartbeat is detected
  if (checkForBeat(irValue)) {
    long timeSinceLastBeat = millis() - lastBeatTime;
    lastBeatTime = millis();

    // Calculate beats per minute (BPM)
    currentBPM = 60 / (timeSinceLastBeat / 1000.0);

    // Filter out unrealistic BPM values
    if (currentBPM > 20 && currentBPM < 255) {
      bpmReadings[currentSampleIndex++] = (byte)currentBPM;
      currentSampleIndex %= SAMPLE_SIZE; // Reset index if it exceeds sample size

      // Compute average BPM from recent readings
      averageBPM = 0;
      for (byte i = 0; i < SAMPLE_SIZE; i++) {
        averageBPM += bpmReadings[i];
      }
      averageBPM /= SAMPLE_SIZE;
    }
  }
  }

  // Check for poor signal (e.g., finger not on sensor)
  if (irValue < 50000) {
    Serial.print("BPM = ");
    Serial.println(0);
    Serial.print("SPO2 = ");
    Serial.println(0);
  } else {
    Serial.print("BPM = ");
    Serial.println(averageBPM);
    currentSPO2 = (averageBPM/85.0)*100.0;
    Serial.print("SPO2 = ");
    Serial.println(currentSPO2);
  }
}
