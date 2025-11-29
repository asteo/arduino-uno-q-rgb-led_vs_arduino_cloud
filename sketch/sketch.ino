// SPDX-FileCopyrightText: Copyright (C) 2025 ARDUINO SA
// SPDX-License-Identifier: MPL-2.0

// Learning Example: Step 2 - Yield CPU with k_sleep()

#include <Arduino_RouterBridge.h>
#include <zephyr/kernel.h>

void setup() {
    pinMode(LED3_R, OUTPUT);
    pinMode(LED3_G, OUTPUT);
    pinMode(LED3_B, OUTPUT);
    pinMode(LED4_R, OUTPUT);
    pinMode(LED4_G, OUTPUT);
    pinMode(LED4_B, OUTPUT);
    
    Bridge.begin();
}

void loop() {
    // Fade RED (HW PWM)
    for (int i = 0; i <= 255; i++) {
        analogWrite(LED3_R, i);
        k_sleep(K_MSEC(5));  // Yields CPU to other threads
    }
    for (int i = 255; i >= 0; i--) {
        analogWrite(LED3_R, i);
        k_sleep(K_MSEC(5));
    }
    
    k_sleep(K_MSEC(500));
    
    // Fade GREEN (HW PWM)
    for (int i = 0; i <= 255; i++) {
        analogWrite(LED3_G, i);
        k_sleep(K_MSEC(5));
    }
    for (int i = 255; i >= 0; i--) {
        analogWrite(LED3_G, i);
        k_sleep(K_MSEC(5));
    }
    
    k_sleep(K_MSEC(500));
    
    // Fade BLUE (HW PWM)
    for (int i = 0; i <= 255; i++) {
        analogWrite(LED3_B, i);
        k_sleep(K_MSEC(5));
    }
    for (int i = 255; i >= 0; i--) {
        analogWrite(LED3_B, i);
        k_sleep(K_MSEC(5));
    }
    
    k_sleep(K_SECONDS(1));  // Pause 1 second
}
