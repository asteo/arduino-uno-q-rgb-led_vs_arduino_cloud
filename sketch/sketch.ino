// SPDX-FileCopyrightText: Copyright (C) 2025 ARDUINO SA
// SPDX-License-Identifier: MPL-2.0

#include <Arduino_RouterBridge.h>

int currentHue = 0;
int currentSat = 0;
int currentBri = 0;

// Converts HSV → RGB (simple integer version)
void hsvToRgb(int h, int s, int v, int& r, int& g, int& b) {
    float S = s / 100.0;
    float V = v / 100.0;
    float C = V * S;
    float X = C * (1 - abs((h / 60) % 2 - 1));
    float m = V - C;

    float R, G, B;

    if      (h < 60)  { R = C; G = X; B = 0; }
    else if (h < 120) { R = X; G = C; B = 0; }
    else if (h < 180) { R = 0; G = C; B = X; }
    else if (h < 240) { R = 0; G = X; B = C; }
    else if (h < 300) { R = X; G = 0; B = C; }
    else              { R = C; G = 0; B = X; }

    r = (R + m) * 255;
    g = (G + m) * 255;
    b = (B + m) * 255;
}

void updateLed() {
    int r, g, b;
    hsvToRgb(currentHue, currentSat, currentBri, r, g, b);

    analogWrite(LED3_R, r);
    analogWrite(LED3_G, g);
    analogWrite(LED3_B, b);
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

void setup() {
    pinMode(LED3_R, OUTPUT);
    pinMode(LED3_G, OUTPUT);
    pinMode(LED3_B, OUTPUT);

    Bridge.begin();
    Bridge.provide("set_hue", set_hue);
    Bridge.provide("set_sat", set_sat);
    Bridge.provide("set_bri", set_bri);
}

void loop() {}
