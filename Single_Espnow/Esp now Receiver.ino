#include <WiFi.h>
#include <esp_now.h>

// RGB LED pins on ESP32-C3
const int RED_PIN = 2;
const int GREEN_PIN = 9;
const int BLUE_PIN = 8;

// Structure to receive
typedef struct ButtonData {
  bool button1;
  bool button2;
  bool button3;
} ButtonData;

ButtonData received;

void setColor(bool r, bool g, bool b) {
  digitalWrite(RED_PIN, r ? HIGH : LOW);
  digitalWrite(GREEN_PIN, g ? HIGH : LOW);
  digitalWrite(BLUE_PIN, b ? HIGH : LOW);
}
void onReceive(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
  if (len == sizeof(ButtonData)) {
    memcpy(&received, incomingData, sizeof(received));
    setColor(received.button1, received.button2, received.button3);
  }
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
  // Nothing needed here
}