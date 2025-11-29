// SPDX-FileCopyrightText: Copyright (C) 2025 ARDUINO SA
// SPDX-License-Identifier: MPL-2.0

// Clean PWM abstraction - embedded approach

#include <Arduino_RouterBridge.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/pwm.h>

// ============================================================================
// PWM LED Abstraction
// ============================================================================

struct pwm_led {
    const struct pwm_dt_spec spec;
    
    // Set brightness (0-255)
    inline void set(uint8_t brightness) const {
        uint32_t pulse = (spec.period * brightness) / 255;
        pwm_set_dt(&spec, spec.period, pulse);
    }
    
    // Turn off
    inline void off() const {
        pwm_set_dt(&spec, spec.period, 0);
    }
    
    // Turn on full
    inline void on() const {
        pwm_set_dt(&spec, spec.period, spec.period);
    }
    
    // Check if ready
    inline bool is_ready() const {
        return pwm_is_ready_dt(&spec);
    }
};

// LED3 RGB (Hardware PWM)
static const pwm_led LED3_PWM_R = { PWM_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 5) };
static const pwm_led LED3_PWM_G = { PWM_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 6) };
static const pwm_led LED3_PWM_B = { PWM_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 7) };

// ============================================================================
// LED4 Software Timer
// ============================================================================

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

// ============================================================================
// Setup & Loop
// ============================================================================

void setup() {
    // LED4 digital pins
    pinMode(LED4_R, OUTPUT);
    pinMode(LED4_G, OUTPUT);
    pinMode(LED4_B, OUTPUT);
    
    // Start LED4 timer
    k_timer_init(&led4_timer, led4_timer_handler, NULL);
    k_timer_start(&led4_timer, K_MSEC(500), K_MSEC(500));
    
    Bridge.begin();
}

void loop() {
    // Fade RED
    for (uint8_t i = 0; i <= 255; i++) {
        LED3_PWM_R.set(i);
        k_sleep(K_MSEC(5));
    }
    for (int i = 255; i >= 0; i--) {
        LED3_PWM_R.set(i);
        k_sleep(K_MSEC(5));
    }
    LED3_PWM_R.off();
    
    k_sleep(K_MSEC(300));
    
    // Fade GREEN
    for (uint8_t i = 0; i <= 255; i++) {
        LED3_PWM_G.set(i);
        k_sleep(K_MSEC(5));
    }
    for (int i = 255; i >= 0; i--) {
        LED3_PWM_G.set(i);
        k_sleep(K_MSEC(5));
    }
    LED3_PWM_G.off();
    
    k_sleep(K_MSEC(300));
    
    // Fade BLUE
    for (uint8_t i = 0; i <= 255; i++) {
        LED3_PWM_B.set(i);
        k_sleep(K_MSEC(5));
    }
    for (int i = 255; i >= 0; i--) {
        LED3_PWM_B.set(i);
        k_sleep(K_MSEC(5));
    }
    LED3_PWM_B.off();
    
    k_sleep(K_MSEC(300));
}
