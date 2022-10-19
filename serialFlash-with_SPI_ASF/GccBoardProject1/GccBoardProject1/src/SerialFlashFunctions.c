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
	spi_select_slave(&spi_master_instance, &slave, true);
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

void globalUnprotected(){
		readStatusRegister(&valueofstatusreg);
	} // well bit is 1 and device is ready
		readStatusRegister(&valueofstatusreg);
	} // well bit is 1 and device is ready
	byte3 = adress;
	byte2 = (adress >> 8);
	byte1 = (adress >> 16);
	while ((0x20 == (0x20 & valueofstatusreg[0]))){
		readStatusRegister(&valueofstatusreg);
	}
	/*while ((0x02 == (0x02 & valueofstatusreg))){
		readStatusRegister(1, &valueofstatusreg);
	}*/
	while ((0x01 == (0x01 & valueofstatusreg[0]))){ // wait until blok erasing is completed
		readStatusRegister(&valueofstatusreg);
	}
	byte3 = adress;
	byte2 = (adress >> 8);
	byte1 = (adress >> 16);
		spi_select_slave(&spi_master_instance, &slave, true);
		spi_write_buffer_wait(&spi_master_instance, &(opcode[1]), 1);
		spi_write_buffer_wait(&spi_master_instance, &byte1, 1);
	}
		spi_write_buffer_wait(&spi_master_instance, &(opcode[2]), 1);
		spi_write_buffer_wait(&spi_master_instance, &byte1, 1);
	{
		spi_read_buffer_wait(&spi_master_instance, (vallue + a), 1, 0xff);
	}