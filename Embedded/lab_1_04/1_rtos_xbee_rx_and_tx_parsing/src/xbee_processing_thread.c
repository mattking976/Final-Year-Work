/*
 * xbee_processing_thread.c
 *
 * xbee data processing thread which pulls the bytes out of the message queue 
 * (put in by the uart 1 irq handler) and uses the xbee packet parser state 
 * machine to turn them into complete packets 
 *
 * author:    Alex Shenfield
 * date:      08/11/2018
 * purpose:   55-604481 embedded computer networks : lab 104
 */

// include the relevant header files (from the c standard libraries)
#include <stdio.h>
#include <string.h>

// include the rtos api
#include "cmsis_os.h"

// include the serial configuration files
#include "vcom_serial.h"
#include "xbee.h"

// include the xbee packet parser
#include "xbee_packet_parser.h"

// include main.h with the mail type declaration
#include "main.h"

// RTOS DEFINES

// declare the thread function prototypes, thread id, and priority
void xbee_rx_thread(void const *argument);
osThreadId tid_xbee_rx_thread;
osThreadDef(xbee_rx_thread, osPriorityAboveNormal, 1, 0);

// setup a message queue to use for receiving characters from the interrupt
// callback
osMessageQDef(message_q, 128, uint8_t);
osMessageQId msg_q;

// set up the mail queues
osMailQDef(mail_box, 16, mail_t);
osMailQId  mail_box;

// FUNCTION PROTOTYPE

// process packet function
void process_packet(uint8_t* packet, int length);

// THREAD INITIALISATION

// create the uart thread(s)
int init_xbee_threads(void)
{
	// print a status message to the vcom port
	init_uart(9600);
	printf("we are alive!\r\n");

	// create the message queue
	msg_q = osMessageCreate(osMessageQ(message_q), NULL);

	// create the mailbox
	mail_box = osMailCreate(osMailQ(mail_box), NULL);

	// create the thread and get its task id
	tid_xbee_rx_thread = osThreadCreate(osThread(xbee_rx_thread), NULL);

	// check if everything worked ...
	if(!tid_xbee_rx_thread)
	{
		printf("thread not created!\r\n");
		return(-1);
	}
	
	return(0);
}

// ACTUAL THREADS

// xbee receive thread
void xbee_rx_thread(void const *argument)
{
	// print some status message ...
	printf("xbee rx thread running!\r\n");

	// infinite loop ...
	while(1)
	{
		// wait for there to be something in the message queue
		osEvent evt = osMessageGet(msg_q, osWaitForever);

		// process the message queue ...
		if(evt.status == osEventMessage)
		{
			// get the message and increment the counter
			uint8_t byte = evt.value.v;
			
			// feed the packet 1 byte at a time to the xbee packet parser
			int len = xbee_parse_packet(byte);
			
			// if len > 0 then we have a complete packet so dump it to the virtual 
			// com port
			if(len > 0)
			{
				printf(">> packet received\r\n");
				
				// get the packet
				uint8_t packet[len];
				get_packet(packet);
				
				// display the packet
				int i = 0;
				for(i = 0; i < len; i++)
				{
					printf("%02X ", packet[i]);
				}
				printf("\r\n");
				
				process_packet(packet, len);
			}			
		}
	}
}

// process an xbee packet
void process_packet(uint8_t* packet, int length)
{
	// check it is an explicit io sample received packet (because otherwise the
	// structure will be wrong)
	if(packet[3] == 0x92)
	{
		// print the xbee long address
		printf("xbee long address is: ");
		int i = 0;
		for(i = 4; i < 12; i++)
		{
			printf("%02X ", packet[i]);
		}
		printf("\r\n\r\n");
		// print the xbee short address
		printf("xbee short address is: ");
		printf("%02X %02X\r\n\r\n", packet[12], packet[13]);
		
		// print out the data that we have - this is the bit of the packet that
		// contains the adc values from the light sensor and temperature sensor
		int datastart = 16;
		if(datastart < (length - 1))
		{
			printf("data = ");
			while(datastart < (length - 1))
		{
			printf("%02X ", packet[datastart++]);
		}
			printf("\r\n\r\n");
		}
		
//		// print temp data 
//		printf("Temp data: ");
//		printf("%02X %02X\r\n\r\n", packet[19], packet [20]);
//		
//		int temp = (packet[19]+packet[20]);
//		
//		printf("Temp :");
//		printf("2d\r\n", temp);
//		
//		// print light data
//		printf("Light data: ");
//		printf("%02X %02X\r\n\r\n", packet[21], packet [22]);

		
		// extract the raw adc values from the io sample rx packet
		//
		// the data field looks like:
		// 01 number of samples
		// 00 00 digital channel mask
		// 03 analog channel mask
		// 02 7D 02 1C analog samples
		//
		// note: we are assuming a maximum of 2 analog channels, no digital
		// channels, and that we are only using dio0 and dio1 (if we are using
		// other inputs we need to adjust this code)
			
		// if there are no digital channels ...
		if(packet[16] + packet[17] == 0)
		{
			// if there is one analog channel read that
			if(packet[18] == 1 || packet[18] == 2)
			{
				printf("adc 1 value is : %4d\r\n", (packet[19] << 8) | packet[20]);
			}
			// if there are two analog channels read both
			if(packet[18] == 3)
			{
				// convert to an integer aand print to display
				uint16_t temp = (packet[19] << 8) | packet[20];
				uint16_t light = (packet[21] << 8) | packet[22];
				printf("adc 1 value is : %4d\r\n", temp);
				printf("adc 2 value is : %4d\r\n", light);
				
				// put into our mailbox (?) and transfer to the display thread
				
				
			}
		}

		else
		{
			printf("network problems! %02X\r\n", packet[17]);
		}
		printf("\r\n");
	}
}

