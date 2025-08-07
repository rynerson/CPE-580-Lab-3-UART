// UART.c
/*Some Example Modules are taken from Book: "
	"Embedded Systems: Introduction to ARM Cortex M Microcontrollers",
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2014
*/
// this connection occurs in the USB debugging cable
// U0Rx (PA0) connected to serial port on PC
// U0Tx (PA1) connected to serial port on PC
// Ground connected ground in the USB cable


#include <stdint.h>
#include "UART.h"
#include "tm4c123gh6pm.h"

#define Todo	0

#define UART_FR_TXFF            0x00000020  // UART Transmit FIFO Full
#define UART_FR_RXFE            0x00000010  // UART Receive FIFO Empty
#define UART_LCRH_WLEN_8        0x00000060  // 8 bit word length
#define UART_LCRH_FEN           0x00000010  // UART Enable FIFOs
#define UART_CTL_UARTEN         0x00000001  // UART Enable


//------------UART_Init------------
// Initialize the UART for 38400 baud rate (50Mhz UART clock - After enabling PLL),
// 8 bit word length, no parity bits, one stop bit, FIFOs enabled
// Input: none
// Output: none
void UART_Init(void){
	// You should find all the values in below code as per configuaration given and replace Todos
	
  SYSCTL_RCGCUART_R |= 0x01;            							// activate UART0
  SYSCTL_RCGCGPIO_R |= 0x01;            							// activate port A
	
	
  while((SYSCTL_PRGPIO_R&0x01) == 0){};
  UART0_CTL_R &= ~UART_CTL_UARTEN;      							// disable UART
		
	// IBRD and FBRD is very critical, follwoing is set for 115200 baud and PLL set at 80M, you should modify for requested configuration (38400 baud and PLL set at 50MHz)
  UART0_IBRD_R = 81;                    							// IBRD = int(50,000,000 / (16 * 38400)) = 81
  UART0_FBRD_R = 25;                     							// FBRD = int(0.4028 * 64+0.5) = 26
  
	// 8 bit word length (no parity bits, one stop bit, FIFOs)
  UART0_LCRH_R = (UART_LCRH_WLEN_8|UART_LCRH_FEN);
		
	// The following values are delibrately changed, you should find as per the instruction in comment
  UART0_CTL_R |= 0x01;                 							// enable UART
  GPIO_PORTA_AFSEL_R |=0x03;           						// enable alt funct on PA1-0
  GPIO_PORTA_DEN_R |= 0x03;             						// enable digital I/O on PA1-0
  
	// configure PA1-0 as UART
  GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R&0xFFFFFF00)+0x00000011;
	
			
  //GPIO_PORTA_AMSEL_R &= 0x01;          // disable analog functionality on PA
}

//------------Getchar-----------
// Wait for new serial port input
// Input: none (reads from UART0) 
// Output: ASCII code for key typed
// Should block until the user presses a key and then returns that value
// Returns a newline until it has been correctly implemented
char UART_GetChar(void){
  char retchar = '\n';
		while((UART0_FR_R&0x0010) != 0);
	// Add your code for following functions
	  // Wait until the Receive FIFO empty flag (RXFE) is 0
		// This means data has been received and is available

	// Put the result from data register to retchar
	retchar = (char)(UART0_DR_R&0xFF);
	// Return
	return retchar;
}

void UART_SendChar(char data){
	while((UART0_FR_R&0x0020) !=0);
	UART0_DR_R = data;
		// Add your code for following tasks
	  // Wait until the Transmit FIFO full flag (TXFF) becomes 0
		// This means that there is room for data to be sent out
	
	// Put data to the data register 
  
}
//------------UART_InUDec------------
// InUDec accepts ASCII input in unsigned decimal format
//     and converts to a 32-bit unsigned number
//     valid range is 0 to 4294967295 (2^32-1)
// Input: none (reads from UART0)
// Output: 32-bit unsigned number
// If you enter a number above 4294967295, it will return an incorrect value
// Backspace will remove last digit typed
uint32_t UART_inUDec(void){
	uint32_t number=0, length=0;
	char character;
  character = UART_InChar();
  
	while(character != 0x0D){ // accepts until <enter - i.e. CR> is typed
		
		// The next line checks that the input is a digit, 0-9.
		// If the character is not 0-9, it is ignored and not echoed
    if((character>='0') && (character<='9')) {
      number =   10*number+(character-0x30); 										// this line overflows if above 4294967295
      length++;
      UART_OutChar(character);       // Instructor defined function, no change
    }
		// Else If the input is a backspace, then the return number is
		// changed and a backspace is outputted to the screen
    else if((character==BS) && length){
      number = character;												// write your logic here
      length--;
      UART_OutChar(character);					// Instructor defined function, no change
    }
		
		// Default accept
    character = UART_InChar();
  }
  return number;
}

//-----------------------UART_OutUDec-----------------------
// Output a 32-bit number in unsigned decimal format
// Input: 32-bit number to be transferred
// Output: none
// Variable format 1-10 digits with no space before or after
void UART_OutUDec(uint32_t n){
// This function uses recursion to convert decimal number
//   of unspecified length as an ASCII string
	if(n>=10){
		UART_OutUDec(n/10);
		n = n%10;
	}
UART_OutChar(n + '0');
}

void Unit_Convert(void) {
	while(1){
		Newline();
		UART_printf("Choice Description");
		Newline();
		UART_printf("0 Convert millimeter to centimeter");
		Newline();
		UART_printf("1 Convert centimeter to millimeter");
		Newline();
		UART_printf("2 Convert meter to centimeter");
		Newline();
		uint32_t num = UART_inUDec();
		if(num == 0){
			Newline();
			UART_printf("Enter a number");
			Newline();
			num = UART_inUDec();
			num = (num)/10;
			Newline();
			UART_OutUDec(num);
		}
		else if(num == 1){
			Newline();
			UART_printf("Enter a number");
			Newline();
			num = UART_inUDec();
			num = (num)*10;
			Newline();
			UART_OutUDec(num);
		}
		else if(num == 2){
			Newline();
			UART_printf("Enter a number");
			Newline();
			num = UART_inUDec();
			num = (num)*100;
			Newline();
			UART_OutUDec(num);
		}
	}		//complete this function for part C of the assignment (Simple unit conversion).
}


/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
/*---------------Instructor Defnied Functions------------------------
 Following are Instructor Defined functions and you should not change any
-------------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
// You should look following function, it may help to write up your code

char UART_InChar(void){
  while((UART0_FR_R&UART_FR_RXFE) != 0);
  return((unsigned char)(UART0_DR_R&0xFF));
}
//------------UART_OutChar------------
// Output 8-bit to serial port
// Input: letter is an 8-bit ASCII character to be transferred
// Output: none
void UART_OutChar(char data){
  while((UART0_FR_R&UART_FR_TXFF) != 0);
  UART0_DR_R = data;
}


//------------UART_OutString------------
// Output String (NULL termination)
// Input: pointer to a NULL-terminated string to be transferred
// Output: none
void UART_printf(char *pt){
  while(*pt){
    UART_OutChar(*pt);
    pt++;
  }
}


//---------------------Newline---------------------
// Output a CR,LF to UART to go to a new line
// Input: none
// Output: none
void Newline(void){
  UART_OutChar(CR);
  UART_OutChar(LF);
}


//------------UART_InString------------
// Accepts ASCII characters from the serial port
//    and adds them to a string until <enter> is typed
//    or until max length of the string is reached.
// It echoes each character as it is inputted.
// If a backspace is inputted, the string is modified
//    and the backspace is echoed
// terminates the string with a null character
// uses busy-waiting synchronization on RDRF
// Input: pointer to empty buffer, size of buffer
// Output: Null terminated string

void UART_InString(char *bufPt, uint16_t max) {
int length=0;
char character;
  character = UART_InChar();
  while(character != CR){
    if(character == BS){
      if(length){
        bufPt--;
        length--;
        UART_OutChar(BS);
      }
    }
    else if(length < max){
      *bufPt = character;
      bufPt++;
      length++;
      UART_OutChar(character);
    }
    character = UART_InChar();
  }
  *bufPt = 0;
}
