#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <analogWrite.h>
#include <dht.h>
dht DHT;
#define DHT11_PIN 7 // Humidity Pin
int Humidity = 0;
int pin = 2; // Pin to which the sprinkler is connected to

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "C2ZNCPRpY58xr0gfXH5JpWOwLGQTKV1Q";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Random";
char pass[] = "";

void setup()
{
  // Debug console
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);  // Starting connection
  pinMode(pin, OUTPUT); 
}

void loop()
{
  Blynk.run();  // allowing blynk to run continously
  Humidity = DHT.humidity;
  if (Humidity > 128){
    digitalWrite(pin, LOW);
  }
  else {
    digitalWrite(pin, HIGH);
  }
}

BLYNK_READ(V2) // Read from Button - to sprinkle water
{
  int coming = param.asInt();   // taking the data from terminal
  Serial.println(coming);    // printing it on serial monitor
  if (coming > 128) {
    digitalWrite(pin, HIGH);
  }
  else {
    digitalWrite(pin, LOW);
  }
}

BLYNK_READ(V4) // Read from Button - start / stop
{
  int coming = param.asInt();   // taking the data from terminal
  Serial.println(coming);    // printing it on serial monitor
  if (coming > 128) {
    digitalWrite(pin, LOW);
  }
}

BLYNK_WRITE(V3) // Write the Humidity Value
{
  Blynk.virtualWrite(V3, Humidity);   // printing that data into terminal
}
