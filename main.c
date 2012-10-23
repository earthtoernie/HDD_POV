/*
 * main.c
 *
 * Created: 8/14/2012 2:35:21 PM
 *  Author: Ernie
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "twi_tx_rx.h"
#include "led_utils.h"
// rgb led's
#define B_ON PORTB5
#define G_ON PORTB6
#define R_ON PORTB7
#define RGB_PORT PORTB
#define RGB_DDR DDRB
// twi addresses
#define LCD_WRITE_ADDR 0x90
#define RTC_READ_ADDR 0b11010001
#define RTC_WRITE_ADDR 0b11010000
// encoder
#define ENCODER_PORT PORTE
#define ENCODER_DDR DDRE
#define ENCODER_PIN PINE
#define ENCODER_BUTTON PINE3
#define ENCODER_A PINE4
#define ENCODER_B PINE6

int main(void)
{	
	_delay_ms(1000);
	ENABLE_DEBUG_LEDS;
	LED3_ON;
	wait_for_push();
	twiInit();
//	init_rgb_leds();
//	init_optical_interrupt();
	//test twi functions
	uint8_t commands[] = {0x1B, 0x5B, 'j'}; // reset lcd cursor and clear screen
	uint8_t data[] = {'H', 'E', 'L', 'L', 'O'};
	twiMasterWrite(LCD_WRITE_ADDR, commands, 3);	
	twiMasterWrite(LCD_WRITE_ADDR, data, 5);
	// test memory write to DS1307
	uint8_t send_with_address[] = {0x08, 'h', 'e', 'l', 'l','o'};
	twiMasterWrite(RTC_WRITE_ADDR, send_with_address, 6);
	uint8_t receive_buffer[] = {0, 0, 0, 0, 0};
	twiMasterReadFromLoc(RTC_READ_ADDR, RTC_WRITE_ADDR, 0x08, receive_buffer, 5);
	twiMasterWrite(LCD_WRITE_ADDR, receive_buffer, 1);


	
    while(1)
    {
		// do nothing
	}		
    
}




ISR (INT7_vect){
	test_pattern();
}

inline void wait_for_push(){
	while(_BV(ENCODER_BUTTON)&ENCODER_PIN)
	; //block till button push
}

inline void init_rgb_leds(){
	RGB_DDR = _BV(G_ON) | _BV(R_ON) | _BV(B_ON);	
}

inline void test_pattern(){
		//RGB_PORT = 0x00;
		RGB_PORT |= _BV(R_ON);
		RGB_PORT &= ~_BV(B_ON);
		RGB_PORT &= ~_BV(G_ON);
		//RGB_PORT = 0xFF;
		_delay_ms(2);
		//RGB_PORT = 0x00;
		RGB_PORT &= ~_BV(R_ON);
		RGB_PORT |= _BV(B_ON);
		RGB_PORT &= ~_BV(G_ON);
		_delay_ms(2);
		//RGB_PORT = 0x00;
		RGB_PORT &= ~_BV(R_ON);
		RGB_PORT &= ~_BV(B_ON);
		RGB_PORT |= _BV(G_ON);
		_delay_ms(2);
		RGB_PORT = 0x00;
}

inline void init_optical_interrupt(){
	EICRB=_BV(ISC70)|_BV(ISC71); // pin 7 int rising edge
	EIMSK=_BV(INT7);
	sei();	
}