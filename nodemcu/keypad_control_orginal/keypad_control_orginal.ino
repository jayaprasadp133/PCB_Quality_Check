#include <ESP8266WiFi.h>
#include <Keypad.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>  // Make sure this is included at the top

WiFiClient wifiClient;   // Declare this globally or at the top of your code

const char* ssid = "vivo1";     // Replace with your mobile hotspot SSID
const char* password = "123456789";  // Replace with your mobile hotspot passwor

const String server = "http://192.168.59.165:5000";  //http://192.168.111.165:5000 http://192.168.59.165:5000 Replace with your Flask server IP http://192.168.145.165:5000

const int buzzerPin = D1;
const int ledPin = D2;

// Keypad config
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {D5, D6, D7, D8};  // R1, R2, R3, R4
byte colPins[COLS] = {D0, D3, D4, D9};  // C1, C2, C3, C4

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  Serial.begin(115200);
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);
  digitalWrite(ledPin, LOW);

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
  http.begin(wifiClient, url); // ✅ Updated line

  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST("{}");

  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println("Trigger response: " + response);
  } else {
    Serial.println("Failed to trigger analysis.");
  }

  http.end();
}
void showEdges() {
  HTTPClient http;
  String url = server + "/show_edges";
  http.begin(wifiClient, url); // ✅ Updated line

  int httpResponseCode = http.GET();
  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println("Show Edges response: " + response);
  } else {
    Serial.println("Failed to show edges.");
  }

  http.end();
}
