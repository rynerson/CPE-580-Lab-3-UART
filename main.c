
#include "stdint.h"
#include "PLL.h"
#include "UART.h"
#include "tm4c123gh6pm.h"



// basic functions defined at end of startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
 
 
int main(){
	uint32_t numb;
	char retchar;
	
	// Functions
	DisableInterrupts(); // The function to disarm the Interrupts - Disabling the interrupt before the PLL configuration
  PLL_Init();          // change this function in PLL.c to configure bus clock at 50 MHz

		// change following module in UART.c to configure UART0 for 8-bit (no parity, One stop) and 38400 baud
		UART_Init(); 	// Initialize UART
		int k = 0;
		for(k=0;k<400;k++){}
		// No need to change the following line- if done correctly then Putty Should display the correct message
		UART_printf("UART Initialization complete");               // Instructor given function to print string
		Newline();																								 // Instructor given function to go to new line
		
		while(1){
			// The following code is used to test your code for part B of the assignment. You need to complete
      //	UART_GetChar() and UART_SendChar(), UART_inUDec(), and UART_OutUDec()  functions in UART.c
			retchar = UART_GetChar();
			UART_SendChar(retchar);   // all lines in the while(1) loop needs to be commented and breakpoint added here for part D
			Newline();	
						
			numb = UART_inUDec();
			Newline();
			UART_OutUDec(numb);
			Newline();
			
			// The following code is used to test your code for part C of the assignment.
			Unit_Convert();		
			
		}
}
	
