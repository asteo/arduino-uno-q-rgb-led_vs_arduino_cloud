// SPDX-FileCopyrightText: Copyright (C) 2025 ARDUINO SA
// SPDX-License-Identifier: MPL-2.0

// Fixed: Correct PWM indices for LED3

#include <Arduino_RouterBridge.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/pwm.h>

// Correct indices: LED3_R=5, LED3_G=6, LED3_B=7
static const struct pwm_dt_spec pwm_led3_r = PWM_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 5);
static const struct pwm_dt_spec pwm_led3_g = PWM_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 6);
static const struct pwm_dt_spec pwm_led3_b = PWM_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 7);

// LED4 timer
struct k_timer led4_timer;
int led4_state = 0;

void led4_timer_handler(struct k_timer *timer) {
    digitalWrite(LED4_R, HIGH);
    digitalWrite(LED4_G, HIGH);
    digitalWrite(LED4_B, HIGH);
    
    switch (led4_state) {
        case 0: digitalWrite(LED4_R, LOW); break;
        case 1: digitalWrite(LED4_G, LOW); break;
        case 2: digitalWrite(LED4_B, LOW); break;
        case 3:
            digitalWrite(LED4_R, LOW);
            digitalWrite(LED4_G, LOW);
            digitalWrite(LED4_B, LOW);
            break;
        case 4: break;
    }
    
    led4_state = (led4_state + 1) % 5;
}

void setup() {
    pinMode(LED4_R, OUTPUT);
    pinMode(LED4_G, OUTPUT);
    pinMode(LED4_B, OUTPUT);
    
    k_timer_init(&led4_timer, led4_timer_handler, NULL);
    k_timer_start(&led4_timer, K_MSEC(500), K_MSEC(500));
    
    Bridge.begin();
}

void loop() {
    uint32_t period = pwm_led3_r.period;
    
    // Fade RED
    for (int i = 0; i <= 255; i++) {
        uint32_t pulse = (period * i) / 255;
        pwm_set_dt(&pwm_led3_r, period, pulse);
        k_sleep(K_MSEC(5));
    }
    for (int i = 255; i >= 0; i--) {
        uint32_t pulse = (period * i) / 255;
        pwm_set_dt(&pwm_led3_r, period, pulse);
        k_sleep(K_MSEC(5));
    }
    
    k_sleep(K_MSEC(300));
    
    // Fade GREEN
    for (int i = 0; i <= 255; i++) {
        uint32_t pulse = (period * i) / 255;
        pwm_set_dt(&pwm_led3_g, period, pulse);
        k_sleep(K_MSEC(5));
    }
    for (int i = 255; i >= 0; i--) {
        uint32_t pulse = (period * i) / 255;
        pwm_set_dt(&pwm_led3_g, period, pulse);
        k_sleep(K_MSEC(5));
    }
    
    k_sleep(K_MSEC(300));
    
    // Fade BLUE
    for (int i = 0; i <= 255; i++) {
        uint32_t pulse = (period * i) / 255;
        pwm_set_dt(&pwm_led3_b, period, pulse);
        k_sleep(K_MSEC(5));
    }
    for (int i = 255; i >= 0; i--) {
        uint32_t pulse = (period * i) / 255;
        pwm_set_dt(&pwm_led3_b, period, pulse);
        k_sleep(K_MSEC(5));
    }
    
    k_sleep(K_MSEC(300));
}
