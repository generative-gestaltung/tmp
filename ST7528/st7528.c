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
//-----------------------------------------------------------
//#define bus 0//P1
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



int port0;

#define HIGH 1
#define LOW 0


uint16_t port_value = 0x0000;

void setPin (int pin, int v) {
	if (v==0)
		port_value &= ~(v<<pin);
	if (v==1)
		port_value |= (v<<pin);

	wiringPiI2CWriteReg16 (port0, 0, port_value);
}

void setPort (uint8_t v) {
	port_value &= 0x00ff;
	port_value |= (v<<8);
	wiringPiI2CWriteReg16 (port0, 0, port_value);
}


//------------------------------------------------------------

//-------------------------------------------------------------
void write_command(unsigned char datum)
{
 setPin (A0, 0); //A0=0;									/*Instruction register*/
 setPin (E, 1);//E=1;									  /*Read inactive*/
 setPort (datum); //bus=datum;							/*put data on port 1*/
 setPin (CSB, 0); //CSB=0;									/*Chip select active*/
 setPin (RW, 0); //RW=0;									/*Write active*/
 setPin (RW, 1); //RW=1;									/*Write inactive; latch in data*/
 setPin (CSB, 1); //CSB=1;									/*Chip select inactive*/
}
void write_data(unsigned char datum)
{
 setPin (A0, 1); //A0=1;									/*DDRAM data register*/
 setPin (E, 1); //E=1;
 setPort (datum); //bus=datum;
 setPin (CSB, 0); //CSB=0;
 setPin (RW, 0); //RW=0;
 setPin (RW, 1); //RW=1;
 setPin (CSB, 1); //CSB=1;
}
void show_display(unsigned char *lcd_string)
{
 unsigned char page;
 unsigned char col;
 unsigned int c=0;
 for (page=0xB0;page<0xC0;page++)		/*write to page 0 then go to mext page .*/
 {										/*     128pixels / 8per page = 16 pages    */
  write_command(page);					/*Set page address*/
  write_command(0x10);					/*Set column address MSB*/
  write_command(0x00);					/*Set column address LSB*/
  for(col=0;col<128;col++)				/*each page has 128 pixel columns*/
  {
   write_data(*lcd_string);				/*16 level grayscale; write each byte 4 times*/
   write_data(*lcd_string);
   write_data(*lcd_string);
   write_data(*lcd_string++);			/*increment to next byte of data*/
  }
 }
}



void main(){


	wiringPiSetup();
	port0 = wiringPiI2CSetup (0x3e);


	printf ("start %d\n", port0);
	setPin (RST, 1); //RST=1;
	delay(1);
	setPin (RST, 0); //RST=0;								/*Reset lcd controller*/
	delay(1);
	setPin (RST, 1); //RST=1;
	delay(1);
	vopcode=45;							/*Electronic volumn setting*/
	Ra_Rb=0x27;							/*Internal resistance ratio*/

	printf("loop\n");
	for(;;){
		lcd_init();						/*Initialize the LCD controller*/
		show_display(small_text);		/*Show 128x128 pictures*/
		delay(500);
		show_display(big_text);
		delay(500);
		show_display(picture);
		delay(500);
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
}
