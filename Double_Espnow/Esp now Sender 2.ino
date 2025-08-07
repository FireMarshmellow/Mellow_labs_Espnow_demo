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

// Receiver MAC Addresses
uint8_t receiverMAC[]  = {0xDC, 0xDA, 0x0C, 0xC1, 0x5E, 0xF8};  // ESP32-C3
uint8_t receiver2MAC[] = {0xE4, 0x65, 0xB8, 0x76, 0x83, 0x24};  // ESP32-Dev

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

  // Add first peer (ESP32-C3)
  esp_now_peer_info_t peer1 = {};
  memcpy(peer1.peer_addr, receiverMAC, 6);
  peer1.channel = 0;
  peer1.encrypt = false;
  if (esp_now_add_peer(&peer1) != ESP_OK) {
    Serial.println("Failed to add peer 1");
  }

  // Add second peer (ESP32-Dev Board)
  esp_now_peer_info_t peer2 = {};
  memcpy(peer2.peer_addr, receiver2MAC, 6);
  peer2.channel = 0;
  peer2.encrypt = false;
  if (esp_now_add_peer(&peer2) != ESP_OK) {
    Serial.println("Failed to add peer 2");
  }
}

void loop() {
  data.button1 = digitalRead(BUTTON1_PIN) == HIGH;
  data.button2 = digitalRead(BUTTON2_PIN) == HIGH;
  data.button3 = digitalRead(BUTTON3_PIN) == HIGH;

  esp_now_send(receiverMAC, (uint8_t *)&data, sizeof(data));
  esp_now_send(receiver2MAC, (uint8_t *)&data, sizeof(data));

  delay(100);  // debounce-friendly rate
}