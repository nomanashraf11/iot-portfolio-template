#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MPR121.h>

Adafruit_MPR121 cap = Adafruit_MPR121();

// LED Pins (D5, D6)
const int ledPins[] = {D5, D6}; // GPIO14 and GPIO12

void setup()
{
  Serial.begin(9600);
  Wire.begin(D2, D1); 

  for (int i = 0; i < 2; i++)
  {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }

  // Initialize MPR121
  if (!cap.begin(0x5A))
  {
    Serial.println("MPR121 not found!");
    while (1)
      ;
  }
  cap.setThresholds(10, 5);
  Serial.println("MPR121 ready!");
}

void loop()
{
  uint16_t touched = cap.touched();

  // Electrode 0 (Aluminum Foil) → LED 1
  if (touched & (1 << 0))
  {
    digitalWrite(ledPins[0], HIGH);
    Serial.println("Foil touched!");
  }
  else
  {
    digitalWrite(ledPins[0], LOW);
  }

  // Electrode 1 (Water) → LED 2
  if (touched & (1 << 1))
  {
    digitalWrite(ledPins[1], HIGH);
    Serial.println("Water touched!");
  }
  else
  {
    digitalWrite(ledPins[1], LOW);
  }

  delay(500);
}