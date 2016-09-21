// This code demonstrates microcontroller and LCD interfacing techniques 

#include <htc.h>          				// header file
#include <stdio.h> 
#define _XTAL_FREQ 	20000000		// define clock frequency
#include "lcd16f887.c"

__CONFIG(0x2CF2); 				// pic16f887 configuration bits

void main(void)   		// the main function where the program will start operating
{				// start of main: enter your code here
 

 // TRISD for LCD interface is set in lcd16f877.c 
       
lcd_init();				 	//  initialize LCD interface
here:					 	// location to jump
	
      	printf("Selamat Datang");
	__delay_ms(1000);
      	lcd_clear();
     	__delay_ms(1000);		      	
	goto here;			 	// goto here
}						//end of main
