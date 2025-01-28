 /*
 * MAIN Generated Driver File
 * 
 * @file main.c
 * 
 * @defgroup main MAIN
 * 
 * @brief This is the generated driver implementation file for the MAIN driver.
 *
 * @version MAIN Driver Version 1.0.2
 *
 * @version Package Version: 3.1.2
*/

/*
© [2025] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/
#include "mcc_generated_files/system/system.h"

// 7 segment and buffer arrays
// BCD to 7 segment decoder array
unsigned char seven[11] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x00};
extern uint8_t displayBuffer[] = {0x00, 0x01, 0x00, 0x02, 0x00, 0x04, 0x00, 0x08};

void display_update(uint16_t val);

/*
                         Main application
 */

int main(void)
{
    SYSTEM_Initialize();
    
    DMA1_DMAPrioritySet(0);
    DMA1_SetDestinationAddress( (uint16_t)(&LATA) );
    DMA1_StartTransferWithTrigger();    
     
	uint16_t count = 9999;
    
    while (1)
    {
		display_update(count--);
		__delay_ms(1000);
    }
}


void display_update(uint16_t val)
{
	uint8_t temp, indx;
	uint16_t powr;

	powr = 1000;                                //set limit for 4 digits
	for(indx = 0; indx < 4; indx++)             //step through all 4
	{
		temp = val/powr;                        //get the most significant digit
		val  = val - (temp * powr);             //subtract it from the remaining value
		temp = seven[temp];                     //convert the value to 7 segments
		if (indx == 1) temp = temp | 0x80;      //if digit 2 turn on decimal point
		displayBuffer[(indx * 2)] = temp;       //output display value to buffer
		powr = (uint8_t)(powr / 10);            //setup power for next significant digit
	}
}