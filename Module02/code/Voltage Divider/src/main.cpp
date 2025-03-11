#include <Arduino.h>

const int adcPin = A0;
const float R1 = 30000.0;   
const float R2 = 10000.0;  
float vRef = 3.3;           

void setup() {
  Serial.begin(9600);
}

void loop() {
  int adcValue = analogRead(adcPin);
  float vADC = (adcValue * vRef) / 1023.0; 
  float vin = vADC * (R1 + R2) / R2;     

  Serial.print("ADC: ");
  Serial.print(adcValue);
  Serial.print("\tVoltage: ");
  Serial.print(vin, 2); 
  Serial.println(" V");
  
  delay(2000);
}