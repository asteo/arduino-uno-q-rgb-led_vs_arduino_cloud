// SPDX-FileCopyrightText: Copyright (C) 2025 ARDUINO SA
// SPDX-License-Identifier: MPL-2.0

#include <Arduino_RouterBridge.h>

// Try multiple pin definitions
#ifndef LED3_R
  #define LED3_R  PH_10
  #define LED3_G  PH_11
  #define LED3_B  PH_12
#endif

int currentHue = 0;
int currentSat = 0;
int currentBri = 0;

void hsvToRgb(int h, int s, int v, int& r, int& g, int& b) {
    float S = s / 100.0;
    float V = v / 100.0;
    float C = V * S;
    float X = C * (1 - abs(((h / 60) % 2) - 1));
    float m = V - C;

    float R, G, B;

    if      (h < 60)  { R = C; G = X; B = 0; }
    else if (h < 120) { R = X; G = C; B = 0; }
    else if (h < 180) { R = 0; G = C; B = X; }
    else if (h < 240) { R = 0; G = X; B = C; }
    else if (h < 300) { R = X; G = 0; B = C; }
    else              { R = C; G = 0; B = X; }

    r = (int)((R + m) * 255);
    g = (int)((G + m) * 255);
    b = (int)((B + m) * 255);
}

void setLED(int r, int g, int b) {
    // Active LOW: 0 = ON, 255 = OFF
    // Invert the values
    analogWrite(LED3_R, 255 - r);
    analogWrite(LED3_G, 255 - g);
    analogWrite(LED3_B, 255 - b);
}

void updateLed() {
    int r, g, b;
    hsvToRgb(currentHue, currentSat, currentBri, r, g, b);
    setLED(r, g, b);
}

void set_hue(int h) {
    currentHue = h;
    updateLed();
}

void set_sat(int s) {
    currentSat = s;
    updateLed();
}

void set_bri(int b) {
    currentBri = b;
    updateLed();
}

void runTests() {
    // Test: R, G, B with 1 second delay
    setLED(255, 0, 0);  // RED
    delay(1000);
    
    setLED(0, 255, 0);  // GREEN
    delay(1000);
    
    setLED(0, 0, 255);  // BLUE
    delay(1000);
    
    // Hue sweep
    for (int hue = 0; hue <= 240; hue += 10) {
        int r, g, b;
        hsvToRgb(hue, 100, 100, r, g, b);
        setLED(r, g, b);
        delay(50);
    }
    
    // Turn off
    setLED(0, 0, 0);
    delay(500);
}

void setup() {
    pinMode(LED3_R, OUTPUT);
    pinMode(LED3_G, OUTPUT);
    pinMode(LED3_B, OUTPUT);
    
    // Turn off initially (Active LOW)
    digitalWrite(LED3_R, HIGH);
    digitalWrite(LED3_G, HIGH);
    digitalWrite(LED3_B, HIGH);
    
    delay(500);
    
    // Run tests
    runTests();

    Bridge.begin();
    Bridge.provide("set_hue", set_hue);
    Bridge.provide("set_sat", set_sat);
    Bridge.provide("set_bri", set_bri);
}

void loop() {}
