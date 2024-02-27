#include <Servo.h>

Servo servo;
Servo servo1;

void setup() {
  servo.attach(9);
  servo1.attach(10);
  servo.write(0);
  servo1.write(0);
  delay(1000);     
}

void loop() {
  servo.write(45); 
  delay(1000);     
  servo.write(0);   
  delay(1000);  
  servo1.write(90); 
  delay(1000);     
  servo1.write(0);   
  delay(1000);   
}
