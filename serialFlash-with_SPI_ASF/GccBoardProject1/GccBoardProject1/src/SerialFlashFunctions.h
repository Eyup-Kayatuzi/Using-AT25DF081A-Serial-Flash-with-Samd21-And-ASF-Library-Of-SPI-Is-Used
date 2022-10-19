/*
 * SerialFlashFunctions.h
 *
 * Created: 23.08.2022 09:49:31
 *  Author: ARGE
 */ 
#include <asf.h>

extern struct spi_module spi_master_instance;
extern struct spi_slave_inst slave; // for slave configuration

extern volatile uint8_t valueofstatusreg[2];
extern uint8_t GelenData[256];
extern uint8_t buffread[256]; // read from sdcard
extern uint8_t buffwrite[256];// write to sdcard, this not used now but next project we mighth need it
	
void writeEnable();
void readStatusRegister(uint8_t *data);void globalUnprotected();void blockErase(uint8_t opcodeoferase, uint32_t adress);void readArray(uint8_t opcodeofread, uint32_t adress, uint8_t howmuchbyte, uint8_t *vallue);void pageProgram(uint32_t startedaddress, uint8_t *data, uint8_t howmuchdata);