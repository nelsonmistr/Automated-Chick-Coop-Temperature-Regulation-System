#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHT.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define DHTPIN 5
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

const int RED_BULB_PIN = 4;
const int GREEN_BULB_PIN = 9;
const int ORANGE_BULB_PIN = 8;

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));

  dht.begin();
  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(4, OUTPUT);

  if (!display.begin(SSD1306_ADDRESS)) {
    Serial.println(F("Display initialization failed!"));
    while (1);
  //}
  display.display();
  delay(2000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
}

void loop() {
  delay(5000);

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);

  // Display temperature and alert on OLED display
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print(F("Humidity: "));
  display.print(h);
  display.print(F("%  Temperature: "));
  display.print(t);
  display.println(F("°C"));

  String alertMessage;

  if (t > 30) {
    alertMessage = "You have fever";
    digitalWrite(4, HIGH);
    digitalWrite(9, LOW);
    digitalWrite(8, LOW);
  } else if ((t <= 30) && (t >= 29)) {
    alertMessage = "Your body temperature is normal";
    digitalWrite(9, HIGH);
    digitalWrite(4, LOW);
    digitalWrite(8, LOW);
  } else {
    alertMessage = "You are cold";
    digitalWrite(8, HIGH);
    digitalWrite(9, LOW);
    digitalWrite(4, LOW);
  }

  // Display alert message
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 30);
  display.println(alertMessage);

  display.display();
  delay(5000);
}