// The purpose of this program is to keep a value of counter (mod 10 000) and display one digit of its value.
// Third button can be used to change the position of displayed digit (from right to left).
// First and second button increments/decrements value of selected (currently displayed) digit.

#include <funshield.h>

constexpr int buttons[] = { button1_pin, button2_pin, button3_pin };
constexpr int button_count = sizeof(buttons) / sizeof(buttons[0]);

constexpr int radix[] = { 1, 10, 100, 1000 };

static int pos = 0;
static int counter = 0;

enum purpose { INCREMENT, DECREMENT, CHANGEPOS };             // function of individual buttons

void write_glyph_bitmask(byte glyph, byte pos_bitmask) {
    digitalWrite(latch_pin, LOW);
    shiftOut(data_pin, clock_pin, MSBFIRST, glyph);
    shiftOut(data_pin, clock_pin, MSBFIRST, pos_bitmask);
    digitalWrite(latch_pin, HIGH);
}

// display glyph on position pos
// numbering of position from 0 to 3 from right to left
void write_glyph_right(byte glyph, int pos) {
    write_glyph_bitmask(glyph, digit_muxpos[4 - pos - 1]);
}

void write_digit(int n, int pos) {
    write_glyph_right(digits[n], pos);
}

void display_number(int n) {
    int number_digits[4] = { 0 };
    for (int i = 0; i < 4; i++) {
        number_digits[i] = n % 10;
        n /= 10;
    }
    write_digit(number_digits[pos], pos);
}

void incr_counter(int pos) {
    counter = (counter + radix[pos]) % 10000;
}

void decr_counter(int pos) {
    counter = (counter - radix[pos] + 10000) % 10000;
}

void change_pos() {
    pos = (pos + 1) & 3;                      // % 4 == & 3
}

class Button {
    public:
        Button(int pin, purpose fun) {
            this->pin = pin;
            this->fun = fun;
        }

        void Update() {
            current_state = digitalRead(pin);

            if (current_state != last_state) {
                last_state = current_state;

                if (current_state == LOW) {
                    switch (fun) {
                        case INCREMENT:
                            incr_counter(pos);
                            break;
                        case DECREMENT:
                            decr_counter(pos);
                            break;
                        case CHANGEPOS:
                            change_pos();
                            break;
                    }
                }
            }
        }

    private:
        int pin;
        int last_state = HIGH;
        int current_state;
        purpose fun;
};

void setup() {
    for (auto button : buttons) {
        pinMode(button, INPUT);
    }

    pinMode(latch_pin, OUTPUT);
    pinMode(clock_pin, OUTPUT);
    pinMode(data_pin, OUTPUT);

    write_glyph_bitmask(0xff, 0xff);
}

Button buttons_used[] = {
    Button(buttons[0], INCREMENT),
    Button(buttons[1], DECREMENT),
    Button(buttons[2], CHANGEPOS)
};

void loop() {
    for (int i = 0; i < button_count; i++) {
        buttons_used[i].Update();
        display_number(counter);
    }
}

