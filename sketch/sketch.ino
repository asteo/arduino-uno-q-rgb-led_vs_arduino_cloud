// SPDX-FileCopyrightText: Copyright (C) 2025 ARDUINO SA
// SPDX-License-Identifier: MPL-2.0

// Learning Example: Step 1 - Simple HW PWM Fade on LED3_R

#include <Arduino_RouterBridge.h>

void setup() {
    // LED3 has hardware PWM support
    pinMode(LED3_R, OUTPUT);
    pinMode(LED3_G, OUTPUT);
    pinMode(LED3_B, OUTPUT);
    
    // LED4 is digital only
    pinMode(LED4_R, OUTPUT);
    pinMode(LED4_G, OUTPUT);
    pinMode(LED4_B, OUTPUT);
    
    Bridge.begin();
}

void loop() {
    // Simple fade effect on LED3_R using hardware PWM
    
    // Fade in (0 -> 255)
    for (int brightness = 0; brightness <= 255; brightness++) {
        analogWrite(LED3_R, brightness);
        delay(5);  // 5ms per step = 1.28 seconds total
    }
    
    // Fade out (255 -> 0)
    for (int brightness = 255; brightness >= 0; brightness--) {
        analogWrite(LED3_R, brightness);
        delay(5);
    }
    
    // Pause
    delay(500);
}
