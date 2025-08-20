#include <ESP8266WiFi.h>
#include <DHT.h>

// 🔹 WiFi credentials
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

// 🔹 ThingSpeak settings
const char* server = "api.thingspeak.com";
String apiKey = "YOUR_WRITE_API_KEY";

// 🔹 DHT sensor setup
#define DHTPIN D4        // GPIO2 (NodeMCU D4 pin)
#define DHTTYPE DHT11    // DHT11 sensor
DHT dht(DHTPIN, DHTTYPE);

WiFiClient client;

void setup() {
  Serial.begin(115200);
  delay(10);

  dht.begin();

  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ WiFi connected");
}

void loop() {
  float temperature = dht.readTemperature(); // Celsius
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("⚠️ Failed to read from DHT sensor!");
    return;
  }

  if (client.connect(server, 80)) {
    String url = "/update?api_key=" + apiKey +
                 "&field1=" + String(temperature) +
                 "&field2=" + String(humidity);

    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + server + "\r\n" +
                 "Connection: close\r\n\r\n");

    Serial.println("✅ Data sent → Temp: " + String(temperature) + "°C, Humidity: " + String(humidity) + "%");
  }

  client.stop();

  delay(15000); // ThingSpeak free limit (15 sec)
}
