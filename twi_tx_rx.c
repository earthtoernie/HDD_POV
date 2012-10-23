/*
 * twi_tx_rx.c
 *
 * Created: 8/31/2012 1:12:13 PM
 *  Author: Ernie
 */ 
#define F_CPU 16000000UL
#include <util/twi.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "twi_tx_rx.h"
#include "led_utils.h"

#define SEND_START (_BV(TWINT) | _BV(TWSTA) | _BV(TWEN))
#define SEND_STOP  (_BV(TWINT)|_BV(TWEN)|_BV(TWSTO))
#define SEND_TWDR (_BV(TWINT) | _BV(TWEN))
#define RECV_TWDR_ACK (_BV(TWINT) | _BV(TWEN) | _BV(TWEA))
#define RECV_TWDR_NACK (_BV(TWINT) | _BV(TWEN))

#define TWI_SPEED_VAR 0x7D; //1kHz with ps of 3
#define WAIT_FOR_FLAG while (!(TWCR & (1<<TWINT))) //wait for twi bus operation to finish

#define SDA_VAL PIND1
#define SCL_VAL PIND0
#define SDA_SINK DDRD1
#define SCL_SINK DDRD0
#define TWI_PIN PIND
#define TWI_DDR DDRD

void twiMasterReadFromLoc(uint8_t sla_R, uint8_t sla_W, uint8_t loc, uint8_t buffer[], uint8_t length){
	TWCR = SEND_START; //send start
	WAIT_FOR_FLAG;
	if((TWSR & 0xF8)!=TW_START){
		LED1_ON;
		return;
	}
	TWDR=sla_W;// write addr
	TWCR=SEND_TWDR;// send addr
	WAIT_FOR_FLAG;
	if((TWSR& 0xF8)!=TW_MT_SLA_ACK){
		LED1_ON;
		return;
	}
	TWDR=loc;// write location
	TWCR=SEND_TWDR;// send location
	WAIT_FOR_FLAG;
	if((TWSR& 0xF8)!=TW_MT_DATA_ACK){
		LED1_ON;
		return;
	}	
	TWCR=SEND_START; //send repeated start
	WAIT_FOR_FLAG;
	if((TWSR& 0xF8)!=TW_REP_START){
		LED1_ON;
		return;
	}
	TWDR=sla_R;// tw read addr
	TWCR=SEND_TWDR;// send addr
	WAIT_FOR_FLAG;
	if((TWSR& 0xF8)!=TW_MR_SLA_ACK){
		LED1_ON;
		return;
	}
	
	for(int i = 0; i<(length-1); i++){ // for multi byte read, send ack
		TWCR=RECV_TWDR_ACK;
		WAIT_FOR_FLAG;
		if((TWSR& 0xF8)!=TW_MR_DATA_ACK){
			LED1_ON;
			return;
		}
		*(buffer+i)=TWDR;// read byte to buffer
	}
	
	TWCR=RECV_TWDR_NACK;
	WAIT_FOR_FLAG;
	if((TWSR& 0xF8)!=TW_MR_DATA_NACK){
		LED1_ON;
		return;
	}
	if(length==1)
		*(buffer)=TWDR; // if only reading a single bit
	else
		*(buffer+(length-1))=TWDR;
	TWCR = SEND_STOP;
	_delay_ms(1);	
	//WAIT_FOR_FLAG;
	
}

void twiMasterWrite(uint8_t sla_W, uint8_t data[], uint8_t length){
	TWCR = SEND_START; //send start
	WAIT_FOR_FLAG;
	if((TWSR& 0xF8)!=TW_START){
		LED1_ON;
		return;
	}
	TWDR=sla_W;// write addr
	TWCR=SEND_TWDR;// send addr
	WAIT_FOR_FLAG;
	if((TWSR& 0xF8)!=TW_MT_SLA_ACK){
		LED1_ON;
		return;
	}
	for(int i = 0; i<length; i++){
		TWDR=*(data+i);// write byte
		TWCR=SEND_TWDR;
		WAIT_FOR_FLAG;
	}
	TWCR =SEND_STOP;
	_delay_ms(1);		
}

void twiInit(){
	TWBR=TWI_SPEED_VAR;
	TWSR=_BV(TWPS0)|_BV(TWPS1);
	TWCR|=_BV(TWEN);
}

unjam_twi(){ // not uesd , just for testing
	uint8_t sda_stuck_low = 0;
	if (!(_BV(SDA_VAL)&TWI_PIN)) // see if SDA is stuck low
	sda_stuck_low = 1;
	while(sda_stuck_low){
		LED4_ON;
		_delay_ms(1);
		TWI_DDR|= _BV(SCL_SINK); // low clk because set to output and sink current
		_delay_ms(1);
		TWI_DDR&=~ _BV(SCL_SINK); // high clk because set to input and highZ
		_delay_ms(1);
		TWI_DDR|= _BV(SCL_SINK); // low clk because set to output and sink current
		_delay_ms(1);
		if(_BV(SDA_VAL)&TWI_PIN) // see if we got SDA to release
		sda_stuck_low=0;
	}
	TWI_DDR|=_BV(SDA_SINK); // pull data line low
	_delay_ms(1);
	TWI_DDR&=~_BV(SCL_SINK); // high clk because set to input and highZ
	_delay_ms(1);
	TWI_DDR&=~_BV(SDA_SINK); // release data line
	_delay_ms(1);
	
}

