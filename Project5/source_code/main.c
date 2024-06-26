#ifndef MAIN_C
#define MAIN_C

#include "avr.h"
#include "lcd.h"
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SPEAKER_PIN 0

void init_keypad();
int is_pressed(int row, int col);
int get_key();
char convert_key_to_char(int key);
void display_locked();
void display_success();
void display_wrong_password(int attempts_left);
void display_menu();
void update_display_with_input(char* input_password, int pos);
int check_password(char* input_password);
void change_password();
void lockout();
void play_tone(unsigned int frequency, unsigned int duration);
void play_success_tone();
void play_error_tone();
int is_password_reused(char* new_password);

char correct_password[5] = "1234";
int loggedIn = 0;
int failed_attempts = 0;
#define MAX_ATTEMPTS 3
#define PASSWORD_HISTORY_SIZE 3

char password_history[PASSWORD_HISTORY_SIZE][5] = { "0000", "0000", "0000" };

void init_keypad() {
	DDRC = 0;
	PORTC = 0;
}

int is_pressed(int r, int c) {
	DDRC = 0;
	PINC = 0;
	SET_BIT(DDRC, r);
	CLR_BIT(PORTC, r);
	CLR_BIT(DDRC, c + 4);
	SET_BIT(PORTC, c + 4);
	avr_wait(1);
	return GET_BIT(PINC, c + 4) ? 0 : 1;
}

int get_key() {
	for (int r = 0; r < 4; r++) {
		for (int c = 0; c < 4; c++) {
			if (is_pressed(r, c)) {
				return (r * 4 + c) + 1;
			}
		}
	}
	return 0;
}

char convert_key_to_char(int key) {
	switch (key) {
		case 1: case 2: case 3: return '0' + key;
		case 4: return 'A';
		case 5: case 6: case 7: return '0' + key - 1;
		case 8: return 'B';
		case 9: case 10: case 11: return '0' + key - 2;
		case 12: return 'C';
		case 13: return '*';
		case 14: return '0';
		case 15: return '#';
		case 16: return 'D';
		default: return ' ';
	}
}

void display_locked() {
	lcd_clr();
	lcd_pos(0, 0);
	lcd_puts2("LOCKED");
	lcd_pos(1, 0);
	lcd_puts2("Password:");
}

void display_success() {
	lcd_clr();
	lcd_pos(0, 0);
	lcd_puts2("Success");
	play_success_tone();
}

void display_wrong_password(int attempts_left) {
	lcd_clr();
	lcd_pos(0, 0);
	lcd_puts2("WRONG PASSWORD");
	lcd_pos(1, 0);
	char buf[16];
	sprintf(buf, "Attempts left: %d", attempts_left);
	lcd_puts2(buf);
	play_error_tone();
	avr_wait(700);
}

void display_menu() {
	lcd_clr();
	lcd_pos(0, 0);
	lcd_puts2("1: Change Pwd");
	lcd_pos(1, 0);
	lcd_puts2("2: Lock System");
}

void update_display_with_input(char* input_password, int pos) {
	lcd_pos(1, 9);
	for (int i = 0; i < pos; i++) {
		char str[2] = {input_password[i], '\0'};
		lcd_puts2(str);
	}
}

int check_password(char* input_password) {
	return strcmp(correct_password, input_password) == 0;
}

void lockout() {
	lcd_clr();
	lcd_pos(0, 0);
	lcd_puts2("LOCKED OUT");
	play_error_tone();
	for (int i = 10; i > 0; i--) {
		lcd_pos(1, 0);
		char buf[16];
		sprintf(buf, "Wait %d sec", i);
		lcd_puts2(buf);
		avr_wait(7000);
	}
	failed_attempts = 0;
	display_locked();
}

void display_change_password() {
	lcd_clr();
	lcd_pos(0, 0);
	lcd_puts2("Change Password");
	lcd_pos(1, 0);
	lcd_puts2("Password:");
}

int is_password_reused(char* new_password) {
	for (int i = 0; i < PASSWORD_HISTORY_SIZE; i++) {
		if (strcmp(new_password, password_history[i]) == 0) {
			return 1;
		}
	}
	return 0;
}

void update_password_history(char* new_password) {
	for (int i = 0; i < PASSWORD_HISTORY_SIZE - 1; i++) {
		strcpy(password_history[i], password_history[i + 1]);
	}
	strcpy(password_history[PASSWORD_HISTORY_SIZE - 1], new_password);
}

void change_password() {
	char new_password[5] = {0};
	int pos = 0;

	display_change_password();
	avr_wait(1800);

	while (1) {
		int key = get_key();
		char input_key = convert_key_to_char(key);

		if (input_key == 'D') {
			new_password[pos] = '\0';
			if (pos == 4) {
				if (is_password_reused(new_password)) {
					lcd_clr();
					lcd_pos(0, 0);
					lcd_puts2("Pwd Reused");
					lcd_pos(1, 0);
					lcd_puts2("Try New Pwd");
					play_error_tone();
					avr_wait(7000);
					display_change_password();
					pos = 0;
					memset(new_password, 0, sizeof(new_password));
					} else {
					strcpy(correct_password, new_password);
					update_password_history(new_password);
					lcd_clr();
					lcd_pos(0, 0);
					lcd_puts2("Password Changed");
					play_success_tone();
					avr_wait(7000);
					break;
				}
				} else {
				lcd_clr();
				lcd_pos(0, 0);
				lcd_puts2("Invalid Length");
				avr_wait(7000);
				display_change_password();
				pos = 0;
				memset(new_password, 0, sizeof(new_password));
			}
			} else if (input_key >= '0' && input_key <= '9' && pos < 4) {
			new_password[pos++] = input_key;
			update_display_with_input(new_password, pos);
		}
		avr_wait(1800);
	}

	display_locked();
}

void avr_wait2(unsigned int usec) {
	TCCR0 = 3;

	while (usec--) {
		TCNT0 = (unsigned char)(256 - (XTAL_FRQ / 8) * 0.000005);
		SET_BIT(TIFR, TOV0);
		WDR();
		while (!GET_BIT(TIFR, TOV0));
	}

	TCCR0 = 0;
}

void play_tone(unsigned int frequency, unsigned int duration) {
	unsigned long period = 1000000 / frequency;
	unsigned long half_period = period / 2;

	unsigned long cycles = (duration * 1000UL) / period;
	for (unsigned long i = 0; i < cycles; i++) {
		SET_BIT(PORTA, SPEAKER_PIN);
		avr_wait2(half_period);
		CLR_BIT(PORTA, SPEAKER_PIN);
		avr_wait2(half_period);
	}
}

void play_success_tone() {
	play_tone(1000, 70);
}

void play_error_tone() {
	play_tone(200, 140);
}

int main(void) {
	avr_init();
	lcd_init();
	init_keypad();

	SET_BIT(DDRA, SPEAKER_PIN);

	display_locked();

	char input_password[5] = {0};
	int pos = 0;

	while (1) {
		int key = get_key();
		char input_key = convert_key_to_char(key);

		if (input_key == 'D') {
			input_password[pos] = '\0';
			if (check_password(input_password)) {
				display_success();
				loggedIn = 1;
				avr_wait(7000);
				display_menu();

				while (loggedIn) {
					int menu_key = get_key();
					char menu_selection = convert_key_to_char(menu_key);

					if (menu_selection == '1') {
						change_password();
						loggedIn = 0;
						} else if (menu_selection == '2') {
						loggedIn = 0;
						display_locked();
					}
					avr_wait(1800);
				}
				} else {
				failed_attempts++;
				if (failed_attempts >= MAX_ATTEMPTS) {
					lockout();
					} else {
					display_wrong_password(MAX_ATTEMPTS - failed_attempts);
				}
				display_locked();
			}
			pos = 0;
			memset(input_password, 0, sizeof(input_password));
			} else if (input_key >= '0' && input_key <= '9' && pos < 4) {
			input_password[pos++] = input_key;
			update_display_with_input(input_password, pos);
		}
		avr_wait(1800);
	}
}

#endif
