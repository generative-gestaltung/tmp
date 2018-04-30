//---------------------------------------------------------
/*
NHD-C128128BZ.c
Program for writing to Newhaven Display graphic LCD.

(c)2008 Curt Lagerstam - Newhaven Display International, LLC.

 	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
*/
//-----------------------------------------------------------
//#include <at89x51.h>
#include "picture.h"
#include <inttypes.h>
#include <stdlib.h>

//-----------------------------------------------------------
//#define bus 0//P1
#define PS0 0
#define PS1 1
#define PS2 2
#define CSB 3//P3_5
#define RST 4//P3_4
#define A0 5//P3_2
#define RW 6// P3_1
#define E 7//P3_0
//-----------------------------------------------------------
void lcd_init(void);
void show_display(unsigned char *lcd_string);
void write_command(unsigned char datum);
void write_data(unsigned char datum);

unsigned char  vopcode;
unsigned char  Ra_Rb;


#define NULL 0
#define I2C_SMBUS 0x0720
#define I2C_SMBUS_READ 1
#define I2C_SMBUS_WRITE 0
#define I2C_SMBUS_BYTE 1
#define I2C_SMBUS_BYTE_DATA 2
#define I2C_SMBUS_WORD_DATA 3



#define LOOP 0
#define STATIC_PINS 0
#define USE_PCF8575 0

#define I2C_ADDR0 0x38
#define I2C_ADDR1 0x39
#define I2C_ADDR2 0x20


/*
union i2c_smbus_data {
	uint8_t data;
	uint16_t word;
	uint8_t block[32];
};
*/
struct i2c_smbus_ioctl_data {
	char read_write;
	uint8_t command;

	int size;
	union i2c_smbus_data* data;
};



int i2c_smbus_access (int fd, char rw, uint8_t cmd, int size, union i2c_smbus_data* data) {

	struct i2c_smbus_ioctl_data args;
	args.read_write = rw;
	args.command    = cmd;
	args.size       = size;
	args.data       = data;

	return ioctl (fd, I2C_SMBUS, &args);

}


int ports[3];
uint8_t data[2];

uint16_t port_value = 0x0000;


void setPin (int pin, int v) {
	if (v==0)
		data[0] &= ~(v<<pin);
	if (v==1)
		data[0] |= (v<<pin);

	//wiringPiI2CWriteReg16 (port0, 0, port_value);
	//wiringPiI2CWrite (port0, port_value & 0xff);
}

void setPort (uint8_t v) {
	data[1] = v;
	//wiringPiI2CWriteReg16 (port0, 0, port_value);
	//wiringPiI2CWrite (port1, port_value >> 8);

}

void update (int p) {
	//printf("%d\n", data[p]);
	//wiringPiI2CWrite (ports[p], data[p]);
	//printf("%d\n", data[0]);

#if USE_PCF8575
	//i2c_smbus_access (ports[0], I2C_SMBUS_WRITE, data, I2C_SMBUS_BYTE, NULL);
	wiringPiI2CWriteReg8 (ports[2], data[0], data[1]);

#else
	i2c_smbus_access (ports[p], I2C_SMBUS_WRITE, data[p], I2C_SMBUS_BYTE, NULL);
#endif
}

//------------------------------------------------------------

//-------------------------------------------------------------
void write_command(unsigned char datum)
{
 setPin (A0, 0); //A0=0;									/*Instruction register*/
 update(0);
 setPin (E, 1);//E=1;									  /*Read inactive*/
 update(0);
 setPort (datum); //bus=datum;							/*put data on port 1*/
 update(1);
 setPin (CSB, 0); //CSB=0;									/*Chip select active*/
 setPin (RW, 0); //RW=0;									/*Write active*/
 update(0);
 setPin (RW, 1); //RW=1;									/*Write inactive; latch in data*/
 setPin (CSB, 1); //CSB=1;									/*Chip select inactive*/
 //printf("cmd\n");
 update(0);
}

void write_data(unsigned char datum)
{
 setPin (A0, 1); //A0=1;
 update(0);									/*DDRAM data register*/
 setPin (E, 1); //E=1;
 update(0);
 setPort (datum); //bus=datum;
 update(1);
 setPin (CSB, 0); //CSB=0;
 setPin (RW, 0); //RW=0;
 update(0);
 setPin (RW, 1); //RW=1;
 setPin (CSB, 1); //CSB=1;
 update(0);
 //printf("dat\n");
}


void show_display(unsigned char *lcd_string)
{
 unsigned char page;
 unsigned char col;
 unsigned int c=0;

 uint8_t dat = 0;

 //for (page=0xB0;page<0xBF;page++)		/*write to page 0 then go to mext page .*/
 //{										/*     128pixels / 8per page = 16 pages    */
 // write_command(page);					/*Set page address*/

int cnt = 0;
for (col=0; col<1; col++) {

  //write_command (0x10);// | (col>>4) );					/*Set column address MSB*/
  //write_command (0x00);// | (col&0x0f) );					/*Set column address LSB*/

  //for(col=0;col<32;col++)				/*each page has 128 pixel columns*/ 
  //{
   //write_data(*lcd_string);				/*16 level grayscale; write each byte 4 times*/
   //write_data(*lcd_string);
   //write_data(*lcd_string);
   dat = *lcd_string++;
   dat = 3;
   //if (col>2) dat = 0;
   //else dat = 1;

   write_data (dat);
   //write_data (dat);
   //write_data (dat);
   //write_data (dat);
   cnt++;

  }
// }

printf("wrote %d cols\n", cnt);
}



void main(){


	wiringPiSetup();


#if USE_PCF8575
	ports[2] = wiringPiI2CSetup (I2C_ADDR2);
#else
	ports[0] = wiringPiI2CSetup (I2C_ADDR0);
	ports[1] = wiringPiI2CSetup (I2C_ADDR1);
#endif


	setPin (PS0, 1);
	update(0);
	delay(1);

#if STATIC_PINS

	setPin (0, 0);
	setPin (1, 0);
	setPin (2, 0);
	setPin (3, 0);
	setPin (4, 0);
	setPin (5, 0);
	setPin (6, 0);
	setPin (7, 0);

	setPort (0x55);
	update(0);


	printf("%d %d\n", data[0], data[1]);

	return 0;
#endif


	setPin (RST, 1); //RST=1;
	delay(1);
	setPin (RST, 0); //RST=0;								/*Reset lcd controller*/
	delay(1);
	setPin (RST, 1); //RST=1;
	delay(1);
	vopcode=45;							/*Electronic volumn setting*/
	Ra_Rb=0x27;							/*Internal resistance ratio*/

	printf("loop\n");
	lcd_init();						/*Initialize the LCD controller*/
	show_display(big_text);

	while (LOOP){
		//lcd_init();						/*Initialize the LCD controller*/
		//show_display(small_text);		/*Show 128x128 pictures*/
		//delay(50);
		show_display(picture);
		delay(2000);
		//show_display(picture);
		//delay(50);
		printf(".\n");
	}
}
void lcd_init(void){
        write_command(0xA2);        //ICON  OFF;
        write_command(0xAE);        //Display OFF

        write_command(0x48);        //Set Duty ratio
        write_command(0x80);        //No operation
        write_command(0xa0);        //Set scan direction
        write_command(0xc8);        //SHL  select
        write_command(0x40);        //Set START LINE
        write_command(0x00);
        write_command(0xab);        //OSC on

        write_command(0x64);        //3x
        delay(2);
        write_command(0x65);        //4x
        delay(2);
        write_command(0x66);        //5x
        delay(2);
        write_command(0x67);        //6x
        delay(2);

        write_command(Ra_Rb);       //RESISTER SET
        write_command(0x81);        //Set electronic volume register
        write_command(vopcode);     //n=0~3f


        write_command(0x57);        //1/12bias
        write_command(0x92);        //FRC and pwm


        write_command(0x2C);
        delay(200);//200ms
        write_command(0x2E);
        delay(200);//200ms
        write_command(0x2F);
        delay(200);//200ms


        write_command(0x92);        //frc and pwm
        write_command(0x38);        //external  mode
        write_command(0x75);
	//start settings for 16-level grayscale

        write_command(0x80);
        write_command(0x00);
        write_command(0x81);
        write_command(0x00);
        write_command(0x82);
        write_command(0x00);
        write_command(0x83);
        write_command(0x00);

        write_command(0x84);
        write_command(0x06);
        write_command(0x85);
        write_command(0x06);
        write_command(0x86);
        write_command(0x06);
        write_command(0x87);
        write_command(0x06);

        write_command(0x88);
        write_command(0x0b);
        write_command(0x89);
        write_command(0x0b);
        write_command(0x8a);
        write_command(0x0b);
        write_command(0x8b);
        write_command(0x0b);

        write_command(0x8c);
        write_command(0x10);
        write_command(0x8d);
        write_command(0x10);
        write_command(0x8e);
        write_command(0x10);
        write_command(0x8f);
        write_command(0x10);

        write_command(0x90);
        write_command(0x15);
        write_command(0x91);
        write_command(0x15);
        write_command(0x92);
        write_command(0x15);
        write_command(0x93);
        write_command(0x15);

        write_command(0x94);
        write_command(0x1a);
        write_command(0x95);
        write_command(0x1a);
        write_command(0x96);
        write_command(0x1a);
        write_command(0x97);
        write_command(0x1a);

        write_command(0x98);
        write_command(0x1e);
        write_command(0x99);
        write_command(0x1e);
        write_command(0x9a);
        write_command(0x1e);
        write_command(0x9b);
        write_command(0x1e);

        write_command(0x9c);
        write_command(0x23);
        write_command(0x9d);
        write_command(0x23);
        write_command(0x9e);
        write_command(0x23);
        write_command(0x9f);
        write_command(0x23);

        write_command(0xa0);
        write_command(0x27);
        write_command(0xa1);
        write_command(0x27);
        write_command(0xa2);
        write_command(0x27);
        write_command(0xa3);
        write_command(0x27);

        write_command(0xa4);
        write_command(0x2b);
        write_command(0xa5);
        write_command(0x2b);
        write_command(0xa6);
        write_command(0x2b);
        write_command(0xa7);
        write_command(0x2b);

        write_command(0xa8);
        write_command(0x2f);
        write_command(0xa9);
        write_command(0x2f);
        write_command(0xaa);
        write_command(0x2f);
        write_command(0xab);
        write_command(0x2f);

        write_command(0xac);
        write_command(0x32);
        write_command(0xad);
        write_command(0x32);
        write_command(0xae);
        write_command(0x32);
        write_command(0xaf);
        write_command(0x32);

        write_command(0xb0);
        write_command(0x35);
        write_command(0xb1);
        write_command(0x35);
        write_command(0xb2);
        write_command(0x35);
        write_command(0xb3);
        write_command(0x35);

        write_command(0xb4);
        write_command(0x38);
        write_command(0xb5);
        write_command(0x38);
        write_command(0xb6);
        write_command(0x38);
        write_command(0xb7);
        write_command(0x38);

        write_command(0xb8);
        write_command(0x3a);
        write_command(0xb9);
        write_command(0x3a);
        write_command(0xba);
        write_command(0x3a);
        write_command(0xbb);
        write_command(0x3a);

        write_command(0xbc);
        write_command(0x3c);
        write_command(0xbd);
        write_command(0x3c);
        write_command(0xbe);
        write_command(0x3c);
        write_command(0xbf);
        write_command(0x3c);
	//end settings for 16-level grayscale
        write_command(0x38);
        write_command(0x74);
        write_command(0xaf);     //Display ON

	printf("init_done\n");
}
