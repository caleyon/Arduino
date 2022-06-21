// The purpose of this program is to display value of counter in binary using four LEDs.
// Counter can be incremented/decremented using two buttons.
// Each press of the button increments/decrements the counter.
// If the button is held for 1 second (interval_1), the value of counter is incremented/decremented one more time.
// If the button is still pressed after this time, value is changed every 200 milliseconds (interval_2).

#include <funshield.h>

constexpr int leds[] = { led1_pin, led2_pin, led3_pin };
constexpr int led_count = sizeof(leds) / sizeof(leds[0]);

constexpr int buttons[] = { button1_pin, button2_pin, button3_pin };
constexpr int button_count = sizeof(buttons) / sizeof(buttons[0]);

constexpr unsigned long interval_1 = 1000;
constexpr unsigned long interval_2 = 200;

constexpr int used_buttons_count = 2;

static int counter = 0;


// turns all LEDs off
void init_led() {
    for (int i = 0; i < led_count; i++) {
        pinMode(leds[i], OUTPUT);
        digitalWrite(leds[i], OFF);
    }
}

// display counter in binary using leds 
void display_bin(int n) {
    for (int i = 3; i >= 0; i--) {
        if ((n & 1) == 1) {
            digitalWrite(leds[i], ON);
        }
        n /= 2;
    }
}

class Button {
    public:
        Button(int pin, bool increments) {
            this->pin = pin;
            this->increments = increments;
        }

        void Change_counter() {
            if (increments) {
                counter = (counter + 1) & 15;           // % 2^n = & (2^n - 1)
            } else {
                counter = (counter + 16 - 1) & 15;
            }

            display_bin(counter);
        }

        void Update() {
            current_state = digitalRead(pin);

            // state of the button changed
            if (current_state != last_state) {
                last_state = current_state;

                if (current_state == LOW) {
                    init_led();
                    Change_counter();
                    start = millis();
                    pressed = true;
                    long_pressed = false;
                } else {
                    pressed = false;
                    long_pressed = false;
                }
            // button is still pressed
            } else if (pressed && !long_pressed) {
                current = millis();

                if (current - start >= interval_1) {
                    init_led();
                    Change_counter();
                    start = millis();
                    long_pressed = true;
                }
            // button has been pressed for more than interval_1 (1 second)
            } else if (pressed && long_pressed) {
                if (millis() - start >= interval_2) {
                    init_led();
                    Change_counter();
                    start = millis();
                }
            }
        }

    private:
        int pin;
        int last_state = HIGH;
        int current_state;
        bool increments;

        unsigned long start = 0;
        unsigned long current = 0;

        bool pressed = false;
        bool long_pressed = false;
};

Button used_buttons[] = {
    Button(button1_pin, true),
    Button(button2_pin, false),
};

void setup() {
    init_led();

    // initialize buttons
    for (auto button : buttons) {
        pinMode(button, INPUT);
    }
}

void loop() {
    for (int i = 0; i < used_buttons_count; i++) {
        used_buttons[i].Update();
    }
}

