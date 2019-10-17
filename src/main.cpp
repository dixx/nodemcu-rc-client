#include "Arduino.h"
#include "ESP8266WiFi.h"
#include "core.h"
#include "my_secrets.h"
#include <ESP8266WebServer.h>

const uint32_t LED_1 = D2;
const uint32_t LED_2 = D5;
const uint32_t LED_3 = D6;
const uint32_t LED_4 = D1;
wl_status_t oldStatus = WL_NO_SHIELD;
const IPAddress IP(192, 168, 10, 73);
const IPAddress GATEWAY(192, 168, 10, 101);
const IPAddress SUBNET(255, 255, 255, 0);

ESP8266WebServer server(80);

void handleNotFound(){
  server.send(404, "text/plain", "Not found");
}

void handleKeepAlive(){
    server.send(201, "text/plain", "Still alive :)");
    digitalWrite(LED_4, HIGH);
    delay(100);
    digitalWrite(LED_4, LOW);
    Serial.print(".");
}

void handleRC() {
    server.send(201, "text/plain", "OK");
    Serial.println(server.args());
}

void setup() {
    pinMode(LED_1, OUTPUT);
    pinMode(LED_2, OUTPUT);
    pinMode(LED_3, OUTPUT);
    pinMode(LED_4, OUTPUT);
    serial::init();
    WiFi.mode(WIFI_STA);
    WiFi.config(IP, GATEWAY, SUBNET);
    WiFi.setSleepMode(WIFI_NONE_SLEEP);
    WiFi.setAutoReconnect(true);
    WiFi.begin(RC_SSID, RC_WLAN_PASSWORD);
    server.on("/keep-alive", handleKeepAlive);
    server.on("/rc", handleRC);
    server.onNotFound(handleNotFound);
    server.begin();
}

void loop() {
    server.handleClient();
    wl_status_t status = WiFi.status();
    if (status != oldStatus) {
        oldStatus = status;
        if (status == WL_NO_SHIELD)
            Serial.println("WL_NO_SHIELD");
        else if (status == WL_IDLE_STATUS)
            Serial.println("WL_IDLE_STATUS");
        else if (status == WL_NO_SSID_AVAIL)
            Serial.println("WL_NO_SSID_AVAIL");
        else if (status == WL_SCAN_COMPLETED)
            Serial.println("WL_SCAN_COMPLETED");
        else if (status == WL_CONNECTED)
            Serial.println("WL_CONNECTED");
        else if (status == WL_CONNECT_FAILED)
            Serial.println("WL_CONNECT_FAILED");
        else if (status == WL_CONNECTION_LOST)
            Serial.println("WL_CONNECTION_LOST");
        else if (status == WL_DISCONNECTED)
            Serial.println("WL_DISCONNECTED");
        else
            Serial.println("unknown status");
    }
    if (!WiFi.isConnected()) {
        digitalWrite(LED_1, HIGH);
        delay(100);
        digitalWrite(LED_1, LOW);
        delay(100);
    } else {
        digitalWrite(LED_1, HIGH);
    }
}

// client:
// if (server.arg(“Temperature”)== “”){     //Parameter not found
//     message = “Temperature Argument not found”;
// }else{     //Parameter found
//     message = “Temperature Argument = “;
//     message += server.arg(“Temperature”);     //Gets the value of the query parameter
// }
