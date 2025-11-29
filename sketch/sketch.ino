// SPDX-FileCopyrightText: Copyright (C) 2025 ARDUINO SA
// SPDX-License-Identifier: MPL-2.0

#include <Arduino_RouterBridge.h>

void set_hue(int h) {
    // Test: Just turn RED LED on
    analogWrite(LED_BUILTIN, 255);
    analogWrite(LED_BUILTIN + 1, 0);
    analogWrite(LED_BUILTIN + 2, 0);
}

void set_sat(int s) {
    // Test: Just turn GREEN LED on
    analogWrite(LED_BUILTIN, 0);
    analogWrite(LED_BUILTIN + 1, 255);
    analogWrite(LED_BUILTIN + 2, 0);
}

void set_bri(int b) {
    // Test: Just turn BLUE LED on
    analogWrite(LED_BUILTIN, 0);
    analogWrite(LED_BUILTIN + 1, 0);
    analogWrite(LED_BUILTIN + 2, 255);
}

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(LED_BUILTIN + 1, OUTPUT);
    pinMode(LED_BUILTIN + 2, OUTPUT);
    
    // Test at startup: Turn LED WHITE
    analogWrite(LED_BUILTIN, 255);
    analogWrite(LED_BUILTIN + 1, 255);
    analogWrite(LED_BUILTIN + 2, 255);

    Bridge.begin();
    Bridge.provide("set_hue", set_hue);
    Bridge.provide("set_sat", set_sat);
    Bridge.provide("set_bri", set_bri);
}

void loop() {}
