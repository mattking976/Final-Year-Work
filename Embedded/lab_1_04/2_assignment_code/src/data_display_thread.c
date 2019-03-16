/*
 * data_display_thread.c
 *
 * this is a thread that pulls the data generated by another thread from a
 * mail queue and then displays it in a terminal
 *
 * author:    Dr. Alex Shenfield
 * date:      06/09/2018
 * purpose:   55-604481 embedded computer networks : lab 103
 */

// include cmsis_os for the rtos api and the stdio package for printing
#include "cmsis_os.h"
#include <stdio.h>

// include main.h (this is where we initialise the mail type)
#include "main.h"
#include "gpio.h"
// include the shu bsp libraries for the stm32f7 discovery board and the serial
// configuration files
#include "pinmappings.h"
#include "clock.h"
#include "vcom_serial.h"

#include "stm32746g_discovery_lcd.h"

// PIR pin mapping
gpio_pin_t pir = {PG_7, GPIOG, GPIO_PIN_7};


// FUNCTION PROTOTYPES
void init_lcd(void);

// LCD DEFINES

// define a message boarder (note the lcd is 28 characters wide using Font24)
#define BOARDER     "****************************"

// specify a welcome message
const char * welcome_message[2] = 
{
  "*     Hello LCD World!     *",
  "*      Welcome to SHU      *"
};

// RTOS DEFINES

// declare the thread function prototypes, thread id, and priority
void display_thread(void const *argument);
osThreadId tid_display_thread;
osThreadDef(display_thread, osPriorityNormal, 1, 0);

// the mailbox we are pulling data from is declared elsewhere ...
extern osMailQId mail_box;

// THREAD INITIALISATION

// create the data display thread
int init_display_thread(void)
{
  // initialize peripherals (i.e. the uart) here
  init_uart(9600);
  printf("display thread up and running ...\r\n");
  
	// initialise the lcd
	init_lcd();
	
	// create the thread and get its task id
  tid_display_thread = osThreadCreate(osThread(display_thread), NULL);

  // check if everything worked ...
  if(!tid_display_thread)
  {
    return(-1);
  }
  return(0);
}

// ACTUAL THREADS

// data display thread - pull the data out of the mail queue and print it to
// the uart
void display_thread(void const *argument)
{
  // infinite loop getting out fake data ...
  while(1)
  {
    // get the data ...
    osEvent evt = osMailGet(mail_box, osWaitForever);
    if(evt.status == osEventMail)
    {
      mail_t *mail = (mail_t*)evt.value.p;
			
			// get the temperature value as degrees c and light 
			// value as a rough %
			float temp = (((mail->temperature * 1200.0) / 1024.0) - 500) / 10.0;
			int llevel = (mail->light / 10.0);
			
			init_gpio(pir, INPUT);
			
			// format some strings
			char str1[32];
			char str2[32];			
			char str3[32];
			sprintf(str1, "Temperature: %3.2f", temp);
			sprintf(str2, "Light:       %03d", llevel);	
			int pir_val = read_gpio(pir);
			sprintf(str3, "Pir value is : %3d", pir_val);
			
			// display on uart					
			printf("\r\n");
      printf("XBee ID:     %d\n\r",	mail->xbee_id);
      printf("%s\n\r", str1);
      printf("%s\n\r", str2);
			printf("%s\n\r", str3);

			// print the message to the lcd
			if (mail->xbee_id == 21821)
			{
				BSP_LCD_ClearStringLine(5);			
				BSP_LCD_ClearStringLine(6);
				BSP_LCD_ClearStringLine(7);
				BSP_LCD_DisplayStringAtLine(5, (uint8_t *)"Zone 1:");			
				BSP_LCD_DisplayStringAtLine(6, (uint8_t *)str1);
				BSP_LCD_DisplayStringAtLine(7, (uint8_t *)str2);
				
			}
			else
			{
				BSP_LCD_ClearStringLine(9);			
				BSP_LCD_ClearStringLine(10);
				BSP_LCD_ClearStringLine(11);
				BSP_LCD_DisplayStringAtLine(9,  (uint8_t *)"Zone 2:");			
				BSP_LCD_DisplayStringAtLine(10, (uint8_t *)str1);
				BSP_LCD_DisplayStringAtLine(11, (uint8_t *)str2);
				BSP_LCD_DisplayStringAtLine(12, (uint8_t *)str3);
			}
			
			// free the mailbox
      osMailFree(mail_box, mail);
    }
  }
}
 
// UTILITY FUNCTIONS

// initialise lcd and print a message
void init_lcd(void)
{
	// initialise the lcd
  BSP_LCD_Init();
  BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER, SDRAM_DEVICE_ADDR);
  BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER);

  // set the background colour to blue and clear the lcd
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_LCD_Clear(LCD_COLOR_BLUE);
  
  // set the font to use
  BSP_LCD_SetFont(&Font16); 
  
  // print the welcome message ...
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_DisplayStringAtLine(0, (uint8_t *)BOARDER);
  BSP_LCD_DisplayStringAtLine(1, (uint8_t *)welcome_message[0]);
  BSP_LCD_DisplayStringAtLine(2, (uint8_t *)welcome_message[1]);
  BSP_LCD_DisplayStringAtLine(3, (uint8_t *)BOARDER); 
}