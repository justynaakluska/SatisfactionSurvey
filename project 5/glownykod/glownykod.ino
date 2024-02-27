#include <Servo.h>
#include <FastLED.h>
#include <Wire.h>
#include <VL53L1X.h>

#define S0 4
#define S1 5
#define S2 7
#define S3 6
#define sensorOut 8

#define LED_PIN 2         // Pin connected to the Data Input of the LED strip
#define NUM_LEDS 40     // Total number of LEDs in the strip
#define LED_TYPE WS2811   // LED strip type (change if using a different type)
#define COLOR_ORDER GRB   // LED strip color order (change if needed)

CRGB leds[NUM_LEDS];
Servo topServo;
Servo bottomServo;
int frequency = 0;
int color = 0;

VL53L1X sensor;

void setup() {
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);
  digitalWrite(S0, HIGH); //20% scaling
  digitalWrite(S1, LOW);
  topServo.attach(9);
  bottomServo.attach(10);
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);  // Initialize the LED strip
  FastLED.setBrightness(50);
  Serial.begin(9600);

  Wire.begin();
  Wire.setClock(400000); // use 400 kHz I2C

  sensor.setTimeout(500);
  if (!sensor.init()) {
    Serial.println("Failed to detect and initialize sensor!");
    while (1);
  }

  sensor.setDistanceMode(VL53L1X::Long);
  sensor.setMeasurementTimingBudget(15000);
  sensor.startContinuous(15);
}

void loop() {
  if (sensor.read() <= 100) { // Start the sorting process if an object is detected within 10 cm
    topServo.write(150); // Set top servo at the first hole to catch the candy
    delay(500);

    for (int i = 150; i >= 65; i--) { // Move top servo with the candy to the color sensor position
      topServo.write(i);
      delay(50);
    }
    delay(1500);

    color = readColor(); // Read the value from the color sensor and put the bottom servo in the right path
    delay(2);
    switch (color) {
      case 1:
        bottomServo.write(45);
        for (int i = 0; i < NUM_LEDS; i++) {
          leds[i] = CRGB::Red;   // Set the LED color to red
        }
        FastLED.show();          // Update the LED strip with the new colors
        delay(500);
        break;
      case 2:
        bottomServo.write(90);
        for (int i = 0; i < NUM_LEDS; i++) {
          leds[i] = CRGB::Green;  // Set the LED color to green
        }
        FastLED.show();
        delay(500);
        break;
      case 3:
        bottomServo.write(135);
        for (int i = 0; i < NUM_LEDS; i++) {
          leds[i] = CRGB::Blue;  // Set the LED color to blue
        }
        FastLED.show();
        delay(500);
        break;

      case 0:
        break;
    }
    delay(300);

    for (int i = 0; i >= 0; i--) { // Move top servo with candy from the color sensor position to the second hole position
      topServo.write(i);
      delay(50);
    }
    delay(200);

    for (int i = 0; i <= 150; i++) { // Return top servo to the first position to catch another candy
      topServo.write(i);
      delay(2);
    }
    color = 0;
  }
}

int readColor() { // Color sensor function to read the color
  // Setting red
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  frequency = pulseIn(sensorOut, LOW);
  int R = frequency;
  Serial.print("R= ");
  Serial.print(frequency);
  Serial.print("  ");
  delay(50);

  // Setting Green
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  frequency = pulseIn(sensorOut, LOW);
  int G = frequency;
  Serial.print("G= ");
  Serial.print(frequency);
  Serial.print("  ");
  delay(50);

  // Setting Blue
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  frequency = pulseIn(sensorOut, LOW);
  int B = frequency;
  Serial.print("B= ");
  Serial.print(frequency);
  Serial.println("  ");
  delay(50);

  if (R < 100 && R > 80 && G > 160 && G < 220 && B > 120 && B < 175) {
    color = 1; // Red
  }
  if (G < 115 && G > 90 && R < 97 && R > 82 && B < 125 && B > 90) {
    color = 2; // Green
  }
  if (B < 80 && B > 40 && G > 75 && G < 120 && R > 55 && R < 95) {
    color = 3; // Blue
  }
  return color;
}
