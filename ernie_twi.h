/*
 * ernie_twi.h
 *
 * Created: 8/23/2012 11:53:47 PM
 *  Author: Ernie
 */ 


#ifndef ERNIE_TWI_H_
#define ERNIE_TWI_H_

void twiWrite(uint8_t address, uint8_t bytes[], uint8_t length);
void initTwi(void);




#endif /* ERNIE_TWI_H_ */