// SPDX-FileCopyrightText: Copyright (C) 2025 ARDUINO SA
// SPDX-License-Identifier: MPL-2.0

#include <Arduino_RouterBridge.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/pwm.h>

// LED3 PWM specs
static const struct pwm_dt_spec pwm_led3_r = PWM_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 5);
static const struct pwm_dt_spec pwm_led3_g = PWM_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 6);
static const struct pwm_dt_spec pwm_led3_b = PWM_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 7);

// Global state
volatile int led3_hue = 0;
volatile int led3_brightness = 100;
volatile bool led3_switch = true;
volatile int led4_state = 0;

// HSV to RGB
void hsv_to_rgb(int h, int s, int v, uint8_t *r, uint8_t *g, uint8_t *b) {
    if (s == 0) {
        *r = *g = *b = (v * 255) / 100;
        return;
    }
    
    int region = h / 60;
    int remainder = (h % 60) * 6;
    
    int p = (v * (100 - s)) / 100;
    int q = (v * (100 - (s * remainder) / 360)) / 100;
    int t = (v * (100 - (s * (360 - remainder)) / 360)) / 100;
    
    switch (region) {
        case 0: *r = v; *g = t; *b = p; break;
        case 1: *r = q; *g = v; *b = p; break;
        case 2: *r = p; *g = v; *b = t; break;
        case 3: *r = p; *g = q; *b = v; break;
        case 4: *r = t; *g = p; *b = v; break;
        default: *r = v; *g = p; *b = q; break;
    }
    
    *r = (*r * 255) / 100;
    *g = (*g * 255) / 100;
    *b = (*b * 255) / 100;
}

// Bridge handlers
void set_hue(int hue) {
    led3_hue = hue % 360;
    Monitor.print("HUE: ");
    Monitor.println(led3_hue);
}

void set_brightness(int bri) {
    led3_brightness = bri > 100 ? 100 : (bri < 0 ? 0 : bri);
    Monitor.print("BRI: ");
    Monitor.println(led3_brightness);
}

void set_switch(bool swi) {
    led3_switch = swi;
    Monitor.print("SWI: ");
    Monitor.println(swi);
}

// LED3 thread
K_THREAD_STACK_DEFINE(led3_stack, 1024);
struct k_thread led3_thread;

void led3_thread_fn(void *, void *, void *) {
    while (1) {
        if (!led3_switch) {
            pwm_set_dt(&pwm_led3_r, pwm_led3_r.period, 0);
            pwm_set_dt(&pwm_led3_g, pwm_led3_g.period, 0);
            pwm_set_dt(&pwm_led3_b, pwm_led3_b.period, 0);
            k_sleep(K_MSEC(100));
            continue;
        }
        
        uint8_t r, g, b;
        hsv_to_rgb(led3_hue, 100, led3_brightness, &r, &g, &b);
        
        uint32_t period = pwm_led3_r.period;
        pwm_set_dt(&pwm_led3_r, period, (period * r) / 255);
        pwm_set_dt(&pwm_led3_g, period, (period * g) / 255);
        pwm_set_dt(&pwm_led3_b, period, (period * b) / 255);
        
        k_sleep(K_MSEC(50));
    }
}

// LED4 heartbeat
struct k_timer led4_timer;

void led4_timer_handler(struct k_timer *timer) {
    digitalWrite(LED4_R, HIGH);
    digitalWrite(LED4_G, HIGH);
    digitalWrite(LED4_B, HIGH);
    
    switch (led4_state) {
        case 0: digitalWrite(LED4_R, LOW); break;
        case 1: digitalWrite(LED4_G, LOW); break;
        case 2: digitalWrite(LED4_B, LOW); break;
        case 3: break;
    }
    
    led4_state = (led4_state + 1) % 4;
}

void setup() {
    pinMode(LED4_R, OUTPUT);
    pinMode(LED4_G, OUTPUT);
    pinMode(LED4_B, OUTPUT);
    
    Bridge.begin();
    Bridge.provide("set_hue", set_hue);
    Bridge.provide("set_brightness", set_brightness);
    Bridge.provide("set_switch", set_switch);
    
    Monitor.println("=== RGB LED Ready ===");
    
    k_thread_create(&led3_thread, led3_stack, K_THREAD_STACK_SIZEOF(led3_stack),
                    led3_thread_fn, NULL, NULL, NULL, 7, 0, K_NO_WAIT);
    
    k_timer_init(&led4_timer, led4_timer_handler, NULL);
    k_timer_start(&led4_timer, K_MSEC(500), K_MSEC(500));
}

void loop() {}
