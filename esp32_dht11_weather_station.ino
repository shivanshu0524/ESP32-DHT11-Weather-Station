// ==================== BLYNK CONFIG ====================

#define BLYNK_TEMPLATE_ID "YOUR_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "ESP32 Weather Sensor"
#define BLYNK_AUTH_TOKEN "YOUR_AUTH_TOKEN"

// ==================== LIBRARIES ====================

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

// ==================== WIFI CREDENTIALS ====================

char ssid[] = "YOUR_WIFI_NAME";
char pass[] = "YOUR_WIFI_PASSWORD";

// ==================== DHT11 CONFIG ====================

#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// ==================== TIMER ====================

BlynkTimer timer;

// ==================== FUNCTION TO SEND DATA ====================

void sendSensorData() {

  // Read DHT11 values
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Check sensor reading
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("❌ ERROR: Failed to read from DHT11 sensor");
    return;
  }

  // Print values in Serial Monitor
  Serial.println("--------------------------------");

  Serial.print("🌡 Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("💧 Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  // Send values to Blynk
  Blynk.virtualWrite(V0, temperature);
  Blynk.virtualWrite(V1, humidity);

  Serial.println("✅ Data sent to Blynk");
}

// ==================== SETUP ====================

void setup() {

  // Start Serial Monitor
  Serial.begin(115200);
  delay(1000);

  Serial.println("\n================================");
  Serial.println("ESP32 DHT11 Weather Station");
  Serial.println("================================");

  // Start DHT11
  dht.begin();
  Serial.println("✅ DHT11 Initialized");

  // Connect WiFi & Blynk
  Serial.println("\n📶 Connecting to WiFi...");
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("✅ WiFi Connected");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("❌ WiFi Connection Failed");
  }

  if (Blynk.connected()) {
    Serial.println("✅ Blynk Connected");
  } else {
    Serial.println("❌ Blynk Connection Failed");
  }

  // Send data every 2 seconds
  timer.setInterval(2000L, sendSensorData);
}

// ==================== LOOP ====================

void loop() {

  // Keep Blynk running
  Blynk.run();

  // Run timer
  timer.run();

  // Reconnect if disconnected
  if (!Blynk.connected()) {
    Serial.println("⚠ Reconnecting to Blynk...");
    Blynk.connect();
  }
}
