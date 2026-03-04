#include "myBMP180.h"
#include "myMAX30102.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  myBMP180_initialise();
  myMAX30102_initialise();

}

void loop() {
  // put your main code here, to run repeatedly:
  myBMP180_read();
  myMAX30102_read();

  float gsrData = analogRead(A0)*3.3/1023.0;
  Serial.print("Stress level = ");
  Serial.println(gsrData);

  delay(2000);

}
