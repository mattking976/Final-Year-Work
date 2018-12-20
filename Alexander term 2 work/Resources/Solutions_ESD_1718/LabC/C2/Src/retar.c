// minimalistic code to retarget PRINTF to SWD
// just use PRINTF as appropriate
#include <stdio.h>
struct __FILE {
    int dummy;
};
FILE __stdout;
int fputc(int ch, FILE *f) {
		HAL_Delay(1);
		CDC_Transmit_FS((uint8_t*)&ch,1);
		HAL_Delay(1);
    return ITM_SendChar(ch);
}
