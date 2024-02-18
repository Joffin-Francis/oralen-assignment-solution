//Refer https://github.com/devxplained/MAX3010x-Sensor-Library
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "MAX30105.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

MAX30105 particleSensor;

#define BUZZER_PIN 6

void setup() {
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  delay(2000);
  display.clearDisplay();
  
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println("MAX30105 was not found. Please check wiring/power.");
    while (1);
  }
  
  particleSensor.setup();
  particleSensor.setPulseAmplitudeRed(0x0A);
  particleSensor.setPulseAmplitudeGreen(0);
  
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  float heartRate = particleSensor.getHeartRate();
  float spo2 = particleSensor.getSpO2();
  float ambientLightCancellation = particleSensor.getAmbientLightCancellation();
  float ledCurrent = particleSensor.getLEDCurrent();
  float pulseWidth = particleSensor.getPulseWidth();
  float temperature = particleSensor.getTemperature();
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Heart Rate: " + String(heartRate));
  display.println("SpO2: " + String(spo2));
  display.println("Ambient Light Cancellation: " + String(ambientLightCancellation));
  display.println("LED Current: " + String(ledCurrent));
  display.println("Pulse Width: " + String(pulseWidth));
  display.println("Temperature: " + String(temperature));
  display.display();
  
  if (heartRate < 60 || heartRate > 100 || spo2 < 95 || ambientLightCancellation > 200 || ledCurrent > 50 || pulseWidth > 411 || temperature < -40 || temperature > 85) {
    digitalWrite(BUZZER_PIN, HIGH);
  } else {
    digitalWrite(BUZZER_PIN, LOW);
  }
  
  delay(1000);
}
