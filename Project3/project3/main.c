#ifndef MAIN_C
#define MAIN_C

#include "avr.h"
#include "lcd.h"
#include <stdio.h>
#include <string.h>

#define KEYPAD_ROW_COUNT 4
#define KEYPAD_COLUMN_COUNT 4

#define A_INDEX 0
#define A_SHARP_INDEX 1
#define B_INDEX 2
#define B_SHARP_INDEX 3
#define C_INDEX 4
#define C_SHARP_INDEX 5
#define D_INDEX 6
#define D_SHARP_INDEX 7
#define E_INDEX 8
#define E_SHARP_INDEX 9
#define F_INDEX 10
#define F_SHARP_INDEX 11
#define G_INDEX 12
#define G_SHARP_INDEX 13

#define A_NOTE 440
#define A_SHARP_NOTE 466
#define B_NOTE 493
#define B_SHARP_NOTE 523
#define C_NOTE 554
#define C_SHARP_NOTE 587
#define D_NOTE 622
#define D_SHARP_NOTE 659
#define E_NOTE 880
#define E_SHARP_NOTE 739
#define F_NOTE 783
#define F_SHARP_NOTE 830
#define G_NOTE 880
#define G_SHARP_NOTE 932

void avr_wait2(unsigned int usec);
void note_to_play(unsigned int frequency, unsigned int duration);
void play_ode_to_joy(void);
void play_jingle_bells(void);
void play_mary_had_a_little_lamb(void);
const char* translateFreq(int freq);
int is_pressed(int r, int c);
int is_pressed_no_wait(int r, int c);
int get_key(void);
int get_key_no_wait(void);
void print_row1(const char *string);
void print_row2(const char *string);

static char line1[16];
static char line2[16];

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

void note_to_play(unsigned int frequency, unsigned int duration) {
	int i;
	unsigned int period = (unsigned int)((100000) / frequency);
	unsigned int tHigh = period / 2;
	unsigned int tLow = period / 2;
	unsigned int cycles = frequency * duration;
	int is_paused = 0; // State variable to track if the playback is paused

	// Display the note being played
	snprintf(line2, sizeof(line2), "%s", translateFreq(frequency));
	print_row2(line2);

	for (i = 0; i < cycles; i++) {
		if (is_paused) {
			// Check if key 4 is pressed to resume
			if (get_key() == 4) {
				is_paused = 0; // Resume playback
				avr_wait(1000); // Debounce delay
			}
			// Stay in the paused state
			i--; // Maintain the current cycle count
			} else {
			// Check if key 4 is pressed to pause
			if (get_key_no_wait() == 4) {
				is_paused = 1; // Enter paused state
				avr_wait(1000); // Debounce delay
				} else {
				// Normal playback
				SET_BIT(PORTB, 3);
				avr_wait2(tHigh);
				CLR_BIT(PORTB, 3);
				avr_wait2(tLow);
			}
		}
	}
}

void play_ode_to_joy() {
	snprintf(line1, sizeof(line1), "ODE TO JOY");
	print_row1(line1);

	note_to_play(E_NOTE, 2);
	note_to_play(E_NOTE, 2);
	note_to_play(F_NOTE, 2);
	note_to_play(G_NOTE, 2);
	note_to_play(G_NOTE, 2);
	note_to_play(F_NOTE, 2);
	note_to_play(E_NOTE, 2);
	note_to_play(D_NOTE, 2);
	note_to_play(C_NOTE, 2);
	note_to_play(C_NOTE, 2);
	note_to_play(D_NOTE, 2);
	note_to_play(E_NOTE, 2);
	note_to_play(E_NOTE, 3);
	note_to_play(D_NOTE, 1);
	note_to_play(D_NOTE, 4);

	note_to_play(E_NOTE, 2);
	note_to_play(E_NOTE, 2);
	note_to_play(F_NOTE, 2);
	note_to_play(G_NOTE, 2);
	note_to_play(G_NOTE, 2);
	note_to_play(F_NOTE, 2);
	note_to_play(E_NOTE, 2);
	note_to_play(D_NOTE, 2);
	note_to_play(C_NOTE, 2);
	note_to_play(C_NOTE, 2);
	note_to_play(D_NOTE, 2);
	note_to_play(E_NOTE, 2);
	note_to_play(D_NOTE, 3);
	note_to_play(C_NOTE, 1);
	note_to_play(C_NOTE, 4);
}

void play_jingle_bells() {
	snprintf(line1, sizeof(line1), "JINGLE BELLS");
	print_row1(line1);

	note_to_play(E_NOTE, 1);
	note_to_play(E_NOTE, 1);
	note_to_play(E_NOTE, 2);
	note_to_play(E_NOTE, 1);
	note_to_play(E_NOTE, 1);
	note_to_play(E_NOTE, 2);
	note_to_play(E_NOTE, 1);
	note_to_play(G_NOTE, 1);
	note_to_play(C_NOTE, 1);
	note_to_play(D_NOTE, 1);
	note_to_play(E_NOTE, 4);

	note_to_play(F_NOTE, 1);
	note_to_play(F_NOTE, 1);
	note_to_play(F_NOTE, 1);
	note_to_play(F_NOTE, 1);
	note_to_play(F_NOTE, 1);
	note_to_play(E_NOTE, 1);
	note_to_play(E_NOTE, 1);
	note_to_play(E_NOTE, 1);
	note_to_play(E_NOTE, 1);
	note_to_play(D_NOTE, 1);
	note_to_play(D_NOTE, 1);
	note_to_play(E_NOTE, 1);
	note_to_play(D_NOTE, 1);
	note_to_play(G_NOTE, 4);
}

void play_mary_had_a_little_lamb() {
	snprintf(line1, sizeof(line1), "MARY HAD A");
	print_row1(line1);

	note_to_play(E_NOTE, 2);
	note_to_play(D_NOTE, 2);
	note_to_play(C_NOTE, 2);
	note_to_play(D_NOTE, 2);
	note_to_play(E_NOTE, 2);
	note_to_play(E_NOTE, 2);
	note_to_play(E_NOTE, 4);
	note_to_play(D_NOTE, 2);
	note_to_play(D_NOTE, 2);
	note_to_play(D_NOTE, 4);
	note_to_play(E_NOTE, 2);
	note_to_play(G_NOTE, 2);
	note_to_play(G_NOTE, 4);

	note_to_play(E_NOTE, 2);
	note_to_play(D_NOTE, 2);
	note_to_play(C_NOTE, 2);
	note_to_play(D_NOTE, 2);
	note_to_play(E_NOTE, 2);
	note_to_play(E_NOTE, 2);
	note_to_play(E_NOTE, 2);
	note_to_play(E_NOTE, 2);
	note_to_play(D_NOTE, 2);
	note_to_play(D_NOTE, 2);
	note_to_play(E_NOTE, 2);
	note_to_play(D_NOTE, 2);
	note_to_play(C_NOTE, 4);
}

const char* translateFreq(int freq) {
	switch (freq) {
		case 440:
		return "A NOTE ";
		case 466:
		return "A SHARP";
		case 493:
		return "B NOTE ";
		case 523:
		return "B SHARP";
		case 554:
		return "C NOTE ";
		case 587:
		return "C SHARP";
		case 622:
		return "D NOTE ";
		case 659:
		return "D SHARP";
		case 694:
		return "E NOTE ";
		case 739:
		return "E SHARP";
		case 783:
		return "F NOTE ";
		case 830:
		return "F SHARP";
		case 880:
		return "G NOTE ";
		case 932:
		return "G SHARP";
		default: return "None";
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

int is_pressed_no_wait(int r, int c) {
	DDRC = 0;
	PINC = 0;
	SET_BIT(DDRC, r);
	CLR_BIT(PORTC, r);
	CLR_BIT(DDRC, c + 4);
	SET_BIT(PORTC, c + 4);
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

int get_key_no_wait() {
	int r, c;
	for (r = 0; r < 4; r++) {
		for (c = 0; c < 4; c++) {
			if (is_pressed_no_wait(r, c)) {
				int key = (r * 4 + c) + 1;
				if(key == 4){
					return key;
				}
			}
		}
	}
	return 0;
}

void print_row1(const char *string) {
	lcd_pos(0, 0);
	lcd_puts2(string);
}

void print_row2(const char *string) {
	lcd_pos(1, 0);
	lcd_puts2(string);
}

int main(void) {
	avr_init();
	lcd_init();
	lcd_clr();

	SET_BIT(DDRB, 3);

	while (1) {
		const char* opener = "PICK A SONG";
		print_row1(opener);
		const char* last_line = " ";
		print_row2(last_line);

		int key = get_key();
		if (key) {
			switch (key) {
				case 1:
				play_ode_to_joy();
				break;
				case 2:
				play_jingle_bells();
				break;
				case 3:
				play_mary_had_a_little_lamb();
				break;
				default:
				break;
			}
		}
	}

	return 0;
}

#endif
