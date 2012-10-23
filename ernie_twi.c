/*
 * ernie_twi.c
 *
 * Created: 8/23/2012 11:53:19 PM
 *  Author: Ernie
 */ 
#include <util/twi.h>
#include <avr/interrupt.h>
#include "ernie_twi.h"
#include "ernie_utils.h"

uint8_t addr;
uint8_t myarray[10];
int len;
int index;

void sendAddr(uint8_t abyte);
void sendNextByte();
void sendStopBit();



twiWrite(uint8_t address, uint8_t bytes[], uint8_t size){
	addr = address;
	sei();
	//int length=sizeof(bytes)/sizeof(uint8_t);
	len=size;
	memcpy(myarray, bytes, size);
//	TWCR |= _BV(TWINT); //clear interrupt flag to start
	TWCR = _BV(TWINT)| _BV(TWSTA) | _BV(TWEN) | _BV(TWIE); //send start
//	TWCR |= _BV(TWINT); //clear interrupt flag to start


	
}

initTwi(){
		//TWBR=18; //100k
		//TWIBR=255 //77821.1
		TWBR=198; //10k
		TWCR|=_BV(TWEN);//TWI enable
		TWCR|=_BV(TWIE);//interrupt enable
}

ISR(TWI_vect){
	switch(TWSR & 0xF8){
		case(TW_START): // start condition transmitted
			LED1_ON;
			sendAddr(addr);
			TWCR &= ~_BV(TWSTA);
			TWCR |= _BV(TWINT);
			break;
		case(TW_MT_SLA_ACK): // SLA+W transmitted, ACK received
			LED2_ON;
			sendNextByte();
			TWCR |= _BV(TWINT);
			break;
		case(TW_MT_DATA_ACK): //data transmitted, ACK received
			LED3_ON;
			sendNextByte();
			TWCR |= _BV(TWINT);
			break;
	}
	
}

sendAddr(uint8_t abyte){
	TWDR = abyte;
	
}

sendNextByte(){
		if (index==len)
			sendStopBit();
		TWDR = myarray[index];
		index++;
}

sendStopBit(){
	TWCR = _BV(TWINT)| _BV(TWEN)| _BV(TWSTO);
	cli();
}

