#include <WiFi.h>
#include <HTTPClient.h>
#include <LiquidCrystal_I2C.h>
#include <ArduinoJson.h>

// Replace with your network credentials
const char* ssid = "Don";
const char* password = "11052002";

// Firebase project details
#define FIREBASE_HOST "wastemanagement-1aa76-default-rtdb.firebaseio.com"  // Firebase Database URL
#define FIREBASE_AUTH "PKIB89JSgz5Oy3I4FCVkkYsRGnuK3u7fU5DK6D4W"           // Firebase Database Secret

// Google API key
const char* apiKey = "AIzaSyDCGfgoNdcA_uoTJZ80uty1yzs-WVdTzbM";

// LCD setup (I2C address 0x27, 16 columns, 2 rows)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pin definitions
#define TRIG_PIN 5    // Ultrasonic Trig pin connected to GPIO 5
#define ECHO_PIN 18   // Ultrasonic Echo pin connected to GPIO 18

void setup() {
  Serial.begin(115200);             // Serial monitor
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Initialize LCD
  lcd.init();             // Initialize the LCD
  lcd.backlight();        // Turn on the backlight
  lcd.setCursor(0, 0);
  lcd.print(F("Smart Bin Init"));  // Store string in flash memory

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println(F("Connecting to WiFi..."));
    lcd.setCursor(0, 1);
    lcd.print(F("."));
  }
  Serial.println(F("Connected to WiFi"));
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("WiFi Connected"));
  delay(2000);
}

void loop() {
  // Ultrasonic Sensor Logic to measure distance
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  float distance = duration * 0.034 / 2;  // Distance in cm

  String binStatus;

  // Determine bin level based on distance
  if (distance > 15) {
    binStatus = "LOW";     // Waste level is low when distance is more than 15 cm
  } else if (distance > 5 && distance <= 15) {
    binStatus = "MEDIUM";  // Waste level is medium when distance is between 5 cm and 15 cm
  } else if (distance <= 5) {
    binStatus = "FULL";    // Waste level is full when distance is 5 cm or less
  }

  // Display bin status on LCD
  lcd.clear();                  // Clear the display
  lcd.setCursor(0, 0);
  lcd.print(F("Bin Status: "));
  lcd.print(binStatus);

  Serial.print(F("Bin Status: "));
  Serial.println(binStatus);

  // Upload bin status to Firebase
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    
    // Send bin status
    String binStatusPath = "https://" FIREBASE_HOST "/smartBin/binStatus.json?auth=" FIREBASE_AUTH;
    String payload = "{\"binStatus\": \"" + binStatus + "\"}";
    http.begin(binStatusPath);
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST(payload);
    http.end();

    // Send distance
    String distancePath = "https://" FIREBASE_HOST "/smartBin/distance.json?auth=" FIREBASE_AUTH;
    payload = "{\"distance\": " + String(distance) + "}";
    http.begin(distancePath);
    http.addHeader("Content-Type", "application/json");
    httpResponseCode = http.POST(payload);
    http.end();

    // Send timestamp
    String timestampPath = "https://" FIREBASE_HOST "/smartBin/lastUpdated.json?auth=" FIREBASE_AUTH;
    http.begin(timestampPath);
    http.addHeader("Content-Type", "application/json");
    payload = "{}";  // Send an empty object for the timestamp
    httpResponseCode = http.POST(payload);
    http.end();
  }

  // If the bin is full, get geolocation using Google API
  if (binStatus == "FULL") {
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;

      // Google Geolocation API URL
      String url = "https://www.googleapis.com/geolocation/v1/geolocate?key=AIzaSyDCGfgoNdcA_uoTJZ80uty1yzs-WVdTzbM";
      http.begin(url); // Specify the URL
      http.addHeader("Content-Type", "application/json"); // Specify content type

      // Empty request body
      String payload = "{\"considerIp\": \"true\"}";

      // Send the request
      int httpResponseCode = http.POST(payload);

      if (httpResponseCode > 0) {
        // Get the response
        String response = http.getString();
        Serial.println("HTTP Response code: " + String(httpResponseCode));
        Serial.println("Response: " + response); // Print the full response for debugging

        // Parse the JSON response to get the latitude and longitude
        StaticJsonDocument<512> doc;
        deserializeJson(doc, response);

        float latitude = doc["location"]["lat"];
        float longitude = doc["location"]["lng"];

        // Display the GPS location on Serial Monitor
        Serial.print(F("Latitude: "));
        Serial.println(latitude, 2);
        Serial.print(F("Longitude: "));
        Serial.println(longitude, 2);

        // Clear the LCD and show the latitude and longitude
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Lat: "));
        lcd.print(latitude, 2); // Limit to 2 decimal places
        lcd.setCursor(0, 1);
        lcd.print(F("Lng: "));
        lcd.print(longitude, 2); // Limit to 2 decimal places
      } else {
        Serial.print(F("Error in HTTP request: "));
        Serial.println(httpResponseCode);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Error in request"));
      }

      // End the request
      http.end();
    } else {
      Serial.println(F("WiFi Disconnected"));
      lcd.setCursor(0, 0);
      lcd.print(F("WiFi Disconnected"));
    }
  } else {
    lcd.setCursor(0, 1);
    lcd.print(F("Distance: "));
    lcd.print(distance);
    lcd.print(F(" cm"));
  }

  delay(2000);  // Delay between readings
}
