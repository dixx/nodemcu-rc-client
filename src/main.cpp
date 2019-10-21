#include <cstdint>
#include <list>
#include "Arduino.h"
#include "painlessMesh.h"
#include "core.h"
#include "my_secrets.h" // <-- here goes RC_SSID and RC_WLAN_PASSWORD

const uint32_t LED_1 = D2;
const uint32_t LED_2 = D5;
const uint32_t LED_3 = D6;
const uint32_t LED_4 = D1;
bool clientsConnected = false;
painlessMesh network;

void checkClients() {
    std::list<uint32_t> nodes = network.getNodeList();
    clientsConnected = (nodes.size() > 0);
    digitalWrite(LED_4, clientsConnected ? HIGH : LOW);
}

void receivedCallback(uint32_t from, String &msg) {
    Serial.printf("startHere: Received from %d msg=%s\n", from, msg.c_str());
}

void newConnectionCallback(bool adopt) {
    Serial.printf("startHere: New Connection, adopt=%d\n", adopt);
}

void setup() {
    serial::init();
    pinMode(LED_1, OUTPUT);
    pinMode(LED_2, OUTPUT);
    pinMode(LED_3, OUTPUT);
    pinMode(LED_4, OUTPUT);
    network.setDebugMsgTypes(ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE); // all types on
    //network.setDebugMsgTypes(ERROR | STARTUP);  // set before init() so that you can see startup messages
    network.init(RC_SSID, RC_WLAN_PASSWORD);
    network.onReceive(&receivedCallback);
    network.onNewConnection(&newConnectionCallback);
}

void loop() {
    network.update();
}
