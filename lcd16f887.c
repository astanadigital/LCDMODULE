
#define bitset(var,bitno) ((var) |= (1 << (bitno)))
#define bitclr(var,bitno) ((var) &= ~(1 << (bitno)))
#define bittst(var,bitno) (var & (1 << (bitno)))


#define LCDPort		PORTD
#define LCDTris		TRISD
#define Epin		6
#define RWpin		5
#define RSpin		4
#define BUSYpin		3
#define DATA0pin	0
#define DATA1pin	1
#define DATA2pin	2
#define DATA3pin	3

#define EHIGH          bitset(LCDPort, Epin)
#define ELOW           bitclr(LCDPort, Epin)
#define E_OUTPUT       bitclr(LCDTris, Epin)
#define RSHIGH         bitset(LCDPort, RSpin)
#define RSLOW          bitclr(LCDPort, RSpin)
#define RS_OUTPUT      bitclr(LCDTris, RSpin)
#define RWHIGH         bitset(LCDPort, RWpin)
#define RWLOW          bitclr(LCDPort, RWpin)
#define RW_OUTPUT      bitclr(LCDTris, RWpin)
#define BUSY_FLAG      bittst(LCDPort, BUSYpin)
#define DATA_DIR_RD    {bitset(LCDTris,DATA3pin);bitset(LCDTris,DATA2pin);bitset(LCDTris,DATA1pin);bitset(LCDTris,DATA0pin);}
#define DATA_DIR_WR    {bitclr(LCDTris,DATA3pin);bitclr(LCDTris,DATA2pin);bitclr(LCDTris,DATA1pin);bitclr(LCDTris,DATA0pin);}
#define OUTPUT_DATA(x) {int aval=LCDPort&0xF0; x = x & 0x0F; LCDPort=aval+x;}

	
void epulse(void){
	__delay_us(1);
	EHIGH;
	__delay_us(1); 
	ELOW; 
	__delay_us(1);
}

void lcd_write(
     unsigned char cmd, unsigned char data_flag, unsigned char chk_busy, unsigned char dflag){
	char bflag,c;
	if (chk_busy) {
		RSLOW;       //RS = 0 to check busy
		// check busy
		DATA_DIR_RD;  //set data pins all inputs
		RWHIGH;       // R/W = 1, for read
		do {
			EHIGH; 
			__delay_us(1);  // upper 4 bits
			bflag = BUSY_FLAG;  
			ELOW; 
			__delay_us(1);
			epulse();
		} while(bflag);
	} else {
		__delay_ms(10); // don't use busy, just delay
	}
	DATA_DIR_WR;
	if (data_flag)  RSHIGH;   // RS=1, data byte
	else    RSLOW;   // RS=0, command byte
	// device is not busy
	RWLOW;       // R/W = 0, for write
	c = cmd >> 4;  // send upper 4 bits
	OUTPUT_DATA(c);
	epulse();
	if (dflag) {
		c = cmd & 0x0F;  //send lower 4 bits
		OUTPUT_DATA(c);
		epulse();
	}
}
    

void lcd_init(void) {
	// configure, see control pins as outputs
	// initialize as low
	E_OUTPUT;  RS_OUTPUT;  RW_OUTPUT;
	ELOW; RSLOW;  RWLOW; 
	
	__delay_ms(25);  //wait for device to settle
	__delay_ms(25);
	lcd_write(0x20,0,0,0);  // 4 bit interface
	lcd_write(0x28,0,0,1);  // 2 line display, 5x7 font
	lcd_write(0x28,0,0,1);  // repeat
	lcd_write(0x06,0,0,1);  // enable display
	lcd_write(0x0C,0,0,1);  // turn display on; cursor, blink is off
	lcd_write(0x01,0,0,1);  // clear display, move cursor to home
	__delay_ms(3);   // wait for busy flag to be ready
}

// send 8 bit char to LCD
void putch(char c) {
    lcd_write(c,1,1,1);
}

void lcd_clear(void){
	lcd_write(0x01,0,0,1);  // clear display, move cursor to home
}

void lcd_2ndline(void){
	lcd_write(0xC0,0,1,1);  // cursor to 2nd line
}

void lcd_shiftleft(void){
	lcd_write(0x18,0,1,1);  // shift left
}
 
void lcd_shiftright(void){
	lcd_write(0x1C,0,1,1);  // shift right
}   

void lcd_blinkcursor(void){
	lcd_write(0x0F,0,1,1);  // displays and blink a cursor
}

void lcd_printbar(int bar){
	for(int i=0; i<bar; i++){
		lcd_write(0xFF,1,1,1);	// print bars on the LCD
	}
}

void lcd_printstring(const char *s)			//send a string to display in the lcd
{          
	unsigned char i=0;
  	while (s && *s)lcd_write(*s++,1,1,1);

}