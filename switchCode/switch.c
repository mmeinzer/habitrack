// Set the CPU frequency to 1 MHz
#define F_CPU 1000000UL

// LED DDR, PORT, and pin locations
#define LED_DDR DDRB
#define LED_PORT PORTB
#define LED_0 PB1
#define LED_1 PB2
#define LED_2 PB3
#define LED_3 PB4
#define LED_4 PB5

// Button DDR, PIN, pull-up resistor, and pin locations
#define BUTTON_DDR DDRD
#define BUTTON_PULL_UP PORTD
#define BUTTON_PIN PIND
#define BUTTON_1 PD2
#define BUTTON_2 PD3

// Delay time before reading button status after button press
#define DEBOUNCE_TIME 20000

// Time in between increments of count (in milliseconds)
#define TIMER 1000

// Upper limit for counter
#define MAX_COUNT 32

// External libraries
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile uint8_t count = 0;

// This method is used to determine the status of Button 2
int Button2Pressed(void) {
	// Return the status of the button
	return (BUTTON_PIN & (1 << BUTTON_2));
}

// Interrupt method that runs every time INT0 is triggered (Button 1 pressed)
ISR(INT0_vect) {
  // Debounce the button with a time delay (microseconds)
  _delay_us(DEBOUNCE_TIME);
  count++;
}

// Initialize interrupt
void initInterrupt0(void) {
	EIMSK |= (1 << INT0); // Enable INT0
	//EICRA |= (1 << ISC00); // Trigger on change to INT0
  EICRA |= (1 << ISC01); // Trigger on change to INT0
	sei(); // Set global interrupt enable bit
}

int main(void) {

	// Variable initialization
	uint8_t buttonWasPressed;

	// Initialize BUTTON_2 as an input
	BUTTON_DDR &= ~(1 << BUTTON_2);

  BUTTON_PULL_UP |= (1 << PD2);
  BUTTON_PULL_UP |= (1 << PD3);

	// Set LED pins to be outputs
	LED_DDR |= (1 << LED_0);
	LED_DDR |= (1 << LED_1);
	LED_DDR |= (1 << LED_2);
	LED_DDR |= (1 << LED_3);
	LED_DDR |= (1 << LED_4);

	// Initialize interrupts
	initInterrupt0();

	// Loop forever
	while(1) {


		// Check if button is pressed
		if (!Button2Pressed()) {

			// Debounce the button with a time delay (microseconds)
			_delay_us(DEBOUNCE_TIME);

			// Check for button press after bouncing has finished
			if (buttonWasPressed == 0) {

				// Set flag for button press
				buttonWasPressed = 1;

				// Reset count
				count = 0;
			}

			// Clear flag for button press
			else {
				buttonWasPressed = 0;
			}
		}

		// If the count isn't a maximum, increment the counter and light
		// up the proper number LEDs
		if (count != MAX_COUNT) {
			if (count & 0b00001) {
				LED_PORT |= (1 << LED_0);
			}
			else {
				LED_PORT &= ~(1 << LED_0);
			}

			// Set the second LED, depending on the value of count
			if (count & 0b00010) {
				LED_PORT |= (1 << LED_1);
			}
			else {
				LED_PORT &= ~(1 << LED_1);
			}

			// Set the third LED, depending on the value of count
			if (count & 0b00100) {
				LED_PORT |= (1 << LED_2);
			}
			else {
				LED_PORT &= ~(1 << LED_2);
			}

			// Set the fourth LED, depending on the value of count
			if (count & 0b01000) {
				LED_PORT |= (1 << LED_3);
			}
			else {
				LED_PORT &= ~(1 << LED_3);
			}

			// Set the fifth LED (MSB), depending on the value of count
			if (count & 0b10000) {
				LED_PORT |= (1 << LED_4);
			}
			else {
				LED_PORT &= ~(1 << LED_4);
			}

		}

	}
	return 0;
}
