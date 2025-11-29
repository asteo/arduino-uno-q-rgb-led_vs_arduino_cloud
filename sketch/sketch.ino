// SPDX-FileCopyrightText: Copyright (C) 2025 ARDUINO SA
// SPDX-License-Identifier: MPL-2.0

// Learning Example: Step 3 - Software Timer for LED4

#include <Arduino_RouterBridge.h>
#include <zephyr/kernel.h>

// Software timer for LED4
struct k_timer led4_timer;
int led4_state = 0;  // 0=R, 1=G, 2=B, 3=W, 4=OFF

void led4_timer_handler(struct k_timer *timer) {
    // Turn off all
    digitalWrite(LED4_R, HIGH);
    digitalWrite(LED4_G, HIGH);
    digitalWrite(LED4_B, HIGH);
    
    // Cycle through R -> G -> B -> W -> OFF
    switch (led4_state) {
        case 0:  // RED
            digitalWrite(LED4_R, LOW);
            break;
        case 1:  // GREEN
            digitalWrite(LED4_G, LOW);
            break;
        case 2:  // BLUE
            digitalWrite(LED4_B, LOW);
            break;
        case 3:  // WHITE (all on)
            digitalWrite(LED4_R, LOW);
            digitalWrite(LED4_G, LOW);
            digitalWrite(LED4_B, LOW);
            break;
        case 4:  // OFF (already off)
            break;
    }
    
    led4_state = (led4_state + 1) % 5;  // Cycle 0-4
}

void setup() {
    pinMode(LED3_R, OUTPUT);
    pinMode(LED3_G, OUTPUT);
    pinMode(LED3_B, OUTPUT);
    pinMode(LED4_R, OUTPUT);
    pinMode(LED4_G, OUTPUT);
    pinMode(LED4_B, OUTPUT);
    
    // Initialize software timer for LED4 (500ms interval)
    k_timer_init(&led4_timer, led4_timer_handler, NULL);
    k_timer_start(&led4_timer, K_MSEC(500), K_MSEC(500));
    
    Bridge.begin();
}

void loop() {
    // LED3: Smooth RGB fade (HW PWM)
    
    // Fade RED
    for (int i = 0; i <= 255; i++) {
        analogWrite(LED3_R, i);
        k_sleep(K_MSEC(5));
    }
    for (int i = 255; i >= 0; i--) {
        analogWrite(LED3_R, i);
        k_sleep(K_MSEC(5));
    }
    
    k_sleep(K_MSEC(300));
    
    // Fade GREEN
    for (int i = 0; i <= 255; i++) {
        analogWrite(LED3_G, i);
        k_sleep(K_MSEC(5));
    }
    for (int i = 255; i >= 0; i--) {
        analogWrite(LED3_G, i);
        k_sleep(K_MSEC(5));
    }
    
    k_sleep(K_MSEC(300));
    
    // Fade BLUE
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
