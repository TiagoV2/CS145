# Embedded Systems Projects

All projects in this repository were developed by Santiago Palomares and Ben Boben for the UCI Embedded Systems class.

## Project 1: LED Blink Pattern Controller

### Overview
In this project, we designed an embedded system using the ATmega32 microcontroller to control an LED blink pattern initiated by a push button. The LED blinks at a 500 ms on-off rate using an internal 1 MHz clock, which we later switched to an external 8 MHz crystal.

### Components
- **Microcontroller:** ATmega32
- **Input:** Push button
- **Output:** LED with 10kΩ resistor
- **Clock:** 8 MHz crystal oscillator
- **Programming:** PlatformIO in Visual Studio with ATATMEL-ICE programmer

### Description
The system reacts to the push button by starting and stopping the LED blinking. The code handles the transition from the internal clock to the external crystal, ensuring accurate timing and stable power with decoupling capacitors.


## Project 2: Digital Clock

### Overview
We developed a digital clock featuring a keypad for input and a 16x2 LCD for displaying the time and date. The clock displays the date in MM/DD/YYYY format and the time in HH:MM:SS format. Users can set the date and time manually through the keypad.

### Components
- **Microcontroller:** ATmega32
- **Input:** Keypad
- **Display:** 16x2 LCD

### Description
The program handles user inputs for setting the date and time, validating entries to ensure correct values, and displaying them on the LCD. The clock supports leap year adjustments and can toggle between 12-hour and 24-hour formats.


## Project 3: Music Player

### Overview
This project is a music player using the ATmega32 microcontroller. It includes a keypad for input, an LCD for displaying information, and a speaker for audio output. The player can play musical notes stored in its memory.

### Components
- **Microcontroller:** ATmega32
- **Input:** Keypad
- **Display:** LCD
- **Output:** Speaker

### Description
The music player allows users to select and play songs stored in memory, such as "Ode to Joy," "Jingle Bells," and "Mary Had a Little Lamb." The LCD shows the current song and note being played, and the keypad provides user control.


## Project 4: Digital Voltmeter

### Overview
We created a digital voltmeter that uses the ATmega32 microcontroller, a keypad for input, and an LCD for displaying voltage readings. The voltmeter can display instantaneous, maximum, minimum, and average voltage values.

### Components
- **Microcontroller:** ATmega32
- **Input:** Keypad
- **Display:** LCD
- **Analog Input:** ADC channels

### Description
The voltmeter samples analog voltage values at regular intervals, controlled by the keypad. The display shows the current, minimum, maximum, and average voltages, with specific buttons to reset or toggle sampling modes.


## Project 5: Combination Lock System

### Overview
This project is a combination lock system using the ATmega32 microcontroller. It allows users to set a single-use password, lock/unlock the system, and includes security features to prevent unauthorized access.

### Components
- **Microcontroller:** ATmega32
- **Input:** Keypad
- **Display:** LCD
- **Output:** Buzzer

### Description
The combination lock system provides visual and audio feedback for user interactions. It ensures secure access by locking out after repeated failed attempts and allows users to manage passwords through the keypad interface.

---

**Important Note:** We do not encourage or participate in any other students using the code in this repository for their coursework. This code is provided for educational purposes only and should not be used for academic dishonesty.

Thanks!
