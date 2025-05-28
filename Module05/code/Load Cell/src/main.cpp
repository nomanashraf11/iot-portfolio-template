#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "HX711.h"

// WiFi & MQTT credentials
const char *ssid = "FirstAid18";
const char *password = "iotempire";
const char *mqtt_server = "192.168.14.1";

// WiFi & MQTT clients
WiFiClient espClient;
PubSubClient client(espClient);

// HX711 pins
#define DOUT D2
#define CLK D3
HX711 scale;

void setup_wifi()
{
  delay(10);
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
}

void reconnect()
{
  while (!client.connected())
  {
    Serial.print("Connecting to MQTT...");
    if (client.connect("LoadCellClient"))
    {
      Serial.println("connected");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 2 seconds");
      delay(2000);
    }
  }
}

void setup()
{
  Serial.begin(115200);

  // Start scale
  scale.begin(DOUT, CLK);
  Serial.println("Initializing scale...");

  scale.set_scale(-1980); // Change based on calibration
  scale.tare();
  Serial.println("Scale tared!");

  // Connect to WiFi & MQTT
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();

  // Get average weight
  float weight = scale.get_units(10);
  Serial.print("Weight: ");
  Serial.print(weight);
  Serial.println(" g");

  // Determine status based on weight
  String status = "";
  if (weight < 6.3)
  {
    status = "Bottle is empty";
  }
  else if (weight < 245)
  {
    status = "Low (Less than half liter)";
  }
  else if (weight < 500)
  {
    status = "Half full (~500ml)";
  }
  else
  {
    status = "Full (~1 liter)";
  }

  // Publish weight
  char weightPayload[16];
  dtostrf(weight, 1, 2, weightPayload);
  client.publish("sensors/weight", weightPayload);

  // Publish status
  client.publish("sensors/status", status.c_str());

  delay(2000); // Wait 2 seconds
}
