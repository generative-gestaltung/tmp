#ifndef DAC_H
#define DAC_H

#define	SPICS	RPI_GPIO_P1_24	//P4
#define CS_1() bcm2835_gpio_write(SPICS,HIGH)
#define CS_0() bcm2835_gpio_write(SPICS,LOW)


/* Unsigned integer types  */
#define uint8_t unsigned char
#define uint16_t unsigned short

#define CHANNEL_A   0x10
#define CHANNEL_B   0x12
#define CHANNEL_C   0x14
#define CHANNEL_D   0x16


void  bsp_DelayUS(uint64_t micros);
void DAC8564_Write(uint8_t channel, uint16_t Data);
void DAC8564_Init();
uint16_t Voltage_Convert(float Vref, float voltage);

#endif
