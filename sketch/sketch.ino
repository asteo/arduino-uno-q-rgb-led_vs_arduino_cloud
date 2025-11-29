// SPDX-FileCopyrightText: Copyright (C) 2025 ARDUINO SA
// SPDX-License-Identifier: MPL-2.0

#include <Arduino_RouterBridge.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/pwm.h>

// LED3 PWM specs
static const struct pwm_dt_spec pwm_led3_r = PWM_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 5);
static const struct pwm_dt_spec pwm_led3_g = PWM_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 6);
static const struct pwm_dt_spec pwm_led3_b = PWM_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 7);

// Fade helper
void fade(const struct pwm_dt_spec *pwm) {
    uint32_t period = pwm->period;
    for (int i = 0; i <= 255; i++) {
        pwm_set_dt(pwm, period, (period * i) / 255);
        k_sleep(K_MSEC(5));
    }
    for (int i = 255; i >= 0; i--) {
        pwm_set_dt(pwm, period, (period * i) / 255);
        k_sleep(K_MSEC(5));
    }
}

// LED3 thread
void led3_thread_fn(void *, void *, void *) {
    while (1) {
        fade(&pwm_led3_r);
        k_sleep(K_MSEC(300));
        fade(&pwm_led3_g);
        k_sleep(K_MSEC(300));
        fade(&pwm_led3_b);
        k_sleep(K_MSEC(300));
    }
}

// LED4 thread
void led4_thread_fn(void *, void *, void *) {
    int state = 0;
    while (1) {
        digitalWrite(LED4_R, HIGH);
        digitalWrite(LED4_G, HIGH);
        digitalWrite(LED4_B, HIGH);
        
        switch (state) {
            case 0: digitalWrite(LED4_R, LOW); break;
            case 1: digitalWrite(LED4_G, LOW); break;
            case 2: digitalWrite(LED4_B, LOW); break;
            case 3:
                digitalWrite(LED4_R, LOW);
                digitalWrite(LED4_G, LOW);
                digitalWrite(LED4_B, LOW);
                break;
        }
        
        state = (state + 1) % 5;
        k_sleep(K_MSEC(1000));
    }
}

K_THREAD_DEFINE(led3_thread, 1024, led3_thread_fn, NULL, NULL, NULL, 7, 0, 0);
K_THREAD_DEFINE(led4_thread, 512, led4_thread_fn, NULL, NULL, NULL, 7, 0, 0);

void setup() {
    pinMode(LED4_R, OUTPUT);
    pinMode(LED4_G, OUTPUT);
    pinMode(LED4_B, OUTPUT);
    Bridge.begin();
}

void loop() {
    k_sleep(K_MSEC(1000));
}
