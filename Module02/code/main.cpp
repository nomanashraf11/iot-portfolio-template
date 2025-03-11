#include <Arduino.h>
#include <Servo.h>

// Define pins
#define LED_PIN 2
#define PWM_LED_PIN 4
#define SERVO_PIN 5
#define ANALOG_PIN 34

Servo myServo;

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  pinMode(PWM_LED_PIN, OUTPUT);
  Serial.begin(115200);
  myServo.attach(SERVO_PIN);
}

void loop()
{
  blinkLED();
  fadeLED();
  readAnalog();
  controlServo();
}

void blinkLED()
{
  digitalWrite(LED_PIN, HIGH);
  delay(500);
  digitalWrite(LED_PIN, LOW);
  delay(500);
}

void fadeLED()
{
  for (int i = 0; i <= 255; i++)
  {
    analogWrite(PWM_LED_PIN, i);
    delay(5);
  }
  for (int i = 255; i >= 0; i--)
  {
    analogWrite(PWM_LED_PIN, i);
    delay(5);
  }
}

void readAnalog()
{
  int rawValue = analogRead(ANALOG_PIN);
  float voltage = rawValue * (3.3 / 4095.0); // Map ADC to voltage for ESP32
  Serial.print("Analog Value: ");
  Serial.print(rawValue);
  Serial.print(" | Voltage: ");
  Serial.println(voltage, 2);
  delay(1000);
}

void controlServo()
{
  myServo.write(0);
  delay(1000);
  myServo.write(90);
  delay(1000);
  myServo.write(180);
  delay(1000);
}
