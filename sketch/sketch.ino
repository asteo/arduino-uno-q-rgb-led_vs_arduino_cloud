// SPDX-FileCopyrightText: Copyright (C) 2025 ARDUINO SA
// SPDX-License-Identifier: MPL-2.0

#include <Arduino_RouterBridge.h>

void set_hue(int h) {
    // Just acknowledge - do nothing
}

void set_sat(int s) {
    // Just acknowledge - do nothing
}

void set_bri(int b) {
    // Just acknowledge - do nothing
}

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(LED_BUILTIN + 1, OUTPUT);
    pinMode(LED_BUILTIN + 2, OUTPUT);
    
    Bridge.begin();
    Bridge.provide("set_hue", set_hue);
    Bridge.provide("set_sat", set_sat);
    Bridge.provide("set_bri", set_bri);
}

void loop() {
    // RED ON (Active LOW: LOW = ON)
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(LED_BUILTIN + 1, HIGH);
    digitalWrite(LED_BUILTIN + 2, HIGH);
    delay(1000);
    
    // GREEN ON
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(LED_BUILTIN + 1, LOW);
    digitalWrite(LED_BUILTIN + 2, HIGH);
    delay(1000);
    
    // BLUE ON
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(LED_BUILTIN + 1, HIGH);
    digitalWrite(LED_BUILTIN + 2, LOW);
    delay(1000);
    
    // ALL OFF
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(LED_BUILTIN + 1, HIGH);
    digitalWrite(LED_BUILTIN + 2, HIGH);
    delay(1000);
}
