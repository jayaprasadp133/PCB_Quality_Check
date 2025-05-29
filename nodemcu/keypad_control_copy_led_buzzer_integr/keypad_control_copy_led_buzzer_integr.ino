#include <ESP8266WiFi.h>
#include <Keypad.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>  // Make sure this is included at the top

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ArduinoJson.h>


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

WiFiClient wifiClient;   // Declare this globally or at the top of your code

const char* ssid = "Redmi";     // Replace with your mobile hotspot SSID
const char* password = "123456789";  // Replace with your mobile hotspot passwor

const String server = "http://192.168.189.165:5000";  // http://192.168.41.165:5000 http://192.168.111.165:5000 http://192.168.59.165:5000 Replace with your Flask server IP http://192.168.145.165:5000

const int buzzerPin = D7;
const int greenLedPin = D6;
const int redLedPin = D5; 

// Keypad config
const byte ROWS = 1;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {D8};  // R4
byte colPins[COLS] = {D0, D3, D4, D5};  // C1, C2, C3, C4

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);


void setup() {
  Serial.begin(115200);
  pinMode(buzzerPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);
  digitalWrite(greenLedPin, LOW);
  digitalWrite(redLedPin, LOW);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED init failed");
    while (1);
  }
  display.clearDisplay();
  display.display();


  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
}

void loop() {
  char key = keypad.getKey();

  if (key) {
    Serial.print("Key Pressed: ");
    Serial.println(key);

    if (key == '*') {
      triggerAnalysis();
    } else if (key == 'C') {
      showEdges();
    }
  }
}

void triggerAnalysis() {
  HTTPClient http;
  String url = server + "/trigger_analysis";
  http.begin(wifiClient, url);
  http.addHeader("Content-Type", "application/json");

  int httpResponseCode = http.POST("{}");

  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println("Trigger response: " + response);

    // Parse JSON
    StaticJsonDocument<256> doc;
    DeserializationError error = deserializeJson(doc, response);
    if (error) {
      Serial.println("JSON Parsing failed");
      return;
    }

    const char* message = doc["message"];
    int result = doc["result"];

    // Display message on OLED
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    display.print(message);
    display.display();

    if (result == 0) {
      // PCB is perfect
      blinkGreenLED();
    } else {
      // PCB is defective
      buzzAlarm();
    }

  } else {
    Serial.println("Failed to trigger analysis.");
  }

  http.end();
}

void showEdges() {
  HTTPClient http;
  String url = server + "/show_edges";
  http.begin(wifiClient, url);
  int httpResponseCode = http.GET();

  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println("Show Edges response: " + response);
  } else {
    Serial.println("Failed to show edges.");
  }

  http.end();
}

void blinkGreenLED() {
  for (int i = 0; i < 6; i++) {
    digitalWrite(greenLedPin, HIGH);
    delay(300);
    digitalWrite(greenLedPin, LOW);
    delay(300);
  }
}

void buzzAlarm() {
  for (int i = 0; i < 6; i++) {
    digitalWrite(buzzerPin, HIGH);
    delay(300);
    digitalWrite(buzzerPin, LOW);
    delay(300);

  }
}