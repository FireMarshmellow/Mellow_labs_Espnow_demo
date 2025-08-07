// Define your button pins (GPIO numbers)
const int BUTTON1_PIN = 20;
const int BUTTON2_PIN = 19;
const int BUTTON3_PIN = 6;

void setup() {
  Serial.begin(115200);

  // Set pins as input (external pull-down resistors or floating)
  pinMode(BUTTON1_PIN, INPUT);
  pinMode(BUTTON2_PIN, INPUT);
  pinMode(BUTTON3_PIN, INPUT);
}

void loop() {
  // Buttons are active HIGH now
  bool b1 = digitalRead(BUTTON1_PIN) == HIGH;
  bool b2 = digitalRead(BUTTON2_PIN) == HIGH;
  bool b3 = digitalRead(BUTTON3_PIN) == HIGH;

  Serial.print("Button 1: ");
  Serial.print(b1 ? "Pressed" : "Released");
  Serial.print(" | Button 2: ");
  Serial.print(b2 ? "Pressed" : "Released");
  Serial.print(" | Button 3: ");
  Serial.println(b3 ? "Pressed" : "Released");

  delay(200);
}