/*
 * usart.h
 *
 *  Created on: 12 Nov 2017
 *      Author: btoland
 */

#ifndef USART_H_
#define USART_H_

#include <stdint.h>

void usartInit(void);
void usartEnableRxInt(void);

void usartSendChar(char c);
void usartSendString(char []);

uint8_t usartCharReceived(void);
char usartReadChar(void);

#endif /* USART_H_ */
