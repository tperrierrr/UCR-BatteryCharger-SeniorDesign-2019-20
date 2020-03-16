#include <avr/io.h>
#include "io.h"
#include "io.c"
//#include <util/delay.h>
//#include <avr/interrupt.h>

void adc_init()
{
	// AREF = AVcc
	ADMUX = (1<<REFS0);
	
	// ADC Enable and prescaler of 128
	// 16000000/128 = 125000
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}


// read adc value
uint16_t adc_read(uint8_t ch)
{
	// select the corresponding channel 0~7
	// ANDing with '7' will always keep the value
	// of 'ch' between 0 and 7
	ch &= 0b00000111;  // AND operation with 7
	ADMUX = (ADMUX & 0xF8)|ch;     // clears the bottom 3 bits before ORing
	
	// start single conversion
	// write '1' to ADSC
	ADCSRA |= (1<<ADSC);
	
	// wait for conversion to complete
	// ADSC becomes '0' again
	// till then, run loop continuously
	while(ADCSRA & (1<<ADSC));
	
	return (ADC);
}

// ADC returns values between 0 and 1023
// 0 = 0V, 512 = 2.5V, 1023 = 5V
int main (void) {
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	
	LCD_init();
	//LCD_ClearScreen();
	LCD_DisplayString(1, "pls work");
	//LCD_Cursor(1);
	//LCD_WriteData(0 + '0');
	
	int i = 0;
	
	/*uint16_t voltage_reading0, voltage_reading1;
	unsigned long voltage_reading;
	adc_init();
	// unsigned long voltage_reading;
	unsigned char led_bus = 0x00;
	*/
	
	while (1) {
		/*voltage_reading0 = adc_read(0);
		voltage_reading1 = adc_read(1);
		
		voltage_reading = voltage_reading0 + voltage_reading1;
		
		if (voltage_reading >= 1) {
			led_bus = 0x01;
		} else {
			led_bus = 0x00;
		}
		
		PORTC = led_bus;
		*/
		continue;
	}
}