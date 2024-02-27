#include <Servo.h>
#define S0 4
#define S1 5
#define S2 7
#define S3 6
#define sensorOut 8
#include <FastLED.h>
#include <VL53L1X.h>

#define LED_PIN 2         // Pin connected to the Data Input of the LED strip
#define NUM_LEDS 100       // Total number of LEDs in the strip
#define LED_TYPE WS2811   // LED strip type (change if using a different type)
#define COLOR_ORDER GRB   // LED strip color order (change if needed)
VL53L1X sensor;
CRGB leds[NUM_LEDS]; 
Servo topServo;
Servo bottomServo;
int frequency = 0;
int color = 0;
void setup() {
  
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT); 
  digitalWrite(S0, HIGH); //20% scalling
  digitalWrite(S1, LOW);
  topServo.attach(9);
  bottomServo.attach(10);
   FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);  // Initialize the LED strip
  FastLED.setBrightness(50);   
  Serial.begin(9600);
  Wire.begin();
  Wire.setClock(400000); // use 400 kHz I2C

  sensor.setTimeout(500);
  if (!sensor.init())
  {
    Serial.println("Failed to detect and initialize sensor!");
    while (1);
  }

  sensor.setDistanceMode(VL53L1X::Long);
  sensor.setMeasurementTimingBudget(15000);
  sensor.startContinuous(15);
}

void loop() {
  
  int distance = sensor.read(); // Read the distance from the sensor

  if (distance <= 50){ // Check if the object is 20 cm or closer
  {
    // Run the rest of your program here

 
  
   topServo.write(83); //set top servo at the first hole to catch the candy
  delay(500);

    for (int i = 83; i >= 30; i--) { //move top servo with the candy to the color sensor position
      topServo.write(i);
      delay(2);
    }
    delay(500);
  topServo.write(83); //set top servo at the first hole to catch the candy
  delay(500);
  
  for(int i = 83; i >= 30; i--) { //move top servo with the candy to the color sensor position
    topServo.write(i);
    delay(2);
  }
  delay(500);
  
  color = readColor(); //read the value from color sensor and put the bottom servo in the right path
  delay(5);  
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
    bottomServo.write(180);
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
  
  for(int i = 30; i >= 0; i--) { //move top servo with candy from the color sensor position to the second hole position
    topServo.write(i);
    delay(2);
  } 
  delay(200);
  
  for(int i = 0; i <= 83; i++) { //return top servo to the first position to catch another candy
    topServo.write(i);
    delay(2);
  }
  color=0;
}
  

int readColor() { //color sensor function to read the color
  
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
  
  if(R<70 & R>45 & G>80 & G<130 & B>60 & B<90){
    color = 1; // Red
  }
  if(G<100 & G>50 & R<65 & R>45 &B<75 & B>50){
    color = 2; // Green
  }
  if (B<55 &B>30 & G>40 & G<65 & R>40& R<60){
    color = 3; // Blue
  }
  return color;  

    // No object detected within 20 cm, wait and try again
}

  }
