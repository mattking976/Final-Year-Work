//******************************************************************************************************************************
FATFS USBDISKFatFs;
FIL MyFile;
uint8_t USBstate;
//******************************************************************************************************************************		
	uint32_t OldClk, NewClk, cnt;
	OldClk = HAL_GetTick();
	printf("%d\n\r",f_mount(&USBDISKFatFs, (TCHAR const*)&USBHPath, 0));
//******************************************************************************************************************************
			NewClk=HAL_GetTick();
			if ( NewClk > (OldClk+2400) )	{
				cnt++;
				HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_13);			
				OldClk=NewClk;
				if (  f_open(&MyFile, "STM.TXT", FA_OPEN_EXISTING | FA_READ| FA_WRITE) == FR_OK )	{					
							f_lseek(&MyFile, f_size(&MyFile));
							HAL_RTC_GetTime (&hrtc, &sTime, RTC_FORMAT_BIN);
							f_printf(&MyFile, "%d:",sTime.Hours);
							f_printf(&MyFile, "%d:",sTime.Minutes);
							f_printf(&MyFile, "%d\n",sTime.Seconds);					
							f_close(&MyFile);
							printf("Written to the STM.TXT file:%d\n\r",cnt);
				}	
				else 	{
					printf("STM.TXT file opening error. CNT=%d\n\r", cnt);	
					f_close(&MyFile);
				}
			}
//******************************************************************************************************************************