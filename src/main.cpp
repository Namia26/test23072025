#include <WiFi.h>
#include <HTTPClient.h>
#include <Update.h>
#include <ArduinoJson.h>

const char *ssid = "RecoveGroupWifi";
const char *password = "Rec0ve2016";

const int ledPin = 2;

const char *currentVersion = "1.0.1";
const char *json_url = "https://raw.githubusercontent.com/Namia26/test23072025/main/firmware_info.json";

void performOTA(const char *binURL)
{
  WiFiClient client;
  HTTPClient http;
  http.begin(client, binURL);

  int httpCode = http.GET();
  if (httpCode == 200)
  {
    int len = http.getSize();
    bool canBegin = Update.begin(len);

    if (canBegin)
    {
      WiFiClient *stream = http.getStreamPtr();
      size_t written = Update.writeStream(*stream);

      if (Update.end() && Update.isFinished())
      {
        Serial.println("✅ OTA SUCCESS! Rebooting...");
        delay(1000);
        ESP.restart();
      }
      else
      {
        Serial.println("❌ OTA failed during write/finish.");
      }
    }
    else
    {
      Serial.println("❌ OTA cannot begin. Not enough space?");
    }
  }
  else
  {
    Serial.printf("❌ Failed to download firmware. HTTP code: %d\n", httpCode);
  }

  http.end();
}

void checkForOTAUpdate()
{
  Serial.println("🔍 Checking for OTA update...");

  HTTPClient http;
  http.begin(json_url);
  int httpCode = http.GET();

  if (httpCode == 200)
  {
    String payload = http.getString();
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, payload);

    if (error)
    {
      Serial.println("❌ Failed to parse firmware_info.json");
      return;
    }

    const char *latestVersion = doc["version"];
    const char *firmwareURL = doc["firmware"];

    Serial.printf("📦 Current: %s | Latest: %s\n", currentVersion, latestVersion);

    if (strcmp(currentVersion, latestVersion) != 0)
    {
      Serial.println("🚀 New firmware available! Updating...");
      performOTA(firmwareURL);
    }
    else
    {
      Serial.println("👍 Already up to date.");
    }
  }
  else
  {
    Serial.printf("❌ Failed to fetch version file. HTTP code: %d\n", httpCode);
  }

  http.end();
}

void setup()
{
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  Serial.println("\n📡 Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.println("\n✅ Wi-Fi connected!");
  checkForOTAUpdate();
}

void loop()
{
  // Blink LED to indicate v1.0.1 is running
  digitalWrite(ledPin, HIGH);
  delay(500);
  digitalWrite(ledPin, LOW);
  delay(500);
}
