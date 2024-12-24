#include <Wire.h>
#include <WiFi.h>
#include <WebServer.h>
#include <DHTesp.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// WiFi credentials
const char* ssid = "techno";
const char* password = "techno@123";

// Sensor pins
#define DHTPIN 18
#define ONE_WIRE_BUS 5
#define HEARTBEAT_PIN 34  // Analog pin for the heartbeat sensor

// Create DHT sensor object
DHTesp dht;

// Create DS18B20 sensor object
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Web server on port 80
WebServer server(80);

// Timing variables
uint32_t lastReportTime = 0;
const uint32_t reportingPeriod = 1000;

// Forward declarations
void handleRoot();
void setupSensors();
void updateSensorReadings();
String getWeatherCondition(float temperature, float humidity);

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());

  // Start web server
  server.on("/", handleRoot);
  server.begin();

  // Setup sensors
  setupSensors();
}

void loop() {
  server.handleClient();

  // Report every reportingPeriod milliseconds
  if (millis() - lastReportTime >= reportingPeriod) {
    lastReportTime = millis();
    updateSensorReadings();
  }
}

void setupSensors() {
  // Initialize DHT sensor
  dht.setup(DHTPIN, DHTesp::DHT11);

  // Initialize DS18B20 sensor
  sensors.begin();

  // Initialize heartbeat sensor pin
  pinMode(HEARTBEAT_PIN, INPUT);
}

void updateSensorReadings() {
  // Read DHT sensor
  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();

  // Read DS18B20 sensor
  sensors.requestTemperatures();
  float tempDS = sensors.getTempCByIndex(0);

  // Read heartbeat sensor
  int heartbeatValue = analogRead(HEARTBEAT_PIN);
  float bpm = map(heartbeatValue, 0, 4095, 0, 150); // Assuming bpm range from 0 to 150

  // Output to serial for debugging
  Serial.print("Temperature DHT: ");
  Serial.print(temperature);
  Serial.print(" °C, Humidity: ");
  Serial.print(humidity);
  Serial.print(" %, Temperature DS: ");
  Serial.print(tempDS);
  Serial.print(" °C, BPM: ");
  Serial.println(bpm);

  // Output to web page
  server.handleClient();
}

void handleRoot() {
  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();
  float tempDS = sensors.getTempCByIndex(0);
  int heartbeatValue = analogRead(HEARTBEAT_PIN);
  float bpm = map(heartbeatValue, 0, 4095, 0, 150); // Assuming bpm range from 0 to 150

  // Determine weather condition
  String weatherCondition = getWeatherCondition(temperature, humidity);

  String html = "<html><head><title>Integrating Sensors with Web Server</title>";
  html += "<style>";
  html += "body { font-family: Arial, sans-serif; text-align: center; color: #333; position: relative; }";
  html += "h1 { color: #0066cc; }";
  html += ".sensor-box { display: inline-block; background: rgba(255, 255, 255, 0.8); padding: 20px; margin: 20px; border-radius: 10px; box-shadow: 0 0 10px rgba(0,0,0,0.1); }";
  html += "p { font-size: 1.2em; }";

  // Set background color based on weather condition
  if (weatherCondition == "clear") {
    html += "body { background-color: #87CEEB; }"; // Light blue for clear weather
  } else if (weatherCondition == "rainy") {
    html += "body { background-color: #B0C4DE; }"; // Light steel blue for rainy weather
  } else if (weatherCondition == "windy") {
    html += "body { background-color: #87CEFA; }"; // Light sky blue for windy weather
  }

  // Set weather icon position
  html += ".weather-icon { position: absolute; font-size: 3em; color: white; }";
  if (weatherCondition == "clear") {
    html += ".weather-icon { top: 5%; right: 5%; color: yellow; }"; // Sunny
  } else if (weatherCondition == "rainy") {
    html += ".weather-icon { bottom: 5%; left: 5%; color: #555; }"; // Rainy
  } else if (weatherCondition == "windy") {
    html += ".weather-icon { bottom: 5%; left: 50%; transform: translateX(-50%); color: #555; }"; // Windy
  }

  html += "</style>";
  html += "</head><body>";

  // Display weather icon
  if (weatherCondition == "clear") {
    html += "<div class='weather-icon'>&#9728;</div>"; // Sunny
  } else if (weatherCondition == "rainy") {
    html += "<div class='weather-icon'>&#9729;</div>"; // Rainy
  } else if (weatherCondition == "windy") {
    html += "<div class='weather-icon'>&#9729;</div>"; // Dark clouds for windy weather
  }

  html += "<h1>Integrating Sensors with Web Server</h1>";
  html += "<div class='sensor-box'><p>Temperature (DHT11): " + String(temperature) + " &deg;C</p>";
  html += "<p>Humidity (DHT11): " + String(humidity) + " %</p>";
  html += "<p>Temperature (DS18B20): " + String(tempDS) + " &deg;C</p>";
  html += "<p>BPM (Heartbeat Sensor): " + String(bpm) + "</p></div>";

  html += "</body></html>";
  server.send(200, "text/html", html);
}

String getWeatherCondition(float temperature, float humidity) {
  // You can adjust these conditions based on your preferences
  if (temperature > 25 && humidity > 70) {
    return "rainy"; // Hot and humid
  } else if (temperature > 15 && temperature <= 25 && humidity > 50) {
    return "windy"; // Mild and humid
  } else {
    return "clear"; // Default to clear weather
  }
}
