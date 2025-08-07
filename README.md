# CPE-580-Lab-3-UART
WHAT:
This lab focused on using the UART (Universal Asynchronous Receiver/Transmitter) to build a simple unit conversion program.

WHY:
The goal was to gain hands-on experience with UART initialization and to practice sending and receiving characters using the UART module. This helped build confidence in working with serial communication on embedded systems.

HOW:
The lab used UART0 on a Tiva-C TM4C123GH6PM microcontroller. The process began with initializing UART0. After that, I implemented functions to receive and transmit data through the UART interface.

    For receiving, I checked whether data was available by reading the RXFE flag (Receive FIFO Empty).

    For transmitting, I ensured the transmit FIFO buffer was not full by checking the TXFF flag (Transmit FIFO Full) before sending characters.

Once the UART was functional, I developed basic functions for unit conversion and created a simple text-based interface to let the user choose between options.

TOOLS USED:

    Laptop

    Keil uVision

    TIVA-C TM4C123 Launchpad

    PA3 Template

    Embedded Systems: Shape the World (course material)

    Lecture 5 (UART)

    PuTTY (for serial communication)
