/**
  DMA1 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    dma1.c

  @Summary
    This is the generated driver implementation file for the DMA1 driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides implementations for driver APIs for DMA1.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.78
        Device            :  PIC18F57Q43
        Driver Version    :  1.0.0
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.05 and above
        MPLAB             :  MPLAB X 5.20
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip software and any
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party
    license terms applicable to your use of third party software (including open source software) that
    may accompany Microchip software.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS
    FOR A PARTICULAR PURPOSE.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS
    SOFTWARE.
*/

/**
  Section: Included Files
*/

#include "dma.h"
#include <xc.h>

/******************************************************************************
*	 					Variable Declarations
******************************************************************************/
// BCD to 7 segment decoder array
unsigned char seven[11]   = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x00};
// display buffer array {value1,digit1,value2,digit2,value3,digit3,value4,digit4}
unsigned char buffer[8] = {0x4F, 0x01, 0x66, 0x02, 0x7D, 0x04, 0x6F, 0x08};

/**
  Section: DMA1 APIs
*/
void DMA1_Initialize(void)
{
    DMASELECT = 0;			// Select DMA1
    DMAnSSA = &buffer[0]; 	//set source start address: display buffer
    DMAnDSA = 0x04BE; 		//set destination start address: LATA (LATB is 0x04BF)
    DMAnCON1 = 0x42; 		//set control register1: both source and destination increment
    DMAnSSZ = 0x0008; 		//set source size is 8 locations: buffer, 4 segment, 4 digit
    DMAnDSZ = 0x0002; 		//set destination size: PORTA and PORTB
    DMAnSIRQ = 0x1F; 		//set DMA Transfer Trigger Source: TMR0 trigger
    DMAnAIRQ = 0x01; 		//set DMA Transfer abort Source: none
    DMAnCON0 = 0xC0; 		//set control register0: enable

	asm ("BANKSEL PRLOCK");	// unlock priority setting register
    asm ("MOVLW 0x55");
    asm ("MOVWF PRLOCK");
    asm ("MOVLW 0xAA");
    asm ("MOVWF PRLOCK");
    asm ("BSF PRLOCK, 0");
	DMA1PR	=	0;			//set DMA1 to highest priority
}

/**
  Section: Display update APIs
*/
void display_update(uint16_t val)
{
	uint8_t temp, indx;
	uint16_t powr;

	powr = 1000;                           //set limit for 4 digits
	for(indx = 0; indx < 4; indx++)        //step through all 4
	{
		temp = val/powr;                   //get the most significant digit
		val  = val - (temp * powr);        //subtract it from the remaining value
		temp = seven[temp];                //convert the value to 7 segments
		if (indx == 1) temp = temp | 0x80; //if digit 2 turn on decimal point
		buffer[(indx * 2)] = temp;         //output display value to buffer
		powr = powr / 10;                  //setup power for next significant digit
	}
}

/**
  End of File
*/
