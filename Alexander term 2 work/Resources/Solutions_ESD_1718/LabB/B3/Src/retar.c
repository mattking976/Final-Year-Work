// minimalistic code to retarget PRINTF to SWD
// just use PRINTF as appropriate
#include <stdio.h>
struct __FILE {
    int dummy;
};
FILE __stdout;
int fputc(int ch, FILE *f) {
    return ITM_SendChar(ch);
}
