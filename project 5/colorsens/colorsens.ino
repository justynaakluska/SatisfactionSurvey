const int pinS0 = 4;
const int pinS1 = 5;
const int pinS2 = 7;
const int pinS3 = 6;
const int pinOUT = A0;

void setup() {
  Serial.begin(9600);
  pinMode(pinS0, OUTPUT);
  pinMode(pinS1, OUTPUT);
  pinMode(pinS2, OUTPUT);
  pinMode(pinS3, OUTPUT);
  pinMode(pinOUT, INPUT);
  digitalWrite(pinS0, HIGH);
  digitalWrite(pinS1, HIGH);
}

void loop() {
  digitalWrite(pinS2, LOW);
  digitalWrite(pinS3, LOW);
  
  int red = readColor(1);
  int green = readColor(2);
  int blue = readColor(3);
  
  Serial.print("Red: ");
  Serial.print(red);
  Serial.print(" Green: ");
  Serial.print(green);
  Serial.print(" Blue: ");
  Serial.println(blue);
  
  delay(1000);
}

int readColor(int color) {
  switch (color) {
    case 1:
      digitalWrite(pinS2, LOW);
      digitalWrite(pinS3, LOW);
      break;
    case 2:
      digitalWrite(pinS2, HIGH);
      digitalWrite(pinS3, HIGH);
      break;
    case 3:
      digitalWrite(pinS2, LOW);
      digitalWrite(pinS3, HIGH);
      break;
  }
  
  return analogRead(pinOUT);
}
