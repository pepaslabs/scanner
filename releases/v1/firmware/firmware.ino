// firmware.ino: Arduino sketch to control the Scanner board. 
// See https://github.com/pepaslabs/scanner.

// Copyright 2022 Jason Pepas.
// Released under the terms of the MIT License.
// See https://opensource.org/licenses/MIT.

#define CLOCK_PIN 4
#define DATA_PIN 2
#define LATCH_PIN 3
#define LED_PIN 13

#define RELAY_DWELL_MS (6)

#define SHIFT_REGISTER_COUNT (4)
uint8_t shift_registers[SHIFT_REGISTER_COUNT] = {0};

#define REF_A (0)
#define REF_B (1)
#define REF_C (2)
#define REF_D (3)
#define REF_E (4)
#define REF_F (5)
#define REF_G (6)
#define REF_H (7)

// LSBFIRST:
// Byte 0: B+d A-d B+e A-e A+e A+d B-d B-e
// Byte 1: C-e C-d C+d C+e D+d D+e E+d E+e
// Byte 2: D-d D-e E-d E-e F-d F-e F+d F+e
// Byte 3: H+d H+e G-d G-e G+e G+d H-e H-d

void set_ref_enabled(uint8_t ref_index) {
    switch (ref_index) {
    case REF_A:
        shift_registers[0] |= 0b00011000;
        break;
    case REF_B:
        shift_registers[0] |= 0b00100001;
        break;
    case REF_C:
        shift_registers[1] |= 0b10010000;
        break;
    case REF_D:
        shift_registers[1] |= 0b00000100;
        shift_registers[2] |= 0b01000000;
        break;
    case REF_E:
        shift_registers[1] |= 0b00000001;
        shift_registers[2] |= 0b00010000;
        break;
    case REF_F:
        shift_registers[2] |= 0b00000101;
        break;
    case REF_G:
        shift_registers[3] |= 0b00011000;
        break;
    case REF_H:
        shift_registers[3] |= 0b01000010;
        break;
    }
}

// LSBFIRST:
// Byte 0: B+d A-d B+e A-e A+e A+d B-d B-e
// Byte 1: C-e C-d C+d C+e D+d D+e E+d E+e
// Byte 2: D-d D-e E-d E-e F-d F-e F+d F+e
// Byte 3: H+d H+e G-d G-e G+e G+d H-e H-d

void set_ref_disabled(uint8_t ref_index) {
    switch (ref_index) {
    case REF_A:
        shift_registers[0] |= 0b01000100;
        break;
    case REF_B:
        shift_registers[0] |= 0b10000010;
        break;
    case REF_C:
        shift_registers[1] |= 0b01100000;
        break;
    case REF_D:
        shift_registers[1] |= 0b00001000;
        shift_registers[2] |= 0b10000000;
        break;
    case REF_E:
        shift_registers[1] |= 0b00000010;
        shift_registers[2] |= 0b00100000;
        break;
    case REF_F:
        shift_registers[2] |= 0b00001010;
        break;
    case REF_G:
        shift_registers[3] |= 0b00100100;
        break;
    case REF_H:
        shift_registers[3] |= 0b10000001;
        break;
    }
}

void reset_registers() {
    for (uint8_t i=0; i < SHIFT_REGISTER_COUNT; i++) {
        shift_registers[i] = 0;
    }
}

void send_registers() {
    digitalWrite(LATCH_PIN, LOW);
    for (int8_t i=SHIFT_REGISTER_COUNT-1; i >= 0; i--) {
        shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, shift_registers[i]);
    }
    digitalWrite(LATCH_PIN, HIGH);
}

void enable_ref(uint8_t ref_index) {
    set_ref_enabled(ref_index);
    send_registers();
    delay(RELAY_DWELL_MS);
    reset_registers();
    send_registers();
    delay(1);
}

void disable_ref(uint8_t ref_index) {
    set_ref_disabled(ref_index);
    send_registers();
    delay(RELAY_DWELL_MS);
    reset_registers();
    send_registers();
    delay(1);
}

void setup() {
    pinMode(CLOCK_PIN, OUTPUT);
    pinMode(DATA_PIN, OUTPUT);
    pinMode(LATCH_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);

    for (uint8_t i = 0; i < 8; i++) {
        disable_ref(i);
    }
}

void loop() {
    digitalWrite(LED_PIN, HIGH);
    enable_ref(0);
    delay(1000);
    digitalWrite(LED_PIN, LOW);
    disable_ref(0);

    for (uint8_t i = 1; i < 8; i++) {
        enable_ref(i);
        delay(1000);
        disable_ref(i);
    }
}
