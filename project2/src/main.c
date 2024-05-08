#ifndef MAIN_C
#define MAIN_C

#include "avr.h"
#include "lcd.h"
#include <stdio.h>
#include <string.h>

#define KEYPAD_ROW_COUNT 4
#define KEYPAD_COLUMN_COUNT 4

/*
1 = Increment Month
2 = Increment Day
3 = Increment Year
4 = Increment Hour
5 = Inctement Minute
6 = Increment Seconds
B = Military
A = Start Time
*/

typedef struct {
    int year;
    unsigned char month;
    unsigned char day;
    unsigned char hour;
    unsigned char minute;
    unsigned char second;
} DateTime;


int isLeapYear(DateTime * dt){
    return dt->year % 4 == 0 && (dt -> year % 100 != 0 || dt ->year % 400 == 0);
}

void verifyTime(DateTime * dt){
    if(dt->year < 0){
        dt->year = 2018;
    }


    if(dt->month > 12){
        dt->month = 1;
    }

    if(dt->hour > 23){
        dt->hour = 0;
    }

    if(dt->minute > 59){
        dt->minute = 0;
    }

    if(dt->second > 59){
        dt->second = 0;
    }

    if(dt->day > 28 ){
        if(dt->month == 2){
            if(dt->day > 29 && isLeapYear(dt)){
                    dt->day = 1;
                } else if(!isLeapYear(dt) && dt->day > 28){
                    dt->day = 1;
                }
            } else if(dt->month == 4 || dt->month == 6 || dt->month == 9 || dt->month == 11){
                if(dt->day > 30){
                    dt->day = 1;
                }
            } else {
            if(dt->day > 31){
                dt->day = 1;
            }
        }
    }
}

int is_pressed(int r, int c) {
    DDRC = 0;
    PINC = 0;
    SET_BIT(DDRC, r);
    CLR_BIT(PORTC, r);
    CLR_BIT(DDRC, c + 4);
    SET_BIT(PORTC, c + 4);
    avr_wait(1);
    if (GET_BIT(PINC, c + 4)) {
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

void advance_dt(DateTime *dt) {
    ++dt->second;
    if (dt->second == 60) {
        dt->second = 0;
        ++dt->minute;
        if (dt->minute == 60) {
            dt->minute = 0;
            ++dt->hour;
            if (dt->hour == 24) {
                dt->hour = 0;
                ++dt->day;

                // Array containing the number of days in each month
                int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

                // Check for leap year and adjust February days
                if (isLeapYear(dt) && dt->month == 2) {
                    days_in_month[1] = 29; // February gets 29 days in a leap year
                }

                // Check if the current day exceeds the number of days in the current month
                if (dt->day > days_in_month[dt->month - 1]) {
                    dt->day = 1;
                    ++dt->month;
                    if (dt->month == 13) {
                        dt->month = 1;
                        ++dt->year;
                    }
                }
            }
        }
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


void print_dt(const DateTime *dt, int is_military_time) {
    char buf[20];  // Buffer size to accommodate AM/PM suffix
    int hour = dt->hour;
    const char *am_pm = "AM";  // Default to AM

    lcd_pos(0, 0);
    sprintf(buf, "%02d-%02d-%04d", dt->month, dt->day, dt->year);
    lcd_puts2(buf);

    lcd_pos(1, 0);
    if (!is_military_time) {
        if (hour == 0) {
            hour = 12;  // Midnight case in 12-hour format
            am_pm = "AM";
        } else if (hour == 12) {
            am_pm = "PM";  // Noon case
        } else if (hour > 12) {
            hour = 11;
            am_pm = "PM";  // Convert to PM time
        }

        sprintf(buf, "%02d:%02d:%02d %s", hour, dt->minute, dt->second, am_pm);
    } else {
        sprintf(buf, "%02d:%02d:%02d", dt->hour, dt->minute, dt->second); // Military time does not change
    }
    lcd_puts2(buf);
}

int main(void) {
    DateTime dt;
    avr_init();
    lcd_init();
    lcd_clr();
    init_dt(&dt);
    int display_time = 0;
    int is_military_time = 0; // 0 for AM/PM, 1 for military time


    while (1) {
        int key = get_key();

        if(key){
            if(key == 4){
                if(!display_time){
                    display_time = 1;
                } else{
                    display_time = 0;
                }
            } else{
                display_time = 0;
            }

            if(key == 8){
                is_military_time = !is_military_time;
            }

            switch (key)
            {
            case 1:
                ++(&dt)->month;
                break;
            case 2:
                ++(&dt)->day;
                break;
            case 3:
                ++(&dt)->year;
                break;
            case 5:
                ++(&dt)->hour;
                break;
            case 6:
                ++(&dt)->minute;
                break;
            case 7:
                ++(&dt)->second;
                break;
            default:
                break;
            }
            verifyTime(&dt);
            lcd_clr();
            print_dt(&dt, is_military_time);
            avr_wait(150);
        }
        
        if (display_time) {
            avr_wait(1000);
            advance_dt(&dt); // Advance the time
            lcd_clr();
            print_dt(&dt, is_military_time); // Print the datetime on LCD
        }
    }
    return 0;
}

#endif
