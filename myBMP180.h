#include <Wire.h>
#include <Adafruit_BMP085.h>
#define seaLevelPressure_hPa 1013.25

Adafruit_BMP085 bmp;
float pressure, temperature;

void myBMP180_initialise() {
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    while (1) {}
  }
}

void myBMP180_read() {
  pressure = bmp.readPressure();
  Serial.print("Pressure = ");
  Serial.print(pressure);
  Serial.println(" Pa");

  temperature = bmp.readTemperature();
  Serial.print("Temperature = ");
  Serial.print(temperature);
  Serial.println(" deg C");
}
