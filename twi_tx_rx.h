/*
 * twi_tx_rx.h
 *
 * Created: 9/7/2012 12:56:41 PM
 *  Author: Ernie
 */ 


#ifndef TWI_TX_RX_H_
#define TWI_TX_RX_H_

void twiMasterReadFromLoc(uint8_t sla_R, uint8_t sla_W, uint8_t loc, uint8_t buffer[], uint8_t length);
void twiMasterWrite(uint8_t sla_W, uint8_t data[], uint8_t length);
void twiInit();
void unjam_twi();


#endif /* TWI_TX_RX_H_ */