/*
 * misc_utils.h
 *
 * Created: 8/28/2012 5:09:08 PM
 *  Author: Ernie
 */ 


#ifndef LED_UTILS_H_
#define LED_UTILS_H_



#define LED1 PF0
#define LED2 PF1
#define LED3 PF2
#define LED4 PF3
#define DEBUG_LED_PORT PORTF
#define DEBUG_LED_DDR DDRF

#define ENABLE_DEBUG_LEDS  (DEBUG_LED_DDR |= _BV(LED1) | _BV(LED2) | _BV(LED3) | _BV(LED4))
#define LED1_ON (DEBUG_LED_PORT |= _BV(LED1))
#define LED2_ON (DEBUG_LED_PORT |= _BV(LED2))
#define LED3_ON (DEBUG_LED_PORT |= _BV(LED3))
#define LED4_ON (DEBUG_LED_PORT |= _BV(LED4))
#define LED1_OFF (DEBUG_LED_PORT &= !_BV(LED1))
#define LED2_OFF (DEBUG_LED_PORT &= !_BV(LED2))
#define LED3_OFF (DEBUG_LED_PORT &= !_BV(LED3))
#define LED4_OFF (DEBUG_LED_PORT &= !_BV(LED4))

#endif /* MISC_UTILS_H_ */