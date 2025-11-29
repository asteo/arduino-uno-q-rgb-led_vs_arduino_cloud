// SPDX-FileCopyrightText: Copyright (C) 2025 ARDUINO SA
// SPDX-License-Identifier: MPL-2.0

#include <Arduino_RouterBridge.h>

// RGB LED 3 (PH10-12)
#define LED3_R PH_10
#define LED3_G PH_11
#define LED3_B PH_12

// RGB LED 4 (PH13-15)
#define LED4_R PH_13
#define LED4_G PH_14
#define LED4_B PH_15

void set_hue(int h) {}
void set_sat(int s) {}
void set_bri(int b) {}

void setup() {
    // Initialize both RGB LEDs
    pinMode(LED3_R, OUTPUT);
    pinMode(LED3_G, OUTPUT);
    pinMode(LED3_B, OUTPUT);
    pinMode(LED4_R, OUTPUT);
    pinMode(LED4_G, OUTPUT);
    pinMode(LED4_B, OUTPUT);
    
    Bridge.begin();
    Bridge.provide("set_hue", set_hue);
    Bridge.provide("set_sat", set_sat);
    Bridge.provide("set_bri", set_bri);
}

void loop() {
    // Test LED3 - RED
    digitalWrite(LED3_R, LOW);
    digitalWrite(LED3_G, HIGH);
    digitalWrite(LED3_B, HIGH);
    delay(1000);
    
    // Test LED3 - GREEN
    digitalWrite(LED3_R, HIGH);
    digitalWrite(LED3_G, LOW);
    digitalWrite(LED3_B, HIGH);
    delay(1000);
    
    // Test LED3 - BLUE
    digitalWrite(LED3_R, HIGH);
    digitalWrite(LED3_G, HIGH);
    digitalWrite(LED3_B, LOW);
    delay(1000);
    
    // Test LED3 - OFF
    digitalWrite(LED3_R, HIGH);
    digitalWrite(LED3_G, HIGH);
    digitalWrite(LED3_B, HIGH);
    delay(500);
    
    // Test LED4 - RED
    digitalWrite(LED4_R, LOW);
    digitalWrite(LED4_G, HIGH);
    digitalWrite(LED4_B, HIGH);
    delay(1000);
    
    // Test LED4 - GREEN
    digitalWrite(LED4_R, HIGH);
    digitalWrite(LED4_G, LOW);
    digitalWrite(LED4_B, HIGH);
    delay(1000);
    
    // Test LED4 - BLUE
    digitalWrite(LED4_R, HIGH);
    digitalWrite(LED4_G, HIGH);
    digitalWrite(LED4_B, LOW);
    delay(1000);
    
    // Test LED4 - OFF
    digitalWrite(LED4_R, HIGH);
    digitalWrite(LED4_G, HIGH);
    digitalWrite(LED4_B, HIGH);
    delay(500);
}
