#include "funshield.h"

constexpr int leds[] = { led1_pin, led2_pin, led3_pin };
constexpr int led_count = sizeof(leds) / sizeof(leds[0]);

constexpr int bounce[] = { 0, 1, 2, 3, 2, 1 };                      // sequence of bouncing leds
constexpr int bounce_len = sizeof(bounce) / sizeof(bounce[0]);      // length of one bounce cycle

constexpr unsigned long interval = 300;                             // milliseconds

static unsigned long start = 0;
static unsigned long current = 0;
static int led = 0;


void init_led() {
    for (int i = 0; i < led_count; i++) {
        pinMode(leds[i], OUTPUT);
        digitalWrite(leds[i], OFF);
    }
}

void setup() {
    init_led();
}

void loop() { 
    current = millis();

    pinMode(leds[bounce[led]], OUTPUT);
    digitalWrite(leds[bounce[led]], ON);

    if (current - start >= interval) {
        pinMode(leds[bounce[led]], OUTPUT);
        digitalWrite(leds[bounce[led]], OFF);

        start = current;
        led = (led + 1) % bounce_len;
    }
}

