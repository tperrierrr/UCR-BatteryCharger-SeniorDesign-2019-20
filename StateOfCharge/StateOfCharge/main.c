#include <avr/io.h>
#include "io.c"
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>

void adc_init()
{
	// AREF = AVcc
	ADMUX = (0<<REFS0);
	
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

enum States{init, Sample, Display}state;

// ADC Variables
uint16_t voltage_reading0;
unsigned long voltage_reading;

// Display + math Variables
double BatVolt = 0;
double BatPcnt = 0;
unsigned char CV_Flag = 0;
// slopes
double m1 = -0.419; double m2 = -0.397; double m3 = -0.279; double m4 = -0.24;
double m5 = -0.08; double m6 = -0.12; double m7 = -0.281; double m8 = -0.466;
double mAh = 0;
char BatVoltStr[10];
char BatPcntStr[10];

void SampleVoltage () {
	switch(state) {		// transitions
		case init:
			state = Sample;
			break;
		
		case Sample:
			state = Display;
			break;
		
		case Display:
			state = Sample;
			break;
		
		default:
			break;
	}
	
	switch (state) {	// actions
		case init:
			break;
		
		case Sample:
			PORTB = 0x00;
			delay_ms(200);
			voltage_reading0 = adc_read(0);
			delay_ms(100);
			break;
		
		case Display:
			PORTB0 = 1;
			if (CV_Flag == 1) {
				PORTB2 = 1;
			}
			
			BatVolt = (voltage_reading0 * 5.06) / 1024.0;
			dtostrf(BatVolt, 4, 2, BatVoltStr);
			
			if (BatVolt <= 4.2 && BatVolt > 4.043) {
				if (BatVolt >= 4.18) {
					CV_Flag = 1;
				}
				mAh = (BatVolt - 4.2) / m1;
			} else if (BatVolt <= 4.043 && BatVolt > 3.927) {
				mAh = (BatVolt - 4.043) / m2 + 0.375;
			} else if (BatVolt <= 3.927 && BatVolt > 3.805) {
				mAh = (BatVolt - 3.927) / m3 + 0.667;
			} else if (BatVolt <= 3.805 && BatVolt > 3.77) {
				mAh = (BatVolt - 3.805) / m4 + 1.104;
			}else if (BatVolt <= 3.77 && BatVolt > 3.75) {
				mAh = (BatVolt - 3.77) / m5 + 1.25;
			}else if (BatVolt <= 3.75 && BatVolt > 3.72) {
				mAh = (BatVolt - 3.75) / m6 + 1.5;
			} else if (BatVolt <= 3.72 && BatVolt > 3.685) {
				mAh = (BatVolt - 3.72) / m7 + 1.75;
			} else if (BatVolt <= 3.685 && BatVolt > 3.452) {
				mAh = (BatVolt - 3.685) / m8 + 1.979;
			} else if (BatVolt <= 3.452 && BatVolt >= 3.0) {
				mAh = 2.479 + sqrt((BatVolt - 3.452) / (-30.87));
			} else if (BatVolt < 3.0) {
				mAh = 2.6;
				BatPcnt = 0;
			} else {
				BatPcnt = 100.0;
				mAh = 0.0;
			}
			
			BatPcnt = 100 * (2.6 - mAh) / 2.6;
			dtostrf(BatPcnt, 4, 1, BatPcntStr);
			
			LCD_DisplayString(1, "BattV:");
			LCD_DisplayString(7, BatVoltStr);
			LCD_DisplayString(11, "V");
			LCD_DisplayString(17, "BattPcnt:");
			LCD_DisplayString(26, BatPcntStr);
			LCD_DisplayString(32, "%");
			
			delay_ms(30000);
			
			break;
	}
}

int main (void) {
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	
	LCD_init();
	
	adc_init();
	
	LCD_ClearScreen();
	
	while (1) {
		
		SampleVoltage();
			
		//continue;
	}
}