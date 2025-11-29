// SPDX-FileCopyrightText: Copyright (C) 2025 ARDUINO SA
// SPDX-License-Identifier: MPL-2.0

#include <Arduino_RouterBridge.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/pwm.h>

// LED3 PWM specs
static const struct pwm_dt_spec pwm_led3_r = PWM_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 5);
static const struct pwm_dt_spec pwm_led3_g = PWM_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 6);
static const struct pwm_dt_spec pwm_led3_b = PWM_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 7);

// Global state: 0=OFF, 1=R, 2=G, 3=B
volatile int led3_color = 1;
volatile int led4_color = 1;

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

// Bridge handler
void set_color_int(int color) {
    led3_color = color;
    led4_color = color;
}

// LED3 thread
K_THREAD_STACK_DEFINE(led3_stack, 1024);
struct k_thread led3_thread;

void led3_thread_fn(void *, void *, void *) {
    while (1) {
        const struct pwm_dt_spec *pwm = NULL;
        
        switch (led3_color) {
            case 1: pwm = &pwm_led3_r; break;
            case 2: pwm = &pwm_led3_g; break;
            case 3: pwm = &pwm_led3_b; break;
            default:
                pwm_set_dt(&pwm_led3_r, pwm_led3_r.period, 0);
                pwm_set_dt(&pwm_led3_g, pwm_led3_g.period, 0);
                pwm_set_dt(&pwm_led3_b, pwm_led3_b.period, 0);
                k_sleep(K_MSEC(100));
                continue;
        }
        
        fade(pwm);
        k_sleep(K_MSEC(300));
    }
}

// LED4 thread
K_THREAD_STACK_DEFINE(led4_stack, 512);
struct k_thread led4_thread;

void led4_thread_fn(void *, void *, void *) {
    while (1) {
        digitalWrite(LED4_R, HIGH);
        digitalWrite(LED4_G, HIGH);
        digitalWrite(LED4_B, HIGH);
        
        switch (led4_color) {
            case 1: digitalWrite(LED4_R, LOW); break;
            case 2: digitalWrite(LED4_G, LOW); break;
            case 3: digitalWrite(LED4_B, LOW); break;
        }
        
        k_sleep(K_MSEC(1000));
    }
}

void setup() {
    pinMode(LED4_R, OUTPUT);
    pinMode(LED4_G, OUTPUT);
    pinMode(LED4_B, OUTPUT);
    
    Bridge.begin();
    Bridge.provide("set_color_int", set_color_int);
    
    // Create threads AFTER Bridge is ready
    k_thread_create(&led3_thread, led3_stack, K_THREAD_STACK_SIZEOF(led3_stack),
                    led3_thread_fn, NULL, NULL, NULL, 7, 0, K_NO_WAIT);
    k_thread_create(&led4_thread, led4_stack, K_THREAD_STACK_SIZEOF(led4_stack),
                    led4_thread_fn, NULL, NULL, NULL, 7, 0, K_NO_WAIT);
}

void loop() {}
