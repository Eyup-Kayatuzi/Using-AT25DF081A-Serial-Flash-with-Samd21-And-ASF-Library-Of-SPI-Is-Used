/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * This is a bare minimum user application template.
 *
 * For documentation of the board, go \ref group_common_boards "here" for a link
 * to the board-specific documentation.
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# Minimal main function that starts with a call to system_init()
 * -# Basic usage of on-board LED and button
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */
#include <asf.h>
#include <string.h> // for memset
#include "SERCOMforUSART.h"
#include "SerialFlashFunctions.h"

#define BUF_LENGTH 20#define SLAVE_SELECT_PIN PIN_PA13 // ss pin

struct spi_module spi_master_instance;
struct spi_slave_inst slave; // for slave configuration

static uint8_t buffer[BUF_LENGTH] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 
										0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13 };
volatile bool transfer_complete_spi_master = false;
void configure_spi_master(void);
void configure_spi_master_callbacks(void);
static void callback_spi_master(const struct spi_module *const module);//////////////////deneme functionsuint8_t GelenData[256];
uint8_t i = 0; // for uart//////////////////////////////////
int main (void)
{
	system_init();
	delay_init();
	// the below codes is added for uart communication as register level
	PM->APBCMASK.reg |= PM_APBCMASK_SERCOM3;
	GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID_SERCOM3_CORE;
	PORT->Group[0].PINCFG[22].bit.PMUXEN = 1;
	PORT->Group[0].PMUX[11].bit.PMUXE = 0x2; // pa22 is config as sercom3 pad[0]
	PORT->Group[0].PINCFG[23].bit.PMUXEN = 1;
	PORT->Group[0].PMUX[11].bit.PMUXO = 0x2; // pa23 is config as sercom3 pad[1]
	////////////////////////////////////////////////////////////////////////////////////////////
	uartInit(115200);
	system_interrupt_enable_global();
	configure_spi_master();
	configure_spi_master_callbacks();
	/////////////// sil
	uint8_t string[] = "Hello World!\r\n"; // testing whether uart is working or not
	uartWrite(string, sizeof(string));
	delay_ms(50);

	globalUnprotected();
	blockErase(0x20, 0x00000000);
	readArray(0x1b, 0x00000000, 5, buffread);
	while (1) {
		
	}
}

void USART_EYUP(){
	GelenData[i] = SERCOM3->USART.DATA.bit.DATA;
	if (GelenData[i] == '\n'){
		pageProgram(0x00000000, GelenData, (i + 1)); // 0x0000fc
		readArray(0x1b, 0x00000000, (i + 1), buffread);
		memset(GelenData, 0x00, (i + 1) *sizeof(char));
		uartWrite(buffread, i + 1);
		memset(buffread, 0xff, (i + 1) *sizeof(char));
		i = 0;
		blockErase(0x20, 0x00000000);
	}
	else{
		i += 1;
	}
}



void configure_spi_master(void)
{
	struct spi_config config_spi_master;
	struct spi_slave_inst_config slave_dev_config;
	/* Configure and initialize software device instance of peripheral slave */
	spi_slave_inst_get_config_defaults(&slave_dev_config); // Addressing not enabled
	slave_dev_config.ss_pin = SLAVE_SELECT_PIN;
	spi_attach_slave(&slave, &slave_dev_config); // slave configuration is enabled....
	/* Configure, initialize and enable SERCOM SPI module */
	spi_get_config_defaults(&config_spi_master);
	config_spi_master.mux_setting = SPI_SIGNAL_MUX_SETTING_E;
	/* Configure pad 0 for data in */
	config_spi_master.pinmux_pad0 = PINMUX_PB16C_SERCOM5_PAD0; // pb16c as a miso for serial flash
	/* Configure pad 1 as unused */
	config_spi_master.pinmux_pad1 = PINMUX_UNUSED;
	/* Configure pad 2 for data out */
	config_spi_master.pinmux_pad2 = PINMUX_PB22D_SERCOM5_PAD2; // pb22 as a mosi
	/* Configure pad 3 for SCK */
	config_spi_master.pinmux_pad3 = PINMUX_PB23D_SERCOM5_PAD3;
	
	spi_init(&spi_master_instance, SERCOM5, &config_spi_master);
	spi_enable(&spi_master_instance);
	
}

void configure_spi_master_callbacks(void)
{
	spi_register_callback(&spi_master_instance, callback_spi_master, SPI_CALLBACK_BUFFER_RECEIVED); // SPI_CALLBACK_BUFFER_RECEIVED
	spi_enable_callback(&spi_master_instance, SPI_CALLBACK_BUFFER_RECEIVED);
}static void callback_spi_master(const struct spi_module *const module)
{
	
}