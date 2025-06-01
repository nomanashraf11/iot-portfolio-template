#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char *ssid = "router11";
const char *password = "iotempire";
const char *mqtt_server = "192.168.14.1";

WiFiClient espClient;
PubSubClient client(espClient);

const int relayPin = D1; // Or whatever GPIO you're using

void setup_wifi()
{
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
    delay(500);
}

void callback(char *topic, byte *payload, unsigned int length)
{
  String msg;
  for (int i = 0; i < length; i++)
    msg += (char)payload[i];

  if (msg == "ON")
    digitalWrite(relayPin, HIGH); // LOW = ON for active-low relays
  else if (msg == "OFF")
    digitalWrite(relayPin, LOW);
}

void reconnect()
{
  while (!client.connected())
  {
    if (client.connect("ESP_Motor_Controller"))
    {
      client.subscribe("home/motor");
    }
    else
    {
      delay(5000);
    }
  }
}

void setup()
{
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW); // Relay OFF by default

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop()
{
  if (!client.connected())
    reconnect();
  client.loop();
}
