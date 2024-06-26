#ifndef MAIN_C
#define MAIN_C

#include "avr.h"
#include "lcd.h"
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>

// Helper functions
void init_keypad();
int is_pressed(int row, int col);
int get_key();
char convert_key_to_char(int key);
int read_adc_channel(int channel);
void reset_readings(unsigned long *sum, int *min, int *max, int *count);
void display_readings_reset();
void display_readings(float current, float avg, float min, float max);

// Initialization
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
		default: return '0';
	}
}

// ADC functions
int read_adc_channel(int channel) {
	ADMUX = (0b01000000) | channel;
	ADCSRA = 0b11000000;
	while (GET_BIT(ADCSRA, 6));
	return ADC;
}

// Reset readings
void reset_readings(unsigned long *sum, int *min, int *max, int *count) {
	*sum = 0;
	*min = 1200; // Larger than any possible value
	*max = 0;
	*count = 1;
}

// Display reset readings
void display_readings_reset() {
	char buf[20];

	lcd_clr();

	// Current
	sprintf(buf, "C:----");
	lcd_pos(0, 0);
	lcd_puts2(buf);

	// AVG
	sprintf(buf, "M:----");
	lcd_pos(0, 9);
	lcd_puts2(buf);

	// Min
	sprintf(buf, "L:----");
	lcd_pos(1, 0);
	lcd_puts2(buf);

	// Max
	sprintf(buf, "H:----");
	lcd_pos(1, 9);
	lcd_puts2(buf);
}

// Display readings
void display_readings(float current, float avg, float min, float max) {
	char buf[20];

	lcd_clr();

	// Current
	sprintf(buf, "C:%.2f", current);
	lcd_pos(0, 0);
	lcd_puts2(buf);

	// AVG
	sprintf(buf, "M:%.2f", avg);
	lcd_pos(0, 9);
	lcd_puts2(buf);

	// Min
	sprintf(buf, "L:%.2f", min);
	lcd_pos(1, 0);
	lcd_puts2(buf);

	// Max
	sprintf(buf, "H:%.2f", max);
	lcd_pos(1, 9);
	lcd_puts2(buf);
}

int main(void) {
	unsigned long int sum = 0;
	int min_value = 1200; // Larger than any possible value
	int max_value = 0;
	int count = 1;
	int current_sample;
	int read_enabled = 0;
	int diff_mode = 0;

	avr_init();
	lcd_init();
	display_readings_reset(); // Initialize display with '----'

	while (1) {
		int key = get_key();
		char input_key = convert_key_to_char(key);

		if (input_key == '1') {
			reset_readings(&sum, &min_value, &max_value, &count);
			display_readings_reset();
			read_enabled = 0;
			diff_mode = 0;
			} else if (input_key == '2') {
			read_enabled = !read_enabled;
			} else if (input_key == '3') {
			diff_mode = !diff_mode;
		}

		if (read_enabled) {
			if (diff_mode) {
				current_sample = abs(read_adc_channel(0) - read_adc_channel(1));
			} else {
				current_sample = read_adc_channel(0);
			}

			sum += current_sample;
			if (current_sample < min_value) {
				min_value = current_sample;
			}
			if (current_sample > max_value) {
				max_value = current_sample;
			}

			display_readings(
			(current_sample / 1023.0) * 5,
			((float)sum / count) / 1023.0 * 5,
			(min_value / 1023.0) * 5,
			(max_value / 1023.0) * 5
			);
			++count;
		}
		avr_wait(450); // Compensate for operations taking ~50ms
	}
}

#endif
