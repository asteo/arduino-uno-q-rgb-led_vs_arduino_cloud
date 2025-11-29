// SPDX-FileCopyrightText: Copyright (C) 2025 ARDUINO SA
// SPDX-License-Identifier: MPL-2.0

// Learning Example: Step 3 - Software Timer for LED4 (FIXED)

#include <Arduino_RouterBridge.h>
#include <zephyr/kernel.h>

struct k_timer led4_timer;
int led4_state = 0;

void led4_timer_handler(struct k_timer *timer) {
    digitalWrite(LED4_R, HIGH);
    digitalWrite(LED4_G, HIGH);
    digitalWrite(LED4_B, HIGH);
    
    switch (led4_state) {
        case 0: digitalWrite(LED4_R, LOW); break;  // RED
        case 1: digitalWrite(LED4_G, LOW); break;  // GREEN
        case 2: digitalWrite(LED4_B, LOW); break;  // BLUE
        case 3:  // WHITE
            digitalWrite(LED4_R, LOW);
            digitalWrite(LED4_G, LOW);
            digitalWrite(LED4_B, LOW);
            break;
        case 4: break;  // OFF
    }
    
    led4_state = (led4_state + 1) % 5;
}

void setup() {
    pinMode(LED3_R, OUTPUT);
    pinMode(LED3_G, OUTPUT);
    pinMode(LED3_B, OUTPUT);
    pinMode(LED4_R, OUTPUT);
    pinMode(LED4_G, OUTPUT);
    pinMode(LED4_B, OUTPUT);
    
    // Turn off all LEDs initially
    digitalWrite(LED3_R, HIGH);
    digitalWrite(LED3_G, HIGH);
    digitalWrite(LED3_B, HIGH);
    digitalWrite(LED4_R, HIGH);
    digitalWrite(LED4_G, HIGH);
    digitalWrite(LED4_B, HIGH);
    
    k_timer_init(&led4_timer, led4_timer_handler, NULL);
    k_timer_start(&led4_timer, K_MSEC(500), K_MSEC(500));
    
    Bridge.begin();
}

void loop() {
    // Fade RED only
    analogWrite(LED3_G, 0);  // Green OFF
    analogWrite(LED3_B, 0);  // Blue OFF
    for (int i = 0; i <= 255; i++) {
        analogWrite(LED3_R, i);
        k_sleep(K_MSEC(5));
    }
    for (int i = 255; i >= 0; i--) {
        analogWrite(LED3_R, i);
        k_sleep(K_MSEC(5));
    }
    
    k_sleep(K_MSEC(300));
    
    // Fade GREEN only
    analogWrite(LED3_R, 0);  // Red OFF
    analogWrite(LED3_B, 0);  // Blue OFF
    for (int i = 0; i <= 255; i++) {
        analogWrite(LED3_G, i);
        k_sleep(K_MSEC(5));
    }
    for (int i = 255; i >= 0; i--) {
        analogWrite(LED3_G, i);
        k_sleep(K_MSEC(5));
    }
    
    k_sleep(K_MSEC(300));
    
    // Fade BLUE only
    analogWrite(LED3_R, 0);  // Red OFF
    analogWrite(LED3_G, 0);  // Green OFF
    for (int i = 0; i <= 255; i++) {
        analogWrite(LED3_B, i);
        k_sleep(K_MSEC(5));
    }
    for (int i = 255; i >= 0; i--) {
        analogWrite(LED3_B, i);
        k_sleep(K_MSEC(5));
    }
    
    k_sleep(K_MSEC(300));
}
