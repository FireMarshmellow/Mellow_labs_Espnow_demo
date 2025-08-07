#include <WiFi.h>
#include <esp_now.h>

// RGB LED pins on the ESP32 Dev Board
const int RED_PIN = 15;
const int GREEN_PIN = 2;
const int BLUE_PIN = 4;

// State tracking
bool redState = false;
bool greenState = false;
bool blueState = false;

// Last known button states (for edge detection)
bool lastButton1 = false;
bool lastButton2 = false;
bool lastButton3 = false;

typedef struct ButtonData {
  bool button1;
  bool button2;
  bool button3;
} ButtonData;

ButtonData received;

// Set physical LED states
void updateLEDs() {
  digitalWrite(RED_PIN, redState ? HIGH : LOW);
  digitalWrite(GREEN_PIN, greenState ? HIGH : LOW);
  digitalWrite(BLUE_PIN, blueState ? HIGH : LOW);
}

// New receive callback (ESP-IDF v5+ compatible)
void onReceive(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
  if (len != sizeof(ButtonData)) return;

  ButtonData current;
  memcpy(&current, incomingData, sizeof(current));

  // Toggle on rising edge (button pressed now, but not before)
  if (current.button1 && !lastButton1) redState = !redState;
  if (current.button2 && !lastButton2) greenState = !greenState;
  if (current.button3 && !lastButton3) blueState = !blueState;

  // Store previous states
  lastButton1 = current.button1;
  lastButton2 = current.button2;
  lastButton3 = current.button3;

  updateLEDs();
}

void setup() {
  Serial.begin(115200);

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }

  esp_now_register_recv_cb(onReceive);
}

void loop() {
  // nothing to do here
}