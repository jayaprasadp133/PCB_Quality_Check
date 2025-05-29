#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3C
#define BUZZER_PIN D10

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(9600);
  pinMode(BUZZER_PIN, OUTPUT);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Infinite loop

  }

  display.clearDisplay();                // Clear the display
  display.setTextSize(1);                // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);   // White text
  display.setCursor(0, 15);              // Start at top-left corner (x=0, y=10)
  display.println("Hello, World! from NodeMCU");      // Print your message
  display.display();                     // Actually display it
}

void loop() {
 
  digitalWrite(BUZZER_PIN, HIGH);
  delay(1000);
  digitalWrite(BUZZER_PIN, LOW);



  // Nothing to do here
}
