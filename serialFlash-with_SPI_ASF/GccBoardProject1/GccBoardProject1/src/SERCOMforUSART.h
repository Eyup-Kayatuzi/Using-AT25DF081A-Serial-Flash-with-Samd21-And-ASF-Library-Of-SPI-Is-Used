/*
 * SERCOMforUSART.h
 *
 * Created: 22.08.2022 10:46:40
 *  Author: ARGE
 */ 


#ifndef SERCOMFORUSART_H_
#define SERCOMFORUSART_H_

///////////////////////// Needed Library

#include <asf.h>
void uartInit(uint32_t fbaud);
void uartWrite(uint8_t *veri, uint8_t karakterAdedi);
void uartReadEnable(void);
void uartReadDisable(void);

#endif /* SERCOMFORUSART_H_ */