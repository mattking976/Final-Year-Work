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

// include the itm debugging
#include "itm_debug.h"

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
	// print a status message to the serial debug (printf) window
	print_debug("we are alive!", 13);

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
	// print some status message to the debug serial window
	print_debug("xbee rx thread running!", 23);

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
				print_debug(">> packet received", 18);
				
				// get the packet
				uint8_t packet[len];
				get_packet(packet);
				
				// process our complete xbee packet
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
		// if there are no digital channels ...
		if(packet[16] + packet[17] == 0)
		{
			// if there are two analog channels read both
			if(packet[18] == 3)
			{
				// convert to an integer
				uint16_t temp = (packet[19] << 8) | packet[20];
				uint16_t light = (packet[21] << 8) | packet[22];
				
				// put all the data into our mailbox and transfer to the display thread
				
				// create our mail (i.e. the message container)   
				mail_t* mail = (mail_t*) osMailAlloc(mail_box, osWaitForever);   
				
				// populate the data fields
				mail->xbee_id = packet[11] | (packet[10] << 8);
				mail->temperature = temp;
				mail->light = light;
				
				// put the data in the mail box
				osMailPut(mail_box, mail);
				
			}
		}

		else
		{
			printf("network problems! %02X\r\n", packet[17]);
		}
		printf("\r\n");
	}
}

