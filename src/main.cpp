#include <Arduino.h>
// Define the LED pin (ESP32 built-in LED is usually on pin 2)
#define LED_PIN 2

void setup()
{
  // Initialize the LED pin as an output
  pinMode(LED_PIN, OUTPUT);

  // Optional: Initialize serial communication for debugging
  Serial.begin(115200);
  Serial.println("ESP32 LED Blink Started");
}

void loop()
{
  // Turn the LED on
  digitalWrite(LED_PIN, HIGH);
  Serial.println("LED ON");

  // Wait for 1 second
  delay(1000);

  // Turn the LED off
  digitalWrite(LED_PIN, LOW);
  Serial.println("LED OFF");

  // Wait for 1 second
  delay(1000);
}