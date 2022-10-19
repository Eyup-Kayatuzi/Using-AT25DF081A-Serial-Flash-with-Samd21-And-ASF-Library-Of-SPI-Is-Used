/*
 * SerialFlashFunctions.c
 *
 * Created: 23.08.2022 09:49:41
 *  Author: ARGE
 */ 
#include "SerialFlashFunctions.h"

volatile uint8_t valueofstatusreg[2];

uint8_t buffread[256]; // read from sdcard
uint8_t buffwrite[256];// write to sdcard, this not used now but next project we mighth need it
	
	
void pageProgram(uint32_t startedaddress, uint8_t *data, uint8_t howmuchdata){
	uint16_t opcode = 0x02;
	writeEnable();
	readStatusRegister(&valueofstatusreg);
	while (!(0x02 == (0x03 & valueofstatusreg[0]))){
		readStatusRegister(&valueofstatusreg);
	} // well bit is 1 and device is ready
	volatile uint8_t max = 0xff, fark = 0x00;
	volatile uint16_t kalanplusone = 0x00;
	volatile uint8_t byte1, byte2, byte3;
	byte3 = startedaddress;
	byte2 = (startedaddress >> 8);
	byte1 = (startedaddress >> 16);
	kalanplusone = (max - byte3) + 1; // + suanki deger
	spi_select_slave(&spi_master_instance, &slave, true);	while ((spi_write_buffer_wait(&spi_master_instance, &opcode, 1) != STATUS_OK)){};
	while ((spi_write_buffer_wait(&spi_master_instance, &byte1, 1) != STATUS_OK)){};
	while ((spi_write_buffer_wait(&spi_master_instance, &byte2, 1) != STATUS_OK)){};
	while ((spi_write_buffer_wait(&spi_master_instance, &byte3, 1) != STATUS_OK)){};
	if (kalanplusone >= howmuchdata){
		
		for (volatile uint8_t a = 0; a < howmuchdata; a++){
			while ((spi_write_buffer_wait(&spi_master_instance, (data + a), 1) != STATUS_OK)){};
		}
		
		spi_select_slave(&spi_master_instance, &slave, false);
		readStatusRegister(&valueofstatusreg);
		while ((0x01 == (0x01 & valueofstatusreg[0]))){ // wait until writing is completed
			readStatusRegister(&valueofstatusreg);
		}
	}
	else{
		
		for (volatile uint8_t b = 0; b < kalanplusone; b++){
			while ((spi_write_buffer_wait(&spi_master_instance, (data + b), 1) != STATUS_OK)){};
		}
		
		spi_select_slave(&spi_master_instance, &slave, false);
		readStatusRegister(&valueofstatusreg);
		while ((0x01 == (0x01 & valueofstatusreg[0]))){ // wait until writing is completed
			readStatusRegister(&valueofstatusreg);
		}
		writeEnable();
		readStatusRegister(&valueofstatusreg);
		while (!(0x02 == (0x03 & valueofstatusreg[0]))){
			readStatusRegister(&valueofstatusreg);
		} // well bit is 1 and device is ready
		startedaddress += (kalanplusone);
		byte3 = startedaddress;
		byte2 = (startedaddress >> 8);
		byte1 = (startedaddress >> 16);
		spi_select_slave(&spi_master_instance, &slave, true);
		while ((spi_write_buffer_wait(&spi_master_instance, &opcode, 1) != STATUS_OK)){};
		while ((spi_write_buffer_wait(&spi_master_instance, &byte1, 1) != STATUS_OK)){};
		while ((spi_write_buffer_wait(&spi_master_instance, &byte2, 1) != STATUS_OK)){};
		while ((spi_write_buffer_wait(&spi_master_instance, &byte3, 1) != STATUS_OK)){};
		fark = howmuchdata - kalanplusone;
		
		for (volatile uint8_t c = 0; c < fark; c++){
			while ((spi_write_buffer_wait(&spi_master_instance, (data + (c + kalanplusone)), 1) != STATUS_OK)){};
		}
		
		spi_select_slave(&spi_master_instance, &slave, false);
		readStatusRegister(&valueofstatusreg);
		while ((0x01 == (0x01 & valueofstatusreg[0]))){ // wait until writing is completed
			readStatusRegister(&valueofstatusreg);
		}
	}
}

void globalUnprotected(){	uint16_t opcode[2] = {0x01, 0x00};	writeEnable();	readStatusRegister(&valueofstatusreg);	while (!(0x02 == (0x03 & valueofstatusreg[0]))){
		readStatusRegister(&valueofstatusreg);
	} // well bit is 1 and device is ready	spi_select_slave(&spi_master_instance, &slave, true);	while ((spi_is_ready_to_write(&spi_master_instance) != true)){};	while ((spi_write_buffer_wait(&spi_master_instance, opcode, 2) != STATUS_OK)){};	spi_select_slave(&spi_master_instance, &slave, false);}void writeEnable(){	uint16_t opcode[1] = {0x06};	spi_select_slave(&spi_master_instance, &slave, true);	while ((spi_is_ready_to_write(&spi_master_instance) != true)){};	while ((spi_write_buffer_wait(&spi_master_instance, opcode, 1) != STATUS_OK)){};	spi_select_slave(&spi_master_instance, &slave, false);}void readStatusRegister(uint8_t *data){	spi_select_slave(&spi_master_instance, &slave, true);	uint16_t opcode[1] = {0x05};	spi_write_buffer_wait(&spi_master_instance, opcode, 1);	spi_read_buffer_wait(&spi_master_instance, data, 2, 0xff);	spi_select_slave(&spi_master_instance, &slave, false);}void blockErase(uint8_t opcodeoferase, uint32_t adress){	writeEnable();	readStatusRegister(&valueofstatusreg);	while (!(0x02 == (0x03 & valueofstatusreg[0]))){
		readStatusRegister(&valueofstatusreg);
	} // well bit is 1 and device is ready	volatile uint8_t byte1, byte2, byte3;
	byte3 = adress;
	byte2 = (adress >> 8);
	byte1 = (adress >> 16);	spi_select_slave(&spi_master_instance, &slave, true);	spi_write_buffer_wait(&spi_master_instance, &opcodeoferase, 1);	spi_write_buffer_wait(&spi_master_instance, &byte1, 1);	spi_write_buffer_wait(&spi_master_instance, &byte2, 1);	spi_write_buffer_wait(&spi_master_instance, &byte3, 1);	spi_select_slave(&spi_master_instance, &slave, false);		readStatusRegister(&valueofstatusreg);
	while ((0x20 == (0x20 & valueofstatusreg[0]))){
		readStatusRegister(&valueofstatusreg);
	}		readStatusRegister(&valueofstatusreg);
	/*while ((0x02 == (0x02 & valueofstatusreg))){
		readStatusRegister(1, &valueofstatusreg);
	}*/
	while ((0x01 == (0x01 & valueofstatusreg[0]))){ // wait until blok erasing is completed
		readStatusRegister(&valueofstatusreg);
	}	}void readArray(uint8_t opcodeofread, uint32_t adress, uint8_t howmuchbyte, uint8_t *vallue){	volatile uint8_t byte1, byte2, byte3;
	byte3 = adress;
	byte2 = (adress >> 8);
	byte1 = (adress >> 16);	uint16_t opcode[3] = {0x1b, 0x0b, 0x03};	volatile uint8_t dummy = 0xff;	if(opcodeofread == 0x1b){		spi_select_slave(&spi_master_instance, &slave, true);		spi_write_buffer_wait(&spi_master_instance, &(opcode[0]), 1);		spi_write_buffer_wait(&spi_master_instance, &byte1, 1);		spi_write_buffer_wait(&spi_master_instance, &byte2, 1);		spi_write_buffer_wait(&spi_master_instance, &byte3, 1);		spi_write_buffer_wait(&spi_master_instance, &dummy, 1);		spi_write_buffer_wait(&spi_master_instance, &dummy, 1); // 2 byte dummy	}	else if(opcodeofread == 0x0b){
		spi_select_slave(&spi_master_instance, &slave, true);
		spi_write_buffer_wait(&spi_master_instance, &(opcode[1]), 1);
		spi_write_buffer_wait(&spi_master_instance, &byte1, 1);		spi_write_buffer_wait(&spi_master_instance, &byte2, 1);		spi_write_buffer_wait(&spi_master_instance, &byte3, 1);		spi_write_buffer_wait(&spi_master_instance, &dummy, 1); // 1 byte dummy
	}	else if(opcodeofread == 0x03){		spi_select_slave(&spi_master_instance, &slave, true);
		spi_write_buffer_wait(&spi_master_instance, &(opcode[2]), 1);
		spi_write_buffer_wait(&spi_master_instance, &byte1, 1);		spi_write_buffer_wait(&spi_master_instance, &byte2, 1);		spi_write_buffer_wait(&spi_master_instance, &byte3, 1);	}	for (uint8_t a = 0; a < howmuchbyte; a++)
	{
		spi_read_buffer_wait(&spi_master_instance, (vallue + a), 1, 0xff);
	}	spi_select_slave(&spi_master_instance, &slave, false);}