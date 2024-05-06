#ifndef MAIN_C
#define MAIN_C

#include "avr.h"
#include "lcd.h"
#include <stdio.h>

#define KEYPAD_ROW_COUNT 4
#define KEYPAD_COLUMN_COUNT 4

int is_pressed(int r, int c) {
    DDRA = 0;
    PINA = 0;
    SET_BIT(DDRA, r);
    CLR_BIT(PORTA, r);
    CLR_BIT(DDRA, c + 4);
    SET_BIT(PORTA, c + 4);
    avr_wait(1);
    if (GET_BIT(PINA, c + 4)) {
        return 0;
    } else {
        return 1;
    }
}

int get_key() {
    int r, c;
    for (r = 0; r < 4; r++) {
        for (c = 0; c < 4; c++) {
            if (is_pressed(r, c)) {
                return (r * 4 + c) + 1;
            }
        }
    }
    return 0;
}

typedef struct {
    int year;
    unsigned char month;
    unsigned char day;
    unsigned char hour;
    unsigned char minute;
    unsigned char second;
} DateTime;

void advance_dt(DateTime *dt) {
    ++dt->second;
    if (dt->second == 60) {
        ++dt->minute;
        if (dt->minute == 60) {
            ++dt->hour;
            if (dt->hour == 24) {
                ++dt->day;
                // Simplify and assume each month has 30 days for example purpose
                if (dt->day == 31) {
                    dt->day = 1;
                    ++dt->month;
                    if (dt->month == 13) {
                        dt->month = 1;
                        ++dt->year;
                    }
                }
                dt->hour = 0;
            }
            dt->minute = 0;
        }
        dt->second = 0;
    }
}

void init_dt(DateTime *dt) {
    dt->year = 2024;
    dt->month = 4;
    dt->day = 26;
    dt->hour = 12;
    dt->minute = 53;
    dt->second = 0;
}

void print_dt(const DateTime *dt) {
    char buf[17];
    lcd_pos(0, 0);
    sprintf(buf, "%02d-%02d-%04d", dt->month, dt->day, dt->year);
    lcd_puts2(buf);

    lcd_pos(1, 0);
    sprintf(buf, "%02d:%02d:%02d", dt->hour, dt->minute, dt->second);
    lcd_puts2(buf);
}

int main(void) {
    DateTime dt;
    avr_init();
    lcd_init();
    lcd_clr();
    init_dt(&dt);
    int display_time = 0;

    while (1) {
        int key = get_key();
        if (key == 1) { // Check if the key '1' is pressed
            display_time = 1;
        }

        if (display_time) {
            avr_wait(1000);
            advance_dt(&dt); // Advance the time
            lcd_clr();
            print_dt(&dt); // Print the datetime on LCD
        }
    }
    return 0;
}

#endif
