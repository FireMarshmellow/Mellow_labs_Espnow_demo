#include <WiFi.h>
#include <esp_now.h>

// Button pins
const int BUTTON1_PIN = 20;
const int BUTTON2_PIN = 19;
const int BUTTON3_PIN = 6;

// Structure to send
typedef struct ButtonData {
  bool button1;
  bool button2;
  bool button3;
} ButtonData;

ButtonData data;

// Receiver MAC Address (use your own)
uint8_t receiverMAC[] = {0x, 0x, 0x, 0x, 0x, 0x};

void setup() {
  Serial.begin(115200);

  // Set button pins
  pinMode(BUTTON1_PIN, INPUT);
  pinMode(BUTTON2_PIN, INPUT);
  pinMode(BUTTON3_PIN, INPUT);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverMAC, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  data.button1 = digitalRead(BUTTON1_PIN) == HIGH;
  data.button2 = digitalRead(BUTTON2_PIN) == HIGH;
  data.button3 = digitalRead(BUTTON3_PIN) == HIGH;

  esp_now_send(receiverMAC, (uint8_t *)&data, sizeof(data));

  delay(100);  // Small delay to avoid spamming
}