    while(1) {

        if(tmp==0) {
     			Write_DAC8532(0x30, Voltage_Convert(5.0,0.00+(float)i/10));    	//Write channel A buffer (0x30)
    			Write_DAC8532(0x34, Voltage_Convert(5.0,5.000-(float)i/10));    	//Write channel B buffer (0x34)		
    			i++;

			if(i==50) {
				i=0;
				 tmp=1;
			bsp_DelayUS(500);
     	}

        else if(tmp==1) {
			Write_DAC8532(0x30, Voltage_Convert(5.0,5.000-(float)i/10));    	//Write channel B buffer (0x30)	
			Write_DAC8532(0x34, Voltage_Convert(5.0,0.00+(float)i/10));    	//Write channel A buffer (0x34)

    			i++;
			if(i==50) {
			    i=0;
			    tmp=0;
			}
			bsp_DelayUS(500);
        }
    }

    bcm2835_spi_end();
    bcm2835_close();

    return 0;
}
