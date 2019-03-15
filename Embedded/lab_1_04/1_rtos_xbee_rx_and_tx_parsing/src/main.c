/*
 * main.c
 *
 * this is the main rtos xbee / uart based application
 *
 * author:		Alex Shenfield
 * date:			08/11/2018
 * purpose:   55-604481 embedded computer networks : lab 104
 */

// include the basic headers for the hal drivers and the rtos library
#include "stm32f7xx_hal.h"
#include "cmsis_os.h"
#include "stm32746g_discovery_ts.h"
#include "stm32746g_discovery_lcd.h"

// include the shu bsp libraries for the stm32f7 discovery board
#include "pinmappings.h"
#include "clock.h"
#include "gpio.h"

// include the xbee tx and rx functionality
#include "xbee.h"

// include the itm debugging
#include "itm_debug.h"

// RTOS

#define BOARDER     "****************************"
// specify a welcome message
const char * welcome_message[2] =
{
	"*     Hello LCD World!     *",
	"*      Welcome to SHU      *"
};

// declare the extern methods that set everything up for us
extern int init_xbee_threads(void);

// OVERRIDE HAL DELAY

// make HAL_Delay point to osDelay (otherwise any use of HAL_Delay breaks things)
void HAL_Delay(__IO uint32_t Delay)
{
	osDelay(Delay);
}

// XBEE

// xbee configuration packets

// set up adc on dio 0 on all xbees connected to the WPAN - temperature
uint8_t init_adc_0[]         = {0x7E, 0x00, 0x10, 0x17, 0x01, 0x00, 0x00, 0x00, 
                                0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFE, 0x02, 
	                              0x44, 0x30, 0x02, 0x74
                               };

// set up adc on dio 1 on all xbees connected to the WPAN - light
uint8_t init_adc_1[]         = {0x7E, 0x00, 0x10, 0x17, 0x02, 0x00, 0x00, 0x00, 
                                0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFE, 0x02, 
	                              0x44, 0x31, 0x02, 0x72
                               };

// configure all radios in the WPAN to do automatic sampling every 10 seconds
uint8_t configure_sampling[] = {0x7E, 0x00, 0x11, 0x17, 0x01, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFE, 0x02, 
	                              0x49, 0x52, 0x27, 0x10, 0x18
                               };

// CODE

// this is the main method
int main()
{
	// we need to initialise the hal library and set up the SystemCoreClock
	// properly
	HAL_Init();
	init_sysclk_216MHz();

	BSP_LCD_Init();
	BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER, SDRAM_DEVICE_ADDR);
	BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER);

	// set the background colour to black and clear the lcd
	BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
	BSP_LCD_Clear(LCD_COLOR_BLACK);

	// draw a blue button
	BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
	BSP_LCD_FillRect(50, 110, 145, 52);

	// write text on to it
	BSP_LCD_SetFont(&Font20);
	BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
	BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
	BSP_LCD_DisplayStringAt(60, 128, (uint8_t *)"Press me!", LEFT_MODE);

	// initialise a toggle state variable for our virtual led
	uint8_t led = 0;

	// draw our virtual led
	BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
	BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
	BSP_LCD_DrawCircle(350, 136, 26);

	// initialise the touch screen state structure to store information about
	// the touches
	TS_StateTypeDef TS_State;
	BSP_TS_Init(480, 272);

	// main loop
	while(1)
	{
		// get the state of the touch screen
		BSP_TS_GetState(&TS_State);

		// if a touch is detected ...
		if(TS_State.touchDetected)
		{
			// work out if any of the touches are in the area of the button
			uint8_t idx;
			for(idx = 0; idx < TS_State.touchDetected; idx++)
			{
				uint16_t x = TS_State.touchX[idx];
				uint16_t y = TS_State.touchY[idx];
				if(x > 50 && x < 195 && y > 110 && y < 162)
				{
					led = !led;
				}
			}

			// update the "led"
			if(led)
			{
				// draw a filled "led"
				BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
				BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
				BSP_LCD_FillCircle(350, 136, 26);
			}
			else
			{
				// clear the led
				BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
				BSP_LCD_FillCircle(350, 136, 26);

				// draw an empty "led"
				BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
				BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
				BSP_LCD_DrawCircle(350, 136, 26);
			}

			// dumb ass delay rather than debouncing because i'm lazy
			HAL_Delay(500);
		}

	}
}
