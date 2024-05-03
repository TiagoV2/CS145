#ifndef MAIN_C
#define MAIN_C

#include "avr.h"
#include "lcd.h"
#include <stdio.h>

#define KEYPAD_ROW_COUNT 4
#define KEYPAD_COLUMN_COUNT 4

// int get_key(){
// 	int row,col;
// 	for(row = 0; row < KEYPAD_ROW_COUNT;row++){
// 		for(col = 0; col < KEYPAD_COLUMN_COUNT; col++){
// 			if(is_pressed(row,col)){
// 				int code = row * KEYPAD_COLUMN_COUNT + col + 1;
// 				return code;
// 			}
// 		}
// 	}
// 	return 0;
// }

typedef struct
{
  int year;
  unsigned char month;
  unsigned char day;
  unsigned char hour;
  unsigned char minute;
  unsigned char second;
} DateTime;

void advance_dt(DateTime *dt)
{
  ++dt->second;
  if(dt->second == 60)
  {
    // increase minute and check.
    ++dt->minute;
    // repeat process as needed.
    dt->second = 0;
  }
}

void init_dt(DateTime *dt)
{
  dt->year = 2024;
  dt->month = 4;
  dt->day = 26;
  dt->hour = 12;
  dt->minute = 53;
  dt->second = 0;
}

void print_dt(const DateTime *dt)
{
    char buf[17];

    // Print the date on the first row.
    lcd_pos(0, 0); // Row 0, Column 0
    sprintf(buf, "%02d-%02d-%04d", dt->month, dt->day, dt->year);
    lcd_puts2(buf);

    // Print the time on the second row.
    lcd_pos(1, 0); // Row 1, Column 0
    sprintf(buf, "%02d:%02d:%02d", dt->hour, dt->minute, dt->second);
    lcd_puts2(buf);
}


int main(void)
{
  DateTime dt;
  avr_init();
  lcd_init();
  lcd_clr();
  init_dt(&dt);
  while (1)
  {
    avr_wait(1000);
    lcd_clr();
    // advance_dt(&dt);
    print_dt(&dt);
  }
  return 0;
}

// int isLeapYear(){
// 	return years % 4 == 0 && (years % 100 != 0 || years % 400 == 0);
// }

#endif