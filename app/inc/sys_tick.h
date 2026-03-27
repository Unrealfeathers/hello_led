#ifndef SYS_TICK_H
#define SYS_TICK_H

#include "gd32f10x.h"

void delay_1ms(uint32_t count);
void delay_decrement(void);
void led_spark(void);

#endif /* SYS_TICK_H */